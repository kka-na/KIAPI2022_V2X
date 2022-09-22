#ifndef SETSTAGE2_H
#define SETSTAGE2_H

#include <iostream>
using namespace std;

#include "obu_packet.h"

#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"

#define DISPLAY_PACKET false

class SetStage2
{
public:
    int RecvMissionStage2(unsigned char *);
    int RecvItemAck(unsigned char *);
    void SetROS(ros::NodeHandle);

private:
    void ParseMissionStage2(MissionListStage2 *, unsigned char *);
    void ParseItemAck(Item_Ack *, unsigned char *);
    void PrintMissionStage2(MissionListStage2 *);
    void PrintItemAck(Item_Ack *);
};

#endif
