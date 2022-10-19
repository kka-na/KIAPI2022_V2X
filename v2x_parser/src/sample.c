/*
 * UPER Based client application sample code
 *
 * Copyright (c) 2022, CEST
 *
 */
#include <stdio.h>
#include "sample.h"
#include "math.h"
#include <ros/ros.h>
#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Float32MultiArray.h"

#include "sbg_driver/SbgEkfEuler.h"
#include "sbg_driver/SbgGpsPos.h"

void gpsPosCallback(const sbg_driver::SbgGpsPos::ConstPtr &msg)
{
    latitude = msg->latitude * pow(10, 7);
    longitude = msg->longitude * pow(10, 7);
    elevation = msg->altitude * pow(10, 2);
}

void ekfEulerCallback(const sbg_driver::SbgEkfEuler::ConstPtr &msg)
{
    int yaw = msg->angle.z *(180/3.14);
    int temp = (yaw <= 0 && yaw >= -180) ? yaw + 360 : yaw;
    heading = int(temp / 0.0125);
}

void canRecordCallback(const std_msgs::Int16MultiArray::ConstPtr &msg)
{
    velocity = int((msg->data[5])/0.072);
    gear = (int)(msg->data[3]);
}

void curLaneIDCallback(const std_msgs::Int16MultiArray::ConstPtr &msg)
{
    curLaneID = msg->data[0];
}

unsigned long long get_clock_time()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    uint64_t clock = ts.tv_sec * 1000 + (ts.tv_nsec / 1000000);
    return clock;
}

int signalstate(int eventState)
{
    int temp;
    if (eventState == 3)
    {
        temp = 1;   // red
    }
    else if ((eventState == 5) || (eventState == 6))
    {
        temp = 2;   //green
    }
    else if ((eventState == 7) || (eventState == 8))
    {
        temp = 3;   //yellow
    }

    return temp;
} 

