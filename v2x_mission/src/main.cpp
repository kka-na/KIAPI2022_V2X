
#include <iostream>

#include "setSocket.h"
#include "setStage1.h"
#include "setStage2.h"
#include "obu_packet.h"

#include "ros/ros.h"

#define MAX_READ_LEN 4096
#define MAX_BUFFER_LEN 1000000 

int main(int argc, char **argv)
{
    SetSocket ss;
    SetStage1 stg1;
    SetStage2 stg2;

    int sock_connect_state = -1;
    string buf;

    ros::init(argc, argv, "mission");
    ros::NodeHandle n;
    ros::Rate loop_rate(10);

    stg1.SetROS(n);
    stg2.SetROS(n);

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
                break;

            unsigned char *temp_buf = (unsigned char *)buf.c_str();
            MsgHeader *header = (MsgHeader *)temp_buf;

            // printf("type checking ! %s",header->message_type);
            if (buf.size() < sizeof(MsgHeader) + header->payload_length)
            {
                printf("pay size error! buffering... %ld / %ld\n", buf.size(), sizeof(MsgHeader) + header->payload_length);
                break;
            }

            if (header->message_type == MessageType::MISSION_LIST_STAGE1)
            {
                stg1.RecvMissionStage1(temp_buf);
            }
            else if (header->message_type == MessageType::MISSION_LIST_STAGE2)
            {
                stg2.RecvMissionStage2(temp_buf);
            }
            else if (header->message_type == MessageType::REQUEST_ACK || header->message_type == MessageType::NODE_ACK)
            {
                stg1.RecvRequestAck(temp_buf);
            }
            else if (header->message_type == MessageType::ITEM_ACK)
            {
                stg2.RecvItemAck(temp_buf);
            }
            else
            {
                printf("Recv unkown message\n");
                buf.clear();
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
