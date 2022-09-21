
#include <iostream>

#include "setSocket.h"
#include "setStage1.h"
#include "obu_packet.h"

#include "ros/ros.h"

#define DISPLAY_PACKET false
#define MAX_READ_LEN 4096
#define MAX_BUFFER_LEN 1000000

// FOR STAGE2
int RecvMissionStage2(unsigned char *);
int RecvItemAck(unsigned char *);
void ParseMissionStage2(MissionListStage2 *, unsigned char *);
void ParseItemAck(Item_Ack *, unsigned char *);
void PrintMissionStage2(MissionListStage2 *);
void PrintItemAck(Item_Ack *);

int main(int argc, char **argv)
{
    SetSocket ss;
    SetStage1 stg1;

    int sock_connect_state = -1;
    string buf;

    ros::init(argc, argv, "listener");
    ros::NodeHandle n;

    stg1.SetROS(n);

    ros::Rate loop_rate(10);

    while (ros::ok())
    {
        if (sock_connect_state == -1)
        {
            ss.Disconnect_socket();
            sock_connect_state = ss.Connect_socket();
            buf.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

        unsigned char recvbuf[MAX_READ_LEN] = {
            0,
        };
        int len = recv(ss.clnt_sock, recvbuf, sizeof(recvbuf), 0); // blocking function!

        if (len <= 0)
        {
            sock_connect_state = ss.Disconnect_socket();
            continue;
        }

        if (len <= MAX_READ_LEN)
        {
            if (buf.size() > MAX_BUFFER_LEN)
            {
                buf.clear();
            }
            buf.append((char *)recvbuf, len);
        }

        stg1.clnt_sock = ss.clnt_sock;

        // Keeping Connection
        while (ros::ok())
        {
            if (buf.size() < sizeof(MsgHeader))
            {
                break;
            }
            unsigned char *temp_buf = (unsigned char *)buf.c_str();
            MsgHeader *header = (MsgHeader *)temp_buf;

            if (buf.size() < sizeof(MsgHeader) + header->payload_length)
            {
                printf("pay size error! buffering... %ld / %ld\n", buf.size(), sizeof(MsgHeader) + header->payload_length);
                break;
            }

            switch (header->message_type)
            {
            case MessageType::MISSION_LIST_STAGE1:
            {
                stg1.RecvMissionStage1(temp_buf);
            }
            break;
            case MessageType::MISSION_LIST_STAGE2:
            {
                // RecvMissionStage2(temp_buf);
            }
            break;
            case MessageType::REQUEST_ACK:
            {
                stg1.RecvRequestAck(temp_buf);
            }
            break;
            case MessageType::ITEM_ACK:
            {
                // RecvItemAck(temp_buf);
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
            ros::spinOnce();
            loop_rate.sleep();
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    ss.Disconnect_socket();

    return 0;
}

/* - - 0 0 - 0 0 - 0 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - 0 - 0 - 0 - 0 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - 0 - - - 0 - 0 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// MISSION_ID Setting

// FOR STAGE 1

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// FOR STAGE 2
int RecvMissionStage2(unsigned char *buf)
{
    MissionListStage2 msg = {
        0,
    };

    ParseMissionStage2(&msg, buf);
    PrintMissionStage2(&msg);

    if (msg.item_list != nullptr)
    {
        delete msg.item_list;
    }
    return 0;
}

// FOR STAGE 2
int RecvItemAck(unsigned char *buf)
{
    Item_Ack msg = {
        0,
    };

    ParseItemAck(&msg, buf);
    PrintItemAck(&msg);

    if (msg.request == RequestType::REQ_ITEM)
    {
        if (msg.response == ItemType::ITEM_LAP_TIME_SUB)
        {
            printf("%d번 아이템 획득 : Lap time 감소\n", msg.mission_id);
        }
        if (msg.response == ItemType::ITEM_LAP_TIME_ADD)
        {
            printf("%d번 아이템 획득 : Lap time 증가\n", msg.mission_id);
        }
        if (msg.response == ItemType::ITEM_BOOST)
        {
            printf("%d번 아이템 획득 : 가속 주행\n", msg.mission_id);
        }
    }

    return 0;
}

void ParseMissionStage2(MissionListStage2 *msg, unsigned char *buf)
{
    if (msg == nullptr)
    {
        printf("[ParseMissionStage2] fail : msg == nullptr\n");
        return;
    }

    size_t copy_len_1 = sizeof(MissionListStage2) - sizeof(MissionListStage2::item_list);
    memmove(msg, buf, copy_len_1);

    size_t copy_len_2 = sizeof(ItemData) * msg->item_count;
    msg->item_list = new ItemData[msg->item_count];
    memmove(msg->item_list, &buf[copy_len_1], copy_len_2);
}

void ParseItemAck(Item_Ack *msg, unsigned char *buf)
{
    if (msg == nullptr)
    {
        printf("[ParseItemAck] fail : msg == nullptr\n");
        return;
    }
    memmove(msg, buf, sizeof(Item_Ack));
}

void PrintMissionStage2(MissionListStage2 *msg)
{
    if (!DISPLAY_PACKET)
        return;
    if (msg == nullptr)
    {
        printf("[PrintMissionStage2] fail : msg == nullptr\n");
        return;
    }

    printf("\theader.message_type : 0x%04X\n", msg->header.message_type);
    printf("\theader.sequence : %d\n", msg->header.sequence);
    printf("\theader.payload_length : %d\n", msg->header.payload_length);
    printf("\theader.device_type : 0x%02X\n", msg->header.device_type);
    printf("\theader.device_id : 0x%02X 0x%02X 0x%02x\n", msg->header.device_id[0], msg->header.device_id[1], msg->header.device_id[2]);
    printf("\n");
    printf("\tmission_status : 0x%02X\n", msg->mission_status);
    printf("\titem_count : %d\n", msg->item_count);

    for (int i = 0; i < msg->item_count; i++)
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

void PrintItemAck(Item_Ack *msg)
{
    if (!DISPLAY_PACKET)
        return;

    if (msg == nullptr)
    {
        printf("[PrintItemAck] fail : msg == nullptr\n");
        return;
    }
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