int connect_obu_uper_tcp(char *ip, unsigned short port)
{

    int sockFd = -1;

    // 소켓 파일디스크립터 생성
    sockFd = socket(PF_INET, SOCK_STREAM, 0);

    // 소켓 생성이 실패 여부 확인
    if (sockFd < 0)
    {
        printf("DEBUG : step sock create error\n");
        return -1;
    }

    // TCP 서버 주소, 포트 입력
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    // TCP 서버 연결
    if (connect(sockFd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("DEBUG : step sock connect error\n");
        close(sockFd);
        return -1;
    }

    // 논 블록킹 소켓으로 동작
    int flag = fcntl(sockFd, F_GETFL, 0);
    fcntl(sockFd, F_SETFL, flag | O_NONBLOCK);


    return sockFd;
}

int receive_from_obu(int sockFd, char *buffer, unsigned short bufferSize, int storedSize,
                     char *uperBuffer, unsigned short uperBufferSize, int *uperRes)
{
    int rxSize = -1;

    *uperRes = 0;

    if (sockFd < 0)
        return rxSize;

    int leftBufferSize = bufferSize - storedSize;

    if (leftBufferSize <= 0)
    {
        printf("socket receive deadlock\n");
        return -1;
    }

    rxSize = read(sockFd, buffer + storedSize, leftBufferSize);

    // read 리턴값이 0 인 경우, Socket 로부터 Read가 불가능한 상태를 의미하며,
    //대표적으로 소켓 연결이 종료된 상태가 있음
    //리턴값이 -1 인 경우, non-blocking 소켓에 대해 수신된 데이터가 존재하지 않는 경우를 의미
    if (rxSize == 0)
        return -1;

    if (rxSize < 0 && storedSize == 0)
        return 0;

    if (rxSize > 0)
        storedSize += rxSize;

    // CEST OBU TCP 헤더 파싱 부문
    int headerByteLen = sizeof(struct CestObuUperPacketHeader);
    int packetLen = headerByteLen;
    if (storedSize < headerByteLen)
    {
        return storedSize;
    }
    struct CestObuUperPacketHeader header;
    memcpy(&header, buffer, headerByteLen);

    packetLen += header.payloadLen;

    if (storedSize < packetLen)
        return storedSize;

    memcpy(uperBuffer, buffer + headerByteLen, header.payloadLen);
    *uperRes = header.payloadLen;

    storedSize -= packetLen;
    memcpy(buffer, buffer + packetLen, storedSize);

    if (header.messageType == 0x3411)
    {
        struct TxWaveUperResultPayload *payload = (struct TxWaveUperResultPayload *)uperBuffer;
        *uperRes = 0;
    }
    else
        return storedSize;
}

int request_tx_wave_obu(int sockFd, char *uper, unsigned short uperLength)
{

    if (sockFd < 0)
        return -1;

    int packetLen = uperLength + sizeof(struct CestObuUperPacketHeader);

    char packet[OBU_RECEIVE_BUFFER_SIZE]; // tcp header size + uper binary size

    struct CestObuUperPacketHeader *ptrHeader = (struct CestObuUperPacketHeader *)&packet[0];
    ptrHeader->messageType = 0x4311;
    ptrHeader->seq = packetSeq++;
    ptrHeader->payloadLen = uperLength;
    ptrHeader->deviceType = 0xCE;
    memcpy(ptrHeader->deviceId, clientDeviceId, 3);

    memcpy(packet + sizeof(struct CestObuUperPacketHeader), uper, uperLength);

    int res = write(sockFd, packet, packetLen);

    if (res > 0)
    {
        // printf("TX - \"TX_WAVE_UPER\" SEQ[%d] = ",ptrHeader->seq);
        print_hex(uper, uperLength);

        if (res != packetLen)
        {
            printf("DEBUG tcp tx purge\n");
            return -1;
        }
        else
            return res;
    }
}

int tx_v2i_pvd(int sockFd, unsigned long long *time)
{
    unsigned long long interval = get_clock_time() - *time; // msec;

    if (interval < PVD_INTERVAL)
        return 0;

    *time += (interval - interval % PVD_INTERVAL);

    MessageFrame_t msg;
    char uper[MAX_UPER_SIZE];

    fill_j2735_pvd(&msg, latitude, longitude, elevation, heading, velocity, gear);

    int encodedBits = encode_j2735_uper(uper, MAX_UPER_SIZE, &msg);

    if (encodedBits < 0){// 인코딩 실패로 전송이 불가능한 상태
        printf(" pvd failed ! \n");
        return 0;
    }
    int byteLen = encodedBits / 8 + ((encodedBits % 8) ? 1 : 0);

    return request_tx_wave_obu(sockFd, uper, byteLen);
}

int main(int argc, char **argv)
{

    int sockFd = -1; // OBU와 연결하기 위한 TCP Client 소켓 File descriptor
    char rxBuffer[OBU_RECEIVE_BUFFER_SIZE], rxUperBuffer[MAX_UPER_SIZE];
    int storedSize = 0, uperSize;
    unsigned long long txPvd = get_clock_time();

    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    ros::AsyncSpinner spinner(1);

    ros::Subscriber sub_gps_pos = n.subscribe("/sbg/gps_pos", 100, gpsPosCallback);
    ros::Subscriber sub_ekf_euler = n.subscribe("/sbg/ekf_euler", 100, ekfEulerCallback);
    ros::Subscriber sub_can_record = n.subscribe("/can_record", 100, canRecordCallback);
    ros::Subscriber sub_cur_laneid = n.subscribe("/current_LaneID", 100, curLaneIDCallback);

    ros::Publisher pub_spat_msg = n.advertise<std_msgs::Float32MultiArray>("/spat_msg", 100);

    latitude = 0;
    longitude = 0;
    elevation = 0;
    heading = 0;
    velocity = 0;
    gear = 0;
    curLaneID = 61;    
    int a = 0;
    int test;
    spinner.start();
    int *parse_msg;

    while (ros::ok())
    {
        // 소켓이 연결되지 않은 경우(sockFd == -1) , OBU TCP 소켓 연결 시도
        if (sockFd < 0)
        {
            sockFd = connect_obu_uper_tcp("192.168.10.10",23000);    // OBU
            // sockFd = connect_obu_uper_tcp("118.45.183.36", 23000);  // TEST Server
            storedSize = 0;
            if (sockFd < 0)
            {
                printf("DEBUG : connect failed, retry\n");
                sleep(1);
                continue;
            }
        }
        // 소켓이 연결된 상태인 경우, OBU로부터 TCP 패킷 수신
        // WAVE 통신으로 수신된 데이터가 없을 경우, 수신되는 데이터 X

        if (((storedSize = receive_from_obu(sockFd,
                                            rxBuffer, OBU_RECEIVE_BUFFER_SIZE, storedSize,
                                            rxUperBuffer, MAX_UPER_SIZE, &uperSize)) < 0) ||
            (tx_v2i_pvd(sockFd, &txPvd) < 0))
        {
            // OBU와 TCP 연결이 끊어진 경우, 연결 재시도
            printf("Reconnecting\n");
            close(sockFd);
            sockFd = -1;
            continue;
        }

        if (uperSize > 0)
        {

            // OBU로부터 수신된 WAVE 메시지가 존재할 경우, UPER 디코딩 -> J2735 메시지 파싱
            MessageFrame_t *msgFrame = NULL;
            parse_msg = decode_j2735_uper(msgFrame, rxUperBuffer, uperSize, curLaneID);

            std_msgs::Float32MultiArray spat_msg;

            // data[0] = eventState, data[1] = timing_minEndTime
            spat_msg.data.resize(2);
            spat_msg.data[0] = signalstate(parse_msg[0]);
            spat_msg.data[1] = parse_msg[1];
            pub_spat_msg.publish(spat_msg);
            
            int temp;
            if (temp != parse_msg[1])
            {
                printf(" Received V2X Signal-------------------\n\n");
                printf(" Current Signal EventState : %d\n\n", parse_msg[0]);
                printf(" Current Signal timing_minEndTime : %d\n\n",parse_msg[1]);
                temp = parse_msg[1];
            }

            ASN_STRUCT_FREE(asn_DEF_MessageFrame, msgFrame);

        }
   
        usleep(10000); // 1msec sleep
    }
    close(sockFd);

    return 0;
}
