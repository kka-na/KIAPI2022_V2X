#include "setStage2.h"

void SetStage2::SetROS(ros::NodeHandle n)
{
}

int SetStage2::RecvMissionStage2(unsigned char *buf)
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

int SetStage2::RecvItemAck(unsigned char *buf)
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
