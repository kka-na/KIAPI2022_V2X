/*
 * UPER Based client application sample code
 *
 * Copyright (c) 2022, CEST
 *
 */
#include "j2735.h"
#include "sys/time.h"
#include "time.h"
#include "string.h"
#include <stdio.h>
#include <json-c/json.h>

int fill_j2735_pvd(MessageFrame_t *dst, long _latitude, long _longitude, long _elevation, long _heading, long _velocity, long _gear)
{
    //  time
    time_t timer;
    struct tm *t;
    timer = time(NULL);
    t = localtime(&timer);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char filename_pvd[512];
    long sec_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    sprintf(filename_pvd, "/home/inha/ros_ws/src/KIAPI_INHA/v2x_parser/log/pvd/pvd_%ld.json", sec_in_mill);

    json_object *PVD = json_object_new_object();

    long latitude = _latitude;
    long longitude = _longitude;
    long elevation = _elevation;
    long heading = _heading;
    long velocity = _velocity;
    long gear = _gear;

    // ASN_STRUCT_RESET(asn_DEF_MesssageFrame, dst);

    dst->messageId = 26;                                          // J2735 표준문서 PDF 파일 참조 DE_DSRC_MessageID,  probeVehicleData DSRCmsgID ::= 26 -- PVD
    dst->value.present = MessageFrame__value_PR_ProbeVehicleData; // MessageFrame::value choice (asn1c)

    ProbeVehicleData_t *ptrPvd = &dst->value.choice.ProbeVehicleData;

    ptrPvd->timeStamp = NULL; // OPTIONAL, not to use
    ptrPvd->segNum = NULL;    // OPTIONAL, not to use
    ptrPvd->regional = NULL;  // OPTIONAL, not to use

    ptrPvd->probeID = (VehicleIdent *)malloc(sizeof(struct VehicleIdent));
    ptrPvd->probeID->name = NULL;         // OPTIONAL, not to use
    ptrPvd->probeID->ownerCode = NULL;    // OPTIONAL, not to use
    ptrPvd->probeID->vehicleClass = NULL; // OPTIONAL, not to use
    ptrPvd->probeID->vin = NULL;          // OPTIONAL, not to use
    ptrPvd->probeID->vehicleType = NULL;  // OPTIONAL, not to use
    ptrPvd->probeID->id = (VehicleID *)malloc(sizeof(struct VehicleID));
    ptrPvd->probeID->id->present = VehicleID_PR_entityID;
    ptrPvd->probeID->id->present = VehicleID_PR_entityID;
    ptrPvd->probeID->id->choice.entityID.buf = (unsigned char *)malloc(4);
    ptrPvd->probeID->id->choice.entityID.size = 4;

    ptrPvd->probeID->id->choice.entityID.buf[0] = 0xCE; // (INPUT) <---- 할당된 대학별 ID 입력
    ptrPvd->probeID->id->choice.entityID.buf[1] = 0x24; // (INPUT) <---- 할당된 대학별 ID 입력
    ptrPvd->probeID->id->choice.entityID.buf[2] = 0x67; // (INPUT) <---- 할당된 대학별 ID 입력
    ptrPvd->probeID->id->choice.entityID.buf[3] = 0x06; // (INPUT) <---- 할당된 대학별 ID 입력

    // temp
    char probeID_id_entityID0[] = "0xCE";
    char probeID_id_entityID1[] = "0x20";
    char probeID_id_entityID2[] = "0x67";
    char probeID_id_entityID3[] = "0x06";

    // StartVector : PVD를 전송할 시점을 기준의 시간과 차량의 위치, 이동상태 값을 반영
    ptrPvd->startVector.utcTime = (DDateTime *)malloc(sizeof(struct DDateTime));
    ptrPvd->startVector.utcTime->year = (DYear_t *)malloc(sizeof(DYear_t));
    ptrPvd->startVector.utcTime->month = (DMonth_t *)malloc(sizeof(DMonth_t));
    ptrPvd->startVector.utcTime->day = (DDay_t *)malloc(sizeof(DDay_t));
    ptrPvd->startVector.utcTime->hour = (DHour_t *)malloc(sizeof(DHour_t));
    ptrPvd->startVector.utcTime->minute = (DMinute_t *)malloc(sizeof(DMinute_t));
    ptrPvd->startVector.utcTime->second = (DSecond_t *)malloc(sizeof(DSecond_t));
    ptrPvd->startVector.utcTime->offset = NULL; // OPTIONAL, not to use

    *ptrPvd->startVector.utcTime->year = t->tm_year + 1900; // (INPUT) <--------------- 현재 UTC 시간 입력 (년도)
    *ptrPvd->startVector.utcTime->month = t->tm_mon + 1;    // (INPUT) <--------------- 현재 UTC 시간 입력 (년도)
    *ptrPvd->startVector.utcTime->day = t->tm_mday;         // (INPUT) <--------------- 현재 UTC 시간 입력 (년도)
    *ptrPvd->startVector.utcTime->hour = t->tm_hour;        // (INPUT) <--------------- 현재 UTC 시간 입력 (년도)
    *ptrPvd->startVector.utcTime->minute = t->tm_min;       // (INPUT) <--------------- 현재 UTC 시간 입력 (년도)
    *ptrPvd->startVector.utcTime->second = t->tm_sec;       // (INPUT) <--------------- 현재 UTC 시간 입력 (년도)

    ptrPvd->startVector.elevation = (DSRC_Elevation_t *)malloc(sizeof(DSRC_Elevation_t));
    ptrPvd->startVector.heading = (Heading_t *)malloc(sizeof(Heading_t));
    ptrPvd->startVector.speed = (TransmissionAndSpeed *)malloc(sizeof(struct TransmissionAndSpeed));
    ptrPvd->startVector.posAccuracy = NULL;     // OPTIONAL, not to use
    ptrPvd->startVector.posConfidence = NULL;   // OPTIONAL, not to use
    ptrPvd->startVector.timeConfidence = NULL;  // OPTIONAL, not to use
    ptrPvd->startVector.speedConfidence = NULL; // OPTIONAL, not to use

    ptrPvd->startVector.Long = longitude;          // (INPUT) <--------------- 현재 차량의 위치 (위도) (Longitude, DD 좌표계)
    ptrPvd->startVector.lat = latitude;            // (INPUT) <--------------- 현재 차량의 위치 (경도) (Latitude,  DD 좌표계)
    *ptrPvd->startVector.elevation = elevation;    // (INPUT) <--------------- 현재 차량의 위치 (고도) (Elevation)
    *ptrPvd->startVector.heading = heading;        // (INPUT) <--------------- 현재 차량의 주행 방향 (북쪽 0도)
    ptrPvd->startVector.speed->speed = velocity;   // (INPUT) <--------------- 현재 차량의 속도
    ptrPvd->startVector.speed->transmisson = gear; // (INPUT) <--------------- 현재 차량의 변속기 상태

    ptrPvd->vehicleType.hpmsType = (VehicleType_t *)malloc(sizeof(VehicleType_t));
    ptrPvd->vehicleType.keyType = NULL;       // OPTIONAL, not to use
    ptrPvd->vehicleType.fuelType = NULL;      // OPTIONAL, not to use
    ptrPvd->vehicleType.iso3883 = NULL;       // OPTIONAL, not to use
    ptrPvd->vehicleType.regional = NULL;      // OPTIONAL, not to use
    ptrPvd->vehicleType.responderType = NULL; // OPTIONAL, not to use
    ptrPvd->vehicleType.responseEquip = NULL; // OPTIONAL, not to use
    ptrPvd->vehicleType.role = NULL;          // OPTIONAL, not to use
    ptrPvd->vehicleType.vehicleType = NULL;   // OPTIONAL, not to use
    *ptrPvd->vehicleType.hpmsType = VehicleType_car;

    // PVD 전송 직전에 전송한 PVD startVector 시간, 위치, 이동상태를 입력
    ptrPvd->snapshots.list.count = 1;
    ptrPvd->snapshots.list.array = (Snapshot **)malloc(sizeof(struct Snapshot *));
    ptrPvd->snapshots.list.array[0] = (Snapshot *)malloc(sizeof(struct Snapshot));
    struct Snapshot *ptrSnapshot = ptrPvd->snapshots.list.array[0];

    ptrSnapshot->thePosition.utcTime = (DDateTime *)malloc(sizeof(struct DDateTime));
    ptrSnapshot->thePosition.utcTime->year = (DYear_t *)malloc(sizeof(DYear_t));
    ptrSnapshot->thePosition.utcTime->month = (DMonth_t *)malloc(sizeof(DMonth_t));
    ptrSnapshot->thePosition.utcTime->day = (DDay_t *)malloc(sizeof(DDay_t));
    ptrSnapshot->thePosition.utcTime->hour = (DHour_t *)malloc(sizeof(DHour_t));
    ptrSnapshot->thePosition.utcTime->minute = (DMinute_t *)malloc(sizeof(DMinute_t));
    ptrSnapshot->thePosition.utcTime->second = (DSecond_t *)malloc(sizeof(DSecond_t));
    ptrSnapshot->thePosition.utcTime->offset = NULL; // OPTIONAL, not to use

    ptrSnapshot->thePosition.elevation = (DSRC_Elevation_t *)malloc(sizeof(DSRC_Elevation_t));
    ptrSnapshot->thePosition.speed = (TransmissionAndSpeed *)malloc(sizeof(struct TransmissionAndSpeed));
    ptrSnapshot->thePosition.heading = (Heading_t *)malloc(sizeof(Heading_t));
    ptrSnapshot->thePosition.posAccuracy = NULL;     // OPTIONAL, not to use
    ptrSnapshot->thePosition.posConfidence = NULL;   // OPTIONAL, not to use
    ptrSnapshot->thePosition.timeConfidence = NULL;  // OPTIONAL, not to use
    ptrSnapshot->thePosition.speedConfidence = NULL; // OPTIONAL, not to use

    *ptrSnapshot->thePosition.utcTime->year = t->tm_year + 1900; // (INPUT) <--------------- 직전 전송한 PVD의 UTC 시간 입력 (년도)
    *ptrSnapshot->thePosition.utcTime->month = t->tm_mon + 1;    // (INPUT) <--------------- 직전 전송한 PVD의 UTC 시간 입력 (월)
    *ptrSnapshot->thePosition.utcTime->day = t->tm_mday;         // (INPUT) <--------------- 직전 전송한 PVD의 UTC 시간 입력 (일)
    *ptrSnapshot->thePosition.utcTime->hour = t->tm_hour;        // (INPUT) <--------------- 직전 전송한 PVD의 UTC 시간 입력 (시)
    *ptrSnapshot->thePosition.utcTime->minute = t->tm_min;       // (INPUT) <--------------- 직전 전송한 PVD의 UTC 시간 입력 (분)
    *ptrSnapshot->thePosition.utcTime->second = t->tm_sec;       // (INPUT) <--------------- 직전 전송한 PVD의 UTC 시간 입력 (초)

    ptrSnapshot->thePosition.Long = longitude;          // (INPUT) <--------------- 현재 차량의 위치 (위도) (Longitude, DD 좌표계)
    ptrSnapshot->thePosition.lat = latitude;            // (INPUT) <--------------- 현재 차량의 위치 (경도) (Latitude,  DD 좌표계)
    *ptrSnapshot->thePosition.elevation = elevation;    // (INPUT) <--------------- 현재 차량의 위치 (고도) (Elevation)
    *ptrSnapshot->thePosition.heading = heading;        // (INPUT) <--------------- 현재 차량의 주행 방향 (북쪽 0도)
    ptrSnapshot->thePosition.speed->speed = velocity;   // (INPUT) <-------------- -현재 차량의 속도
    ptrSnapshot->thePosition.speed->transmisson = gear; // (INPUT) <--------------- 현재 차량의 변속기 상태

    json_object *probeID = json_object_new_object();
    json_object *id = json_object_new_array();
    json_object *entityID0 = json_object_new_object();
    json_object *entityID1 = json_object_new_object();
    json_object *entityID2 = json_object_new_object();
    json_object *entityID3 = json_object_new_object();
    json_object_object_add(entityID0, "buf0", json_object_new_string(probeID_id_entityID0));
    json_object_object_add(entityID1, "buf1", json_object_new_string(probeID_id_entityID1));
    json_object_object_add(entityID2, "buf2", json_object_new_string(probeID_id_entityID2));
    json_object_object_add(entityID3, "buf3", json_object_new_string(probeID_id_entityID3));
    json_object_array_add(id, entityID0);
    json_object_array_add(id, entityID1);
    json_object_array_add(id, entityID2);
    json_object_array_add(id, entityID3);
    json_object_object_add(probeID, "id", id);
    json_object_object_add(PVD, "probeID", probeID);

    json_object *startVector = json_object_new_object();
    json_object *utcTime = json_object_new_object();
    json_object *speed = json_object_new_object();
    json_object_object_add(utcTime, "year", json_object_new_int(*ptrPvd->startVector.utcTime->year));
    json_object_object_add(utcTime, "month", json_object_new_int(*ptrPvd->startVector.utcTime->month));
    json_object_object_add(utcTime, "day", json_object_new_int(*ptrPvd->startVector.utcTime->day));
    json_object_object_add(utcTime, "hour", json_object_new_int(*ptrPvd->startVector.utcTime->hour));
    json_object_object_add(utcTime, "minute", json_object_new_int(*ptrPvd->startVector.utcTime->minute));
    json_object_object_add(utcTime, "second", json_object_new_int(*ptrPvd->startVector.utcTime->second));
    json_object_object_add(startVector, "utcTime", utcTime);
    json_object_object_add(startVector, "Long", json_object_new_int(ptrPvd->startVector.Long));
    json_object_object_add(startVector, "lat", json_object_new_int(ptrPvd->startVector.lat));
    json_object_object_add(startVector, "elevation", json_object_new_int(*ptrPvd->startVector.elevation));
    json_object_object_add(startVector, "heading", json_object_new_int(*ptrPvd->startVector.heading));
    json_object_object_add(speed, "speed", json_object_new_int(ptrPvd->startVector.speed->speed));
    json_object_object_add(speed, "transmisson", json_object_new_int(ptrPvd->startVector.speed->transmisson));
    json_object_object_add(startVector, "speed", speed);
    json_object_object_add(PVD, "startVector", startVector);

    json_object *vehicleType = json_object_new_object();
    json_object_object_add(vehicleType, "hpmsType", json_object_new_int(*ptrPvd->vehicleType.hpmsType));
    json_object_object_add(PVD, "vehicleType", vehicleType);

    json_object *snapshots = json_object_new_object();
    json_object *thePosition = json_object_new_object();
    json_object *thePosition_utcTime = json_object_new_object();
    json_object *thePosition_speed = json_object_new_object();
    json_object_object_add(thePosition_utcTime, "year", json_object_new_int(*ptrSnapshot->thePosition.utcTime->year));
    json_object_object_add(thePosition_utcTime, "month", json_object_new_int(*ptrSnapshot->thePosition.utcTime->month));
    json_object_object_add(thePosition_utcTime, "day", json_object_new_int(*ptrSnapshot->thePosition.utcTime->day));
    json_object_object_add(thePosition_utcTime, "hour", json_object_new_int(*ptrSnapshot->thePosition.utcTime->hour));
    json_object_object_add(thePosition_utcTime, "minute", json_object_new_int(*ptrSnapshot->thePosition.utcTime->minute));
    json_object_object_add(thePosition_utcTime, "second", json_object_new_int(*ptrSnapshot->thePosition.utcTime->second));
    json_object_object_add(thePosition, "utcTime", thePosition_utcTime);
    json_object_object_add(thePosition, "Long", json_object_new_int(ptrSnapshot->thePosition.Long));
    json_object_object_add(thePosition, "lat", json_object_new_int(ptrSnapshot->thePosition.lat));
    json_object_object_add(thePosition, "elevation", json_object_new_int(*ptrSnapshot->thePosition.elevation));
    json_object_object_add(thePosition, "heading", json_object_new_int(*ptrSnapshot->thePosition.heading));
    json_object_object_add(thePosition_speed, "speed", json_object_new_int(ptrSnapshot->thePosition.speed->speed));
    json_object_object_add(thePosition_speed, "transmisson", json_object_new_int(ptrSnapshot->thePosition.speed->transmisson));
    json_object_object_add(thePosition, "speed", thePosition_speed);
    json_object_object_add(snapshots, "thePosition", thePosition);
    json_object_object_add(PVD, "snapshots", snapshots);

    // json_object_to_file_ext(filename_pvd, json_object_get(PVD), JSON_C_TO_STRING_PRETTY);

    printf("PVD Logged\n");
    return 0;
}
