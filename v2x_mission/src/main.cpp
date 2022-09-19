#include <iostream>

#include <thread>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string.h>

#include "obu_packet.h"

#define DISPLAY_PACKET false
#define MAX_READ_LEN  4096
#define MAX_BUFFER_LEN  1000000

int clnt_sock = -1;
std::string ip = "192.168.10.10";

uint32_t port = 24000;

uint8_t seq = 0;

int Connect_socket();
int Disconnect_socket();

int RecvMissionSatge1(unsigned char* buf);
int RecvMissionSatge2(unsigned char* buf);
int RecvRequestAck(unsigned char* buf);
int RecvItemAck(unsigned char* buf);

void SendRequest(unsigned char id, unsigned char req);

void ParseMissionStage1(MissionListStage1 *msg, unsigned char* buf);
void ParseMissionStage2(MissionListStage2 *msg, unsigned char* buf);
void ParseRequestAck(Request_Ack *msg, unsigned char* buf);
void ParseItemAck(Item_Ack *msg, unsigned char* buf);

void PrintMissionStage1(MissionListStage1 *msg);
void PrintMissionStage2(MissionListStage2 *msg);
void PrintRequestAck(Request_Ack *msg);
void PrintItemAck(Item_Ack *msg);

int main()
{ 
    int sock_connect_state = -1;
    std::string buf;
    while (true)
    {
        if(sock_connect_state == -1)
        {
            Disconnect_socket();
            sock_connect_state = Connect_socket();
            buf.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }
        
        unsigned char recvbuf[MAX_READ_LEN] = {0,};
        int len = recv(clnt_sock, recvbuf, sizeof(recvbuf), 0); //blocking function!
        
        if(len <= 0)
        {
            sock_connect_state = Disconnect_socket();
            continue;
        }
        
        if(len <= MAX_READ_LEN)
        {
            if(buf.size() > MAX_BUFFER_LEN)
            {
                buf.clear();
            }
            buf.append((char*)recvbuf, len);
        }
        
        while (true)
        {
            if(buf.size() < sizeof(MsgHeader))
            {
                break;;
            }
            unsigned char* temp_buf = (unsigned char*)buf.c_str();
            MsgHeader *header = (MsgHeader*)temp_buf;
        
            if(buf.size() < sizeof(MsgHeader) + header->payload_length )
            {
                printf("pay size error! buffering... %d / %d\n", buf.size(),  sizeof(MsgHeader) + header->payload_length);
                break;
            }
            
            switch (header->message_type)
            {
            case MessageType::MISSION_LIST_STAGE1 :
                {              
                    RecvMissionSatge1(temp_buf);
                }
                break;
            case MessageType::MISSION_LIST_STAGE2 :
                {
                    RecvMissionSatge2(temp_buf);
                }
                break;
            case MessageType::REQUEST_ACK :
                {
                    RecvRequestAck(temp_buf);
                }
                break;
            case MessageType::ITEM_ACK :
                {
                    RecvItemAck(temp_buf);
                }
                break;
            default:
                {
                    printf("Recv unkown message\n");
                    buf.clear();
                }
                break;
            }
           
            buf.erase(0, sizeof(MsgHeader) + header->payload_length);
        }
    }
    
    Disconnect_socket();

    return 0;
}

int Connect_socket()
{
    if(clnt_sock != -1)
    {
        printf("aleady create sock!!\n");
        return -1;
    }
    clnt_sock = socket(PF_INET,SOCK_STREAM,0);
    if(clnt_sock == -1)
    {
        printf("sock create error\n");
        return -1;
    }

    int opt_val = 1;
    setsockopt(clnt_sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, sizeof(opt_val));
    setsockopt(clnt_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&opt_val, sizeof(opt_val));
    
    sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr. s_addr= inet_addr(ip.c_str());
    serv_addr.sin_port = htons(port);
   
    int res = -1;
    res = connect(clnt_sock,(sockaddr*)&serv_addr,sizeof(serv_addr));
    if(res == -1)
    {
        printf("sock connect error\n");
    }
    else
    {
        printf("sock connect success\n");
    }
    return res;
}

int Disconnect_socket()
{
    if(clnt_sock != -1)
    {
        shutdown(clnt_sock, SHUT_RDWR);
        close(clnt_sock);
        clnt_sock=-1;
        printf("sock disconnect\n");
    }    
    return -1;
}

