#include "setStage2.h"

void SetStage2::SetROS(ros::NodeHandle n)
{
    mission_stage2_pub = n.advertise<geometry_msgs::PoseArray>("/stage2_mission", 1000);
    stage2_state_pub = n.advertise<std_msgs::Int16MultiArray>("/stage2_state", 1000);
}

void SetStage2::init()
{
    cout << "Initialize" << endl;
    stage2_state_set_cnt = 0;
    delete stage2_state;
}

bool check(vector<int> array, int d)
{
    for (int &i : array)
    {
        if (i != d)
            return false;
    }
    return true;
}

int SetStage2::RecvMissionStage2(unsigned char *buf)
{
    MissionListStage2 msg = {
        0,
    };

    ParseMissionStage2(&msg, buf);
    PrintMissionStage2(&msg);
    PublishMissionStage2(&msg);

    vector<int> check_arr;

    if (stage2_state_set_cnt == 0)
    {
        SetStage2State(&msg);
        stage2_state_set_cnt = -1;
    }
    else
    {
        stage2_state[0] = int(msg.mission_status);
        if (msg.mission_status == 0x03)
        {
            // cout << "Stage2 Clear!" << endl;
            clear_cnt += 1;
            if (clear_cnt > 5)
            {
                clear_cnt = 0;
                init();
            }
        }
        else
        {
            for (int i = 1; i < item_count; i++)
            {
                check_arr.push_back(stage2_state[i]);
            }
            if (check(check_arr, 1))
            {
                // cout << "Stage2 Clear!" << endl;
                clear_cnt += 1;
                if (clear_cnt > 5)
                {
                    clear_cnt = 0;
                    init();
                }
            }
        }
        PublishStage2State();
    }
    if (msg.item_list != nullptr)
        delete msg.item_list;
    return 0;
}

int SetStage2::RecvItemAck(unsigned char *buf)
{
    Item_Ack msg = {
        0,
    };

    ParseItemAck(&msg, buf);
    PrintItemAck(&msg);

    if (msg.request == RequestType::REQ_ITEM)
    {
        if (msg.response == ItemType::ITEM_LAP_TIME_SUB || msg.response == ItemType::ITEM_LAP_TIME_ADD || msg.response == ItemType::ITEM_BOOST)
        {
            stage2_state[msg.mission_id] = 1;
        }
    }

    return 0;
}

struct by_id
{
    bool operator()(ItemData &a, ItemData &b)
    {
        return a.item_id < b.item_id;
    }
};
void SetStage2::PublishMissionStage2(MissionListStage2 *msg)
{
    if (msg == nullptr)
    {
        printf("[PublishMissionStage2] fail : msg == nullptr\n");
        return;
    }

    geometry_msgs::PoseArray pose_array;
    // geometry_msgs/PoseArray/poses/
    // position.x : item id
    // position.y : speed or score
    // position.z : duration
    // orientation.x : type
    // orientation.y : extend
    // orientation.z : latitude
    // orientation.w : longitude

    vector<ItemData> item_data;
    for (int i = 0; i < item_count; i++)
        item_data.push_back(msg->item_list[i]);
    sort(item_data.begin(), item_data.end(), by_id());

    pose_array.poses.resize(item_count);
    for (int i = 0; i < item_count; i++)
    {
        geometry_msgs::Pose pose;
        pose.position.x = int(item_data[i].item_id);
        if (!item_data[i].item_type == 3)
            pose.position.y = int(item_data[i].score);
        else
            pose.position.y = int(item_data[i].speed); //<0 minus socre, >0 plus score

        pose.position.z = int(item_data[i].duration);
        //[TYPE] 1: MINUS, 2:PLUS, 3:BOOSTER
        pose.orientation.x = int(item_data[i].item_type);
        pose.orientation.y = int(item_data[i].extend);
        pose.orientation.z = float(float(item_data[i].pos_lat) / 10000000.0);
        pose.orientation.w = float(float(item_data[i].pos_long) / 10000000.0);

        pose_array.poses[i] = pose;
    }
    mission_stage2_pub.publish(pose_array);
}

void SetStage2::SetStage2State(MissionListStage2 *msg)
{
    item_count = int(msg->item_count);
    stage2_state = new int[item_count + 1]{0};
}
void SetStage2::PublishStage2State()
{
    std_msgs::Int16MultiArray arr;
    // std_msgs/Int16MultiArray/data
    //  data[0] : Mission Status
    //  data[item_id] : Accepted
    arr.data.resize(item_count + 1);
    for (int i = 0; i < item_count + 1; i++)
        arr.data[i] = stage2_state[i];
    stage2_state_pub.publish(arr);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void SetStage2::ParseMissionStage2(MissionListStage2 *msg, unsigned char *buf)
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

void SetStage2::ParseItemAck(Item_Ack *msg, unsigned char *buf)
{
    if (msg == nullptr)
    {
        printf("[ParseItemAck] fail : msg == nullptr\n");
        return;
    }
    memmove(msg, buf, sizeof(Item_Ack));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void SetStage2::PrintMissionStage2(MissionListStage2 *msg)
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

void SetStage2::PrintItemAck(Item_Ack *msg)
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

SetStage2::~SetStage2()
{
    delete stage2_state;
}