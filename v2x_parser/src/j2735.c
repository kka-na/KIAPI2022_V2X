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
#include <typeinfo>

int* parse_msg = new int[2] {100, 100};

void print_hex(char *data, int len)
{
    // printf("HEX[%d] : ", len);
    for (int i = 0; i < len; i++)
    {
        // printf("%02X", (data[i] & 0xFF));
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

int *decode_j2735_uper(MessageFrame_t *dst, char *src, int size, unsigned long curLaneID)
{

    int res = -1;
    MessageFrame_t *ptrMsg = NULL;

    asn_dec_rval_t ret = uper_decode(NULL,
                                     &asn_DEF_MessageFrame,
                                     (void **)&dst,
                                     src, size, 0, 0);
    // if (ret.code != RC_OK)
        // return res;

    res = ret.consumed;
    // asn_fprint(stdout,&asn_DEF_MessageFrame,dst);
    parse_decoded_j2735(dst, curLaneID);
    return parse_msg;
}

int parse_decoded_j2735(MessageFrame_t *msg, unsigned long curLaneID)
{

    if (msg->messageId == DSRC_ID_SPAT)
    {
        SPAT_t *ptrSpat = &msg->value.choice.SPAT;
        parse_spat(ptrSpat, curLaneID);

    }

    if (msg->messageId == DSRC_ID_MAP)
    {
        MapData_t *ptrMapData = &msg->value.choice.MapData;
        // parse_map(ptrMapData);
    }
    return 0;
}

int parse_spat(SPAT_t *spat, unsigned long curLaneID)
{
    IntersectionState_t intersection = *spat->intersections.list.array[0];
    char *name = (char *)intersection.name->buf;
    int id_id = intersection.id.id;
    char _status[50];
    sprintf(_status, "%x", intersection.status.buf);

    printf(" %s\n", name);

    for (int j = 0; j < intersection.states.list.count; j++)
    {
        MovementState_t movement = *intersection.states.list.array[j];
        char *movementName = (char *)movement.movementName->buf;
        int signalGroup = movement.signalGroup;
        int state_time_speed_eventState = movement.state_time_speed.list.array[0]->eventState;
        int state_time_speed_timing_minEndTime = movement.state_time_speed.list.array[0]->timing->minEndTime;

        char *status_name;

        if(signalGroup == 13 && (((curLaneID == 60) && (id_id == 1)) || ((curLaneID == 40) && (id_id == 2)) || ((curLaneID) == 25 && (id_id == 3))))
        {
            parse_msg[0] = state_time_speed_eventState;
            parse_msg[1] = state_time_speed_timing_minEndTime;
            break;
        }
        if(signalGroup == 16 && (((curLaneID == 57) && (id_id == 1)) || ((curLaneID == 75) && (id_id == 1)) || ((curLaneID == 80) && (id_id == 2)) || ((curLaneID == 79) && (id_id == 2)) || ((curLaneID == 31) && (id_id == 3)) || ((curLaneID == 82) && (id_id == 3))))
        {
            parse_msg[0] = state_time_speed_eventState;
            parse_msg[1] = state_time_speed_timing_minEndTime;
            break;
        }
        if (signalGroup == 19 && (((curLaneID == 59) && (id_id == 1)) || ((curLaneID == 77) && (id_id == 2)) || ((curLaneID == 73 && id_id == 3))))
        {
            parse_msg[0] = state_time_speed_eventState;
            parse_msg[1] = state_time_speed_timing_minEndTime;
            break;
        }
        if (signalGroup == 22 && (((curLaneID == 78) && (id_id == 1)) || ((curLaneID == 39) && (id_id == 1)) || ((curLaneID == 81 && id_id == 2)) || ((curLaneID == 24) && (id_id == 2))))
        {
            parse_msg[0] = state_time_speed_eventState;
            parse_msg[1] = state_time_speed_timing_minEndTime;
            // printf(" %s, %d, %d, %d\n", movementName, signalGroup, state_time_speed_eventState, state_time_speed_timing_minEndTime);
            break;
        }
        
    }

    return 0;
}
