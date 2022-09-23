#ifndef SETSTAGE2_H
#define SETSTAGE2_H

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "obu_packet.h"

#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"

#define DISPLAY_PACKET false

class SetStage2
{
public:
    int RecvMissionStage2(unsigned char *);
    int RecvItemAck(unsigned char *);
    void SetROS(ros::NodeHandle);
    ~SetStage2();

private:
    void ParseMissionStage2(MissionListStage2 *, unsigned char *);
    void ParseItemAck(Item_Ack *, unsigned char *);
    void PrintMissionStage2(MissionListStage2 *);
    void PrintItemAck(Item_Ack *);
    // ROS
    void PublishMissionStage2(MissionListStage2 *);
    void SetStage2State(MissionListStage2 *);
    void PublishStage2State();

private:
    ros::Publisher stage2_state_pub;
    ros::Publisher mission_stage2_pub;

    int *stage2_state;
    int stage2_state_set_cnt = 0;
    int item_count = 0;
};

#endif