int RecvMissionSatge1(unsigned char* buf)
{
    printf("\n[RecvMissionSatge1] start\n");
    MissionListStage1 msg = {0,};
    
    ParseMissionStage1(&msg, buf);
    PrintMissionStage1(&msg);
    //select mission 
    if(msg.mission_count > 0)
    {
        if(msg.mission_list[0].status == 0x00)
        {
            printf("select mission %d\n", msg.mission_list[0].mission_id);
            SendRequest(msg.mission_list[0].mission_id, RequestType::REQ_SELECT_MISSION);
        }
    }

    if(msg.mission_list != nullptr)
    {
        delete msg.mission_list;
    }
    if(msg.mission_route_list != nullptr)
    {
        delete msg.mission_route_list;
    }

    return 0;
}
int RecvMissionSatge2(unsigned char* buf)
{
    printf("\n[RecvMissionSatge2] start\n");

    MissionListStage2 msg = {0,};

    ParseMissionStage2(&msg, buf);
    PrintMissionStage2(&msg);

    if(msg.item_list != nullptr)
    {
        delete msg.item_list;
    }
    return 0;
}
int RecvRequestAck(unsigned char* buf)
{
    printf("\n[RecvRequestAck] start\n");
    Request_Ack msg = {0,};

    ParseRequestAck(&msg, buf);
    PrintRequestAck(&msg);

    if(msg.request == RequestType::REQ_SELECT_MISSION && msg.response == ResponseType::RES_SUCCESS)
    {
        printf("%d번 미션 선택 완료\n", msg.mission_id);
        
        //moving starting position.....
        SendRequest(msg.mission_id, RequestType::REQ_START_POSITION);
    }

    if(msg.request == RequestType::REQ_START_POSITION && msg.response == ResponseType::RES_SUCCESS)
    {
        printf("%d번 출발지 도착 완료\n", msg.mission_id);

        //moving destination position.....
        SendRequest(msg.mission_id, RequestType::REQ_END_POSITION);
    }

    if(msg.request == RequestType::REQ_END_POSITION && msg.response == ResponseType::RES_SUCCESS)
    {
        printf("%d번 목적지 도착 완료\n", msg.mission_id);
    }

    return 0;
}
int RecvItemAck(unsigned char* buf)
{
    printf("\n[RecvItemAck] start\n");
    Item_Ack msg = {0,};
    
    ParseItemAck(&msg, buf);
    PrintItemAck(&msg);

    if(msg.request == RequestType::REQ_ITEM)
    {
        if(msg.response == ItemType::ITEM_LAP_TIME_SUB)
        {
            printf("%d번 아이템 획득 : Lap time 감소\n", msg.mission_id);
        }
        if(msg.response == ItemType::ITEM_LAP_TIME_ADD)
        {
            printf("%d번 아이템 획득 : Lap time 증가\n", msg.mission_id);
        }
        if(msg.response == ItemType::ITEM_BOOST)
        {
            printf("%d번 아이템 획득 : 가속 주행\n", msg.mission_id);
        }
    }

    return 0;
}

void ParseMissionStage1(MissionListStage1 *msg, unsigned char* buf)
{
    if(msg == nullptr)
    {
        printf("[ParseMissionStage1] fail : msg == nullptr\n");
        return;
    }
    printf("[ParseMissionStage1] start\n");
    
    size_t copy_len_1 = sizeof(MissionListStage1) - sizeof(MissionListStage1::mission_list) - sizeof(MissionListStage1::mission_route_list);

    memmove(msg,buf,copy_len_1);

	size_t copy_len_2 = sizeof(MissionData) * msg->mission_count;
    msg->mission_list = new MissionData[msg->mission_count];
    memmove(msg->mission_list, &buf[copy_len_1], copy_len_2);

	size_t copy_len_3 = sizeof(MissionRouteData) * msg->mission_route_count;
    msg->mission_route_list = new MissionRouteData[msg->mission_route_count];
    memmove(msg->mission_route_list, &buf[copy_len_1 + copy_len_2], copy_len_3);
    
}
void ParseMissionStage2(MissionListStage2 *msg, unsigned char* buf)
{
    if(msg == nullptr)
    {
        printf("[ParseMissionStage2] fail : msg == nullptr\n");
        return;
    }
    printf("[ParseMissionStage2] start\n");

    size_t copy_len_1 = sizeof(MissionListStage2) - sizeof(MissionListStage2::item_list);
    memmove(msg,buf,copy_len_1);

    size_t copy_len_2 = sizeof(ItemData) * msg->item_count;
    msg->item_list = new ItemData[msg->item_count];
    memmove(msg->item_list, &buf[copy_len_1],copy_len_2);
    
}
void ParseRequestAck(Request_Ack *msg, unsigned char* buf)
{
    if(msg == nullptr)
    {
        printf("[ParseRequestAck] fail : msg == nullptr\n");
        return;
    }
    printf("[ParseRequestAck] start\n");
    memmove(msg,buf,sizeof(Request_Ack));
}
void ParseItemAck(Item_Ack *msg, unsigned char* buf)
{
    if(msg == nullptr)
    {
        printf("[ParseItemAck] fail : msg == nullptr\n");
        return;
    }
    printf("[ParseItemAck] start\n");
    memmove(msg,buf,sizeof(Item_Ack));
}

