#ifndef SETSTAGE1_H
#define SETSTAGE1_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <netinet/tcp.h>
using namespace std;

#include "obu_packet.h"

#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"

#define MISSION_ID 2
#define DISPLAY_PACKET false

class SetStage1
{
public:
    int RecvMissionStage1(unsigned char *);
    int RecvRequestAck(unsigned char *);
    void SetROS(ros::NodeHandle);

    int clnt_sock = -1;

private:
    void SendRequest(unsigned char, unsigned char);
    void ParseMissionStage1(MissionListStage1 *, unsigned char *);
    void ParseRequestAck(Request_Ack *, unsigned char *);
    void PrintMissionStage1(MissionListStage1 *);
    void PrintRequestAck(Request_Ack *);
    void init();

    // ROS
    void PublishMissionStage1(MissionListStage1 *);
    void PublishStage1State();
    void arriveInfoCallback(const std_msgs::Int16MultiArray::ConstPtr &msg);

private:
    ros::Publisher mission_stage1_pub;
    ros::Publisher stage1_state_pub;
    ros::Subscriber arrive_info_sub;
    int seq = 0;
    int stage1_state[4] = {0, 0, 0, 0};
    int arrive_info[2] = {0, 0};

    int clear_cnt = 0;
};

#endif
