#include "setStage1.h"

static void SetStage1::arriveInfoCallback(const std_msgs::Int16MultiArray::ConstPtr &msg)
{
    // std_msgs/Int16MultiArray/data
    //  data[0] : Departure, if arrived == 1
    //  data[1] : Destination, if arrived == 1
    arrive_info[0] = msg->data[0];
    arrive_info[1] = msg->data[1];
}

void SetStage1::SetROS(ros::NodeHandle n)
{
    mission_stage1_pub = n.advertise<geometry_msgs::PoseArray>("/mission_stage1", 1000);
    stage1_state_pub = n.advertise<std_msgs::Int16MultiArray>("/stage1_state", 1000);
    ros::Subscriber arrive_info_sub = n.subscribe("/arrive_info", 100, arriveInfoCallback);
}

int SetStage1::RecvMissionStage1(unsigned char *buf)
{
    // printf("\n[RecvMissionStage1] start\n");
    MissionListStage1 msg = {
        0,
    };

    ParseMissionStage1(&msg, buf);
    PrintMissionStage1(&msg);
    PublishMissionStage1(&msg);
    PublishStage1State(stage1_state);

    unsigned char mission_id = msg.mission_list[MISSION_ID].mission_id;
    // WAITING
    if (msg.mission_status == 0x00)
    {
        //[IF] selection available, mission_list[2] : Hard
        if (msg.mission_list[MISSION_ID].status == 0x00)
        {
            SendRequest(mission_id, RequestType::REQ_SELECT_MISSION);
        }
    }
    // IN PROGRESS
    else if (msg.mission_status == 0x01)
    {
        //[IF] Mission Selection was Accepted,
        if (stage1_state[0])
        {
            cout << "Go to Departure !" << endl;
            if (SubscribeArriveInfo(1))
            {
                cout << "Arrived at Deaparture" << endl;
                SendRequest(mission_id, RequestType::REQ_START_POSITION);
            }
        }
        //[IF] Arrive at Departure Position,
        else if (stage1_state[1])
        {
            cout << "Go to Destination !" << endl;
            if (SubscribeArriveInfo(2))
            {
                cout << "Arrived at Destination" << endl;
                SendRequest(mission_id, RequestType::REQ_END_POSITION);
            }
        }
    }
    // PAUSE
    else if (msg.mission_status == 0x02)
    {
        cout << "MISSION PAUSE" << endl;
    }
    // FINISH
    else if (msg.mission_status == 0x03)
    {
        //[IF] Arrive at Destination Position,
        if (stage1_state[2])
        {
            cout << "Stage1 Clear!" << endl;
        }
    }
    // TEST
    else
    {
        //[IF] selection available, mission_list[2] : Hard
        if (msg.mission_list[MISSION_ID].status == 0x00)
        {
            cout << "Select Mission !" << endl;
            SendRequest(mission_id, RequestType::REQ_SELECT_MISSION);
        }
        // [IF] Mission Selection was Accepted,
        else if (stage1_state[0])
        {
            cout << "Go to Departure !" << endl;
            // MOVE
            if (SubscribeArriveInfo(1))
            {
                cout << "Arrived at Deaparture" << endl;
                SendRequest(mission_id, RequestType::REQ_START_POSITION);
            }
        }
        // [IF] Arrive at Departure Position,
        else if (stage1_state[1])
        {
            cout << "Go to Destination !" << endl;
            // MOVE
            if (SubscribeArriveInfo(2))
            {
                cout << "Arrived at Destination" << endl;
                SendRequest(mission_id, RequestType::REQ_END_POSITION);
            }
        }
        //[IF] Arrive at Destination Position,
        else if (stage1_state[2])
        {
            cout << "Stage1 Clear!" << endl;
        }
    }

    if (msg.mission_list != nullptr)
        delete msg.mission_list;
    if (msg.mission_route_list != nullptr)
        delete msg.mission_route_list;

    return 0;
}

// FOR STAGE 1
int SetStage1::RecvRequestAck(unsigned char *buf)
{
    Request_Ack msg = {
        0,
    };

    ParseRequestAck(&msg, buf);
    // PrintRequestAck(&msg);

    stage1_state[0] = (msg.request == RequestType::REQ_SELECT_MISSION && msg.response == ResponseType::RES_SUCCESS) ? 1 : 0;
    stage1_state[1] = (msg.request == RequestType::REQ_START_POSITION && msg.response == ResponseType::RES_SUCCESS) ? 1 : 0;
    stage1_state[2] = (msg.request == RequestType::REQ_END_POSITION && msg.response == ResponseType::RES_SUCCESS) ? 1 : 0;

    return 0;
}

// FOR STAGE 1
void SetStage1::ParseMissionStage1(MissionListStage1 *msg, unsigned char *buf)
{
    if (msg == nullptr)
    {
        printf("[ParseMissionStage1] fail : msg == nullptr\n");
        return;
    }

    size_t copy_len_1 = sizeof(MissionListStage1) - sizeof(MissionListStage1::mission_list) - sizeof(MissionListStage1::mission_route_list);
    memmove(msg, buf, copy_len_1);

    size_t copy_len_2 = sizeof(MissionData) * msg->mission_count;
    msg->mission_list = new MissionData[msg->mission_count];
    memmove(msg->mission_list, &buf[copy_len_1], copy_len_2);

    size_t copy_len_3 = sizeof(MissionRouteData) * msg->mission_route_count;
    msg->mission_route_list = new MissionRouteData[msg->mission_route_count];
    memmove(msg->mission_route_list, &buf[copy_len_1 + copy_len_2], copy_len_3);
}