void SendRequest(unsigned char id, unsigned char req)
{
    Request msg = {0,};

    msg.header.message_type = MessageType::REQUEST;
    msg.header.sequence = seq++;
    msg.header.payload_length = sizeof(Request) - sizeof(MsgHeader);
    msg.header.device_type = 0xCE;
    msg.header.device_id[0] = 0x01;//team id
    msg.header.device_id[1] = 0x02;//team id
    msg.header.device_id[2] = 0x03;//team id

    msg.mission_id = id;
    msg.request = req;

    msg.response = 0x00;
    sprintf(msg.description,"team_1");//team name
    msg.temporary; //all 0
    msg.end_point = 0x0D0A;
    
    int len = send(clnt_sock, (char*)&msg, sizeof(msg), 0); 
    if(len <=0)
    {
        printf("[SendRequest] fail send\n");
    }
    else
    {
        printf("[SendRequest] id : %d, req : %d, byte : %d\n",id, req, len);
    }
}

void PrintMissionStage1(MissionListStage1 *msg)
{
    if(!DISPLAY_PACKET)
    {
        return;
    }
    if(msg == nullptr)
    {
        printf("[PrintMissionStage1] fail : msg == nullptr\n");
        return;
    }
    printf("[PrintMissionStage1] start\n");
    //print

    printf("\theader.message_type : 0x%04X\n", msg->header.message_type);
    printf("\theader.sequence : %d\n", msg->header.sequence);
    printf("\theader.payload_length : %d\n", msg->header.payload_length);
    printf("\theader.device_type : 0x%02X\n", msg->header.device_type);
    printf("\theader.device_id : 0x%02X 0x%02X 0x%02x\n", msg->header.device_id[0], msg->header.device_id[1], msg->header.device_id[2]);
    printf("\n");
    printf("\tmission_status : 0x%02X\n", msg->mission_status);
    printf("\tmission_count : %d\n", msg->mission_count);
    printf("\tmission_route_count : %d\n", msg->mission_route_count);

    for(int i=0; i<msg->mission_count; i++)
    {
        printf("\tmission_list[%d].mission_id : %d\n", i, msg->mission_list[i].mission_id);
        printf("\tmission_list[%d].status : 0x%02X\n", i, msg->mission_list[i].status);
        printf("\tmission_list[%d].event_count : %d\n", i, msg->mission_list[i].event_count);
        printf("\tmission_list[%d].distance : %d\n", i, msg->mission_list[i].distance);
        printf("\tmission_list[%d].route_id : %d\n", i, msg->mission_list[i].route_id);
        printf("\tmission_list[%d].start_lat : %d\n", i, msg->mission_list[i].start_lat);
        printf("\tmission_list[%d].start_Lon : %d\n", i, msg->mission_list[i].start_Lon);
        printf("\tmission_list[%d].end_lat : %d\n", i, msg->mission_list[i].end_lat);
        printf("\tmission_list[%d].end_lon : %d\n", i, msg->mission_list[i].end_lon);
    }
    for(int i=0; i<msg->mission_route_count; i++)
    {
        printf("\tmission_route_list[%d].mission_route_id : %d\n", i, msg->mission_route_list[i].mission_route_id);
        printf("\tmission_route_list[%d].route_node_total_count : %d\n", i, msg->mission_route_list[i].route_node_total_count);
        printf("\tmission_route_list[%d].route_node_index : %d\n", i, msg->mission_route_list[i].route_node_index);
        printf("\tmission_route_list[%d].route_node_type : %d\n", i, msg->mission_route_list[i].route_node_type);
        printf("\tmission_route_list[%d].route_node_pos_lat : %d\n", i, msg->mission_route_list[i].route_node_pos_lat);
        printf("\tmission_route_list[%d].route_node_pos_lon : %d\n", i, msg->mission_route_list[i].route_node_pos_lon);
    }

}
void PrintMissionStage2(MissionListStage2 *msg)
{
    if(!DISPLAY_PACKET)
    {
        return;
    }
    if(msg == nullptr)
    {
        printf("[PrintMissionStage2] fail : msg == nullptr\n");
        return;
    }
    printf("[PrintMissionStage2] start\n");
    //print

    printf("\theader.message_type : 0x%04X\n", msg->header.message_type);
    printf("\theader.sequence : %d\n", msg->header.sequence);
    printf("\theader.payload_length : %d\n", msg->header.payload_length);
    printf("\theader.device_type : 0x%02X\n", msg->header.device_type);
    printf("\theader.device_id : 0x%02X 0x%02X 0x%02x\n", msg->header.device_id[0], msg->header.device_id[1], msg->header.device_id[2]);
    printf("\n");
    printf("\tmission_status : 0x%02X\n", msg->mission_status);
    printf("\titem_count : %d\n", msg->item_count);

    for(int i=0; i<msg->item_count; i++)
    {
        printf("\titem_list[%d].item_id : %d\n", i, msg->item_list[i].item_id);
        printf("\titem_list[%d].item_type : %d\n", i, msg->item_list[i].item_type);
        printf("\titem_list[%d].item_status : %d\n", i, msg->item_list[i].item_status);
        printf("\titem_list[%d].score : %d\n", i, msg->item_list[i].score);
        printf("\titem_list[%d].speed : %d\n", i, msg->item_list[i].speed);
        printf("\titem_list[%d].duration : %d\n", i, msg->item_list[i].duration);
        printf("\titem_list[%d].pos_lat : %d\n", i, msg->item_list[i].pos_lat);
        printf("\titem_list[%d].pos_long : %d\n", i, msg->item_list[i].pos_long);
        printf("\titem_list[%d].extend : %d\n", i, msg->item_list[i].extend);
    }
    
}
void PrintRequestAck(Request_Ack *msg)
{
    if(!DISPLAY_PACKET)
    {
        return;
    }
    if(msg == nullptr)
    {
        printf("[PrintRequestAck] fail : msg == nullptr\n");
        return;
    }
    printf("[PrintRequestAck] start\n");
    //print

    printf("\theader.message_type : 0x%04X\n", msg->header.message_type);
    printf("\theader.sequence : %d\n", msg->header.sequence);
    printf("\theader.payload_length : %d\n", msg->header.payload_length);
    printf("\theader.device_type : 0x%02X\n", msg->header.device_type);
    printf("\theader.device_id : 0x%02X 0x%02X 0x%02x\n", msg->header.device_id[0], msg->header.device_id[1], msg->header.device_id[2]);
    printf("\n");
    printf("\tmission_id : %d\n", msg->mission_id);
    printf("\trequest : %d\n", msg->request);
    printf("\tresponse : %d\n", msg->response);
    printf("\tdescription : %s\n", msg->description);
    printf("\ttemporary : %s\n", msg->temporary);
    printf("\tend_point : 0x%04X\n", msg->end_point);
    
}
void PrintItemAck(Item_Ack *msg)
{
    if(!DISPLAY_PACKET)
    {
        return;
    }
    if(msg == nullptr)
    {
        printf("[PrintItemAck] fail : msg == nullptr\n");
        return;
    }
    printf("[PrintItemAck] start\n");
    //print

    printf("\theader.message_type : 0x%04X\n", msg->header.message_type);
    printf("\theader.sequence : %d\n", msg->header.sequence);
    printf("\theader.payload_length : %d\n", msg->header.payload_length);
    printf("\theader.device_type : 0x%02X\n", msg->header.device_type);
    printf("\theader.device_id : 0x%02X 0x%02X 0x%02x\n", msg->header.device_id[0], msg->header.device_id[1], msg->header.device_id[2]);
    printf("\n");
    printf("\tmission_id : %d\n", msg->mission_id);
    printf("\trequest : %d\n", msg->request);
    printf("\tresponse : %d\n", msg->response);
    printf("\tdescription : %s\n", msg->description);
    printf("\ttemporary : %s\n", msg->temporary);
    printf("\tend_point : 0x%04X\n", msg->end_point);

}
