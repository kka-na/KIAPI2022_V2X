# V2X

KIAPI2022 : V2X Parser

JSON Parsing and Writting with C for V2X PVD, SPaT Message Logging.

1. Install

```
#for JSON Parsing
sudo apt-get remove libjson-c-dev

#for SBG Driver
sudo apt-get install ros-noetic-sbg-driver
```

2. Build

```
clone in catkin_ws/src
catkin_make
```

3. Execute

```
rosrun v2x_parser v2x_parser
```

## Result pvd.json

```
{
  "probeID":{
    "id":[
      {
        "buf0":"0xCE"
      },
      {
        "buf1":"0x20"
      },
      {
        "buf2":"0x67"
      },
      {
        "buf3":"0x06"
      }
    ]
  },
  "startVector":{
    "utcTime":{
      "year":2022,
      "month":6,
      "day":23,
      "hour":22,
      "minute":43,
      "second":9
    },
    "Long":126656955,
    "lat":37450585,
    "elevation":1022,
    "heading":3030,
    "speed":{
      "speed":0,
      "transmisson":1
    }
  },
  "vehicleType":{
    "hpmsType":4
  },
  "snapshots":{
    "thePosition":{
      "utcTime":{
        "year":2022,
        "month":6,
        "day":23,
        "hour":22,
        "minute":43,
        "second":9
      },
      "Long":126656955,
      "lat":37450585,
      "elevation":1022,
      "heading":3030,
      "speed":{
        "speed":0,
        "transmisson":1
      }
    }
  }
}
```

## Result spat_KIAPI-1.json

```
{
  "name":"KIAPI-1",
  "id":{
    "region":0,
    "id":1
  },
  "revision":46,
  "status":"1ed28c50",
  "moy":154257,
  "timestamp":13054,
  "states0":{
    "movementName":"STR\/SE",
    "signalGroup":16,
    "state-time-speed":{
      "eventState":5,
      "timing_minEndTime":10
    },
    "maneuverAssistList":{
      "connectionID":0,
      "pedBicycleDetect":true
    }
  },
  "states1":{
    "movementName":"LEF\/SE",
    "signalGroup":17,
    "state-time-speed":{
      "eventState":6,
      "timing_minEndTime":10
    },
    "maneuverAssistList":{
      "connectionID":0,
      "pedBicycleDetect":true
    }
  },

  ...

  "states11":{
    "movementName":"PED\/NE",
    "signalGroup":15,
    "state-time-speed":{
      "eventState":3,
      "timing_minEndTime":610
    },
    "maneuverAssistList":{
      "connectionID":0,
      "pedBicycleDetect":true
    }
  }
}
```