void SetStage1::ParseRequestAck(Request_Ack *msg, unsigned char *buf)
{
    if (msg == nullptr)
    {
        printf("[ParseRequestAck] fail : msg == nullptr\n");
        return;
    }
    memmove(msg, buf, sizeof(Request_Ack));
}

void SetStage1::SendRequest(unsigned char id, unsigned char req)
{
    Request msg = {
        0,
    };

    msg.header.message_type = MessageType::REQUEST;
    msg.header.sequence = seq++;
    msg.header.payload_length = sizeof(Request) - sizeof(MsgHeader);
    msg.header.device_type = 0xCE;
    msg.header.device_id[0] = 0x01; // team id
    msg.header.device_id[1] = 0x02; // team id
    msg.header.device_id[2] = 0x03; // team id

    msg.mission_id = id;
    msg.request = req;

    msg.response = 0x00;
    sprintf(msg.description, "IHU"); // team name
    // msg.temporary;                      // all 0
    msg.end_point = 0x0D0A;

    int len = send(clnt_sock, (char *)&msg, sizeof(msg), 0);
    if (len <= 0)
        printf("[SendRequest] fail send\n");
    else
        printf("[SendRequest] id : %d, req : %d, byte : %d\n", id, req, len);
}

struct by_index
{
    bool operator()(MissionRouteData &a, MissionRouteData &b) const
    {
        return a.route_node_index < b.route_node_index;
    }
};

void SetStage1::PublishMissionStage1(MissionListStage1 *msg)
{
    if (msg == nullptr)
    {
        printf("[PublishMissionStage1] fail : msg == nullptr\n");
        return;
    }

    geometry_msgs::PoseArray pose_array;
    // geometry_msgs/PoseArray/poses/
    // position.x : prediction distance
    // orientation.x : index
    // orientation.y : latitude
    // orientation.z : longitude
    // orientation.w : type

    int route_data_count = 2;
    vector<MissionRouteData> mission_route_data;

    for (int i = 0; i < msg->mission_route_count; i++)
    {
        if (msg->mission_route_list[i].mission_route_id == msg->mission_list[MISSION_ID].route_id)
        {
            route_data_count = int(msg->mission_route_list[i].route_node_total_count);
            mission_route_data.push_back(msg->mission_route_list[i]);
        }
    }
    // Sorting by INDEX
    if (mission_route_data.size() != 0)
        sort(mission_route_data.begin(), mission_route_data.end(), by_index());

    pose_array.poses.resize(route_data_count);
    for (int i = 0; i < route_data_count; i++)
    {
        geometry_msgs::Pose pose;
        pose.position.x = int(msg->mission_list[MISSION_ID].distance);
        pose.orientation.x = int(mission_route_data[i].route_node_index);
        pose.orientation.y = float(float(mission_route_data[i].route_node_pos_lat) / 10000000.0);
        pose.orientation.z = float(float(mission_route_data[i].route_node_pos_lon) / 10000000.0);
        //[TYPE] 0: NODE, 1 : DEPARTURE, 2 : DESTINATION
        pose.orientation.w = int(mission_route_data[i].route_node_type);
        pose_array.poses[i] = pose;
    }
    mission_stage1_pub.publish(pose_array);
}

void SetStage1::PublishStage1State(int *state)
{
    std_msgs::Int16MultiArray arr;
    // std_msgs/Int16MultiArray/data
    //  data[0] : Selection Accepted
    //  data[1] : Departure Accepted
    //  data[2] : Destination Accepted
    arr.data.resize(3);
    for (int i = 0; i < 3; i++)
    {
        arr.data[i] = state[i];
    }
    stage1_state_pub.publish(arr);
}

bool SetStage1::SubscribeArriveInfo(int type)
{
    return arrive_info[type - 1];
}

void SetStage1::PrintMissionStage1(MissionListStage1 *msg)
{
    if (!DISPLAY_PACKET)
        return;
    if (msg == nullptr)
    {
        printf("[PrintMissionStage1] fail : msg == nullptr\n");
        return;
    }

    printf("\theader.message_type : 0x%04X\n", msg->header.message_type);
    printf("\theader.sequence : %d\n", msg->header.sequence);
    printf("\theader.payload_length : %d\n", msg->header.payload_length);
    printf("\theader.device_type : 0x%02X\n", msg->header.device_type);
    printf("\theader.device_id : 0x%02X 0x%02X 0x%02x\n", msg->header.device_id[0], msg->header.device_id[1], msg->header.device_id[2]);
    printf("\n");
    printf("\tmission_status : 0x%02X\n", msg->mission_status);
    printf("\tmission_count : %d\n", msg->mission_count);
    printf("\tmission_route_count : %d\n", msg->mission_route_count);

    for (int i = 0; i < msg->mission_count; i++)
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
    for (int i = 0; i < msg->mission_route_count; i++)
    {
        printf("\tmission_route_list[%d].mission_route_id : %d\n", i, msg->mission_route_list[i].mission_route_id);
        printf("\tmission_route_list[%d].route_node_total_count : %d\n", i, msg->mission_route_list[i].route_node_total_count);
        printf("\tmission_route_list[%d].route_node_index : %d\n", i, msg->mission_route_list[i].route_node_index);
        printf("\tmission_route_list[%d].route_node_type : %d\n", i, msg->mission_route_list[i].route_node_type);
        printf("\tmission_route_list[%d].route_node_pos_lat : %d\n", i, msg->mission_route_list[i].route_node_pos_lat);
        printf("\tmission_route_list[%d].route_node_pos_lon : %d\n", i, msg->mission_route_list[i].route_node_pos_lon);
    }
}

void SetStage1::PrintRequestAck(Request_Ack *msg)
{
    if (!DISPLAY_PACKET)
        return;
    if (msg == nullptr)
    {
        printf("[PrintRequestAck] fail : msg == nullptr\n");
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