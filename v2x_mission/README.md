# V2X Mission

V2X Mission protocol integration with ROS 


1. Build

```
clone in catkin_ws/src
catkin_make
```

2. Execute

1) Parsing mission information, publish ROS topics, communicate with Control Systems
```
rosrun v2x_mission v2x_mission
```

2-1) Testing Stage1 mission planing, passing node etc, ...
```
python utils/test1.py 
```

2-2) Testing Stage2 mission planing, passing item etc, ...
```
python utils/test2.py
```


3. ROS Topics

1) Stage 1

/stage1_mission  ( geometry_msgs/PoseArray )
```
orientation.x : type
orientation.y : index
orientation.z : latitude
orientation.w : longitude
```

/stage1_state  ( std_msgs/Int16MultiArray )
```
data[0] : Mission Status
data[1] : Selection Accepted
data[2] : Departure Accepted
data[3] : Destination Accepted
```

/arrive_info  ( std_msgs/Int16MultiArray )
```
data[0] : Departure, if arrived == 1
data[1] : Destination, if arrived == 1
```

2) Stage 2

/stage2_mission  ( geometry_msgs/PoseArray )
```
orientation.x : type
orientation.y : extend
orientation.z : latitude
orientation.w : longitude
position.x : item id
position.y : speed
position.z : duration
```

/stage2_state  ( std_msgs/Int16MultiArray )
```
data[0] : Mission Status
data[item_id] : Accepted
```