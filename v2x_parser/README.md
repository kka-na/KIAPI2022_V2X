# V2X Parser

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

4. ROS Topic
/spat_mag  ( Float32MultiArray )
```
data[2] = [Current Signal EventState, Current Signal minEndTime] 
```
5. Location of Traffic Lights
* SG = SignalGroup, mN = movementName
1) Intersection 1, 2

![spat1](https://user-images.githubusercontent.com/94699295/196671750-543c9304-70c5-4639-8e87-b7325a5a3404.png)
2) Intersection 3

![spat2](https://user-images.githubusercontent.com/94699295/196671776-70237199-f28a-4881-a1e9-2770dadb20af.png)
6. The Number of Traffic Lights
```
if eventState == 3:
    spat_msg = 1  //  Red
elif (eventState == 5) or (eventState == 6):
    spat_msg = 2  //  Green
elif (eventState == 7) or (eventState == 8):
    spat_msg = 3  //  Yellow
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
