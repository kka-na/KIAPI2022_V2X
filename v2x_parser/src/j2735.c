/*
 * UPER Based client application sample code
 *
 * Copyright (c) 2022, CEST
 *
 */

#include "j2735.h"
#include "string.h"
#include "sys/time.h"
#include "time.h"
#include <stdbool.h>
#include <json-c/json.h>

#define LOGGING_SPAT false

void print_hex(char *data, int len)
{
    printf("HEX[%d] : ", len);
    for (int i = 0; i < len; i++)
    {
        printf("%02X", (data[i] & 0xFF));
        continue;
    }
    printf("\n");
}

int encode_j2735_uper(char *dst, unsigned short dstLen, MessageFrame_t *src)
{
    int res = -1;

    asn_enc_rval_t ret = uper_encode_to_buffer(&asn_DEF_MessageFrame,
                                               NULL,
                                               src,
                                               dst, dstLen);

    if (ret.encoded > 0)
        return ret.encoded; //  UPER Encoding Success
    else
    {
        if (ret.failed_type != NULL)
            printf("encoded error value name = %s\n", ret.failed_type->name);

        return -1; // UPER Encoding failed
    }
}

int decode_j2735_uper(MessageFrame_t *dst, char *src, int size)
{

    int res = -1;

    MessageFrame_t *ptrMsg = NULL;

    asn_dec_rval_t ret = uper_decode(NULL,
                                     &asn_DEF_MessageFrame,
                                     (void **)&dst,
                                     src, size, 0, 0);
    if (ret.code != RC_OK)
        return res;

    res = ret.consumed;
    // asn_fprint(stdout,&asn_DEF_MessageFrame,dst);
    parse_decoded_j2735(dst);
    return res;
}

int parse_decoded_j2735(MessageFrame_t *msg)
{
    printf("Parse decode j2735\n");
    if (msg->messageId == DSRC_ID_SPAT)
    {
        SPAT_t *ptrSpat = &msg->value.choice.SPAT;
        parse_spat(ptrSpat);
    }
    else if (msg->messageId == DSRC_ID_MAP)
    {
        MapData_t *ptrMapData = &msg->value.choice.MapData;
        parse_map(ptrMapData);
    }
    return 0;
}

void parse_spat(SPAT_t *spat)
{
    IntersectionState_t intersection = *spat->intersections.list.array[0];
    char *name = (char *)intersection.name->buf;
    int id_region = intersection.id.region[0];
    int id_id = intersection.id.id;
    int revision = intersection.revision;
    char _status[50];
    sprintf(_status, "%x", intersection.status.buf);
    int moy = intersection.moy[0];
    int timestamp = intersection.timeStamp[0];

    // time
    time_t timer;
    struct tm *t;
    timer = time(NULL);
    t = localtime(&timer);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char filename_spat[512]; // = "/home/kana/Documents/Project/KIAPI/v2x_parse/json/spat.json";
    long sec_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    sprintf(filename_spat, "/home/kana/catkin_ws/src/KIAPI_INHA/v2x_parser/log/spat/spat_%s_%ld.json", name, sec_in_mill);
    json_object *SPAT = json_object_new_object();

    // printf("%s, %d, %d, %d, %s, %d, %d\n", name, id_region, id_id, revision, _status, moy, timestamp);

    json_object_object_add(SPAT, "name", json_object_new_string(name));
    json_object *id = json_object_new_object();
    json_object_object_add(id, "region", json_object_new_int(id_region));
    json_object_object_add(id, "id", json_object_new_int(id_id));
    json_object_object_add(SPAT, "id", json_object_get(id));
    json_object_object_add(SPAT, "revision", json_object_new_int(revision));
    json_object_object_add(SPAT, "status", json_object_new_string(_status));
    json_object_object_add(SPAT, "moy", json_object_new_int(moy));
    json_object_object_add(SPAT, "timestamp", json_object_new_int(timestamp));

    for (int j = 0; j < intersection.states.list.count; j++)
    {
        MovementState_t movement = *intersection.states.list.array[j];
        char *movementName = (char *)movement.movementName->buf;
        int signalGroup = movement.signalGroup;
        int state_time_speed_eventState = movement.state_time_speed.list.array[0]->eventState;
        int state_time_speed_timing_minEndTime = movement.state_time_speed.list.array[0]->timing->minEndTime;
        int maneuverAssistList_connectionID = movement.maneuverAssistList->list.array[0]->connectionID;
        bool maneuverAssistList_pedBicycleDetect_bool = movement.maneuverAssistList->list.array[0]->pedBicycleDetect;

        char *status_name;
        sprintf(status_name, "states%d", j);

        json_object *status = json_object_new_object();
        json_object_object_add(status, "movementName", json_object_new_string(movementName));
        json_object_object_add(status, "signalGroup", json_object_new_int(signalGroup));
        json_object *state_time_speed = json_object_new_object();
        json_object_object_add(state_time_speed, "eventState", json_object_new_int(state_time_speed_eventState));
        json_object_object_add(state_time_speed, "timing_minEndTime", json_object_new_int(state_time_speed_timing_minEndTime));
        json_object_object_add(status, "state-time-speed", json_object_get(state_time_speed));
        json_object *maneuverAssistList = json_object_new_object();
        json_object_object_add(maneuverAssistList, "connectionID", json_object_new_int(maneuverAssistList_connectionID));
        json_object_object_add(maneuverAssistList, "pedBicycleDetect", json_object_new_boolean(maneuverAssistList_pedBicycleDetect_bool));
        json_object_object_add(status, "maneuverAssistList", json_object_get(maneuverAssistList));
        json_object_object_add(SPAT, status_name, json_object_get(status));

        // printf("%s, %s, %d, %d, %d, %d, %s\n", status_name, movementName, signalGroup, state_time_speed_eventState, state_time_speed_timing_minEndTime, maneuverAssistList_connectionID, maneuverAssistList_pedBicycleDetect_bool ? "True" : "False");
    }
    if (LOGGING_SPAT)
    {
        json_object_to_file_ext(filename_spat, json_object_get(SPAT), JSON_C_TO_STRING_PRETTY);

        json_object_put(SPAT);

        printf("SPAT Logged\n");
    }
}

int parse_map(MapData_t *map)
{
    for (int i = 0; i < map->intersections->list.count; i++)
    {
        struct IntersectionGeometry *intersection = map->intersections->list.array[i];

        for (int j = 0; j < intersection->laneSet.list.count; j++)
        {
            struct GenericLane *lane = intersection->laneSet.list.array[j];
            for (int k = 0; k < lane->nodeList.choice.nodes.list.count; k++)
            {
                struct NodeXY *node = lane->nodeList.choice.nodes.list.array[k];
                int offset1_x = node->delta.choice.node_XY1.x;
                int offset1_y = node->delta.choice.node_XY1.y;
                printf("Offset1 x : %d, y : %d \n", offset1_x, offset1_x);
            }
        }
    }

    return 0;
}
