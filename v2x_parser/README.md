# V2X

KIAPI : V2X Mission

JSON Parsing and Writting with C for V2X PVD, SPaT Message Logging.

1. Install

```
sudo apt-get remove libjson-c-dev
```

2. Build

```
clone in catkin_ws
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
