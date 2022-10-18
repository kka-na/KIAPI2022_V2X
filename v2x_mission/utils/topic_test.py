import rospy
from std_msgs.msg import Int16MultiArray
from geometry_msgs.msg import Pose, PoseArray


STAGE1_NODES_PHASE1 = [
    {"type": 1, "idx": 1, "lat": 35.6473269, "lng": 128.4007912},
    {"type": 0, "idx": 2, "lat": 35.6475503, "lng": 128.4004897},
    {"type": 0, "idx": 3, "lat": 35.6476327, "lng": 128.4000041},
    {"type": 0, "idx": 4, "lat": 35.6479669, "lng": 128.3994251},
    {"type": 0, "idx": 5, "lat": 35.6484246, "lng": 128.3991161},
    # pedestrian1
    {"type": 0, "idx": 6, "lat": 35.6483555, "lng": 128.3994841},
    # bumper
    {"type": 0, "idx": 7, "lat": 35.6479855, "lng": 128.3998964},
    {"type": 0, "idx": 8, "lat": 35.6476466, "lng": 128.4000641},
    {"type": 0, "idx": 9, "lat": 35.6479488, "lng": 128.3994474},
    {"type": 0, "idx": 10, "lat": 35.6487352, "lng": 128.3984803},
    # sudden event
    {"type": 0, "idx": 11, "lat": 35.6492639, "lng": 128.398201},
    # vehicle 1
    {"type": 0, "idx": 12, "lat": 35.6492473, "lng": 128.3984808},
    # vegicle 2
    {"type": 0, "idx": 13, "lat": 35.6487824, "lng": 128.3990016},
    # pedestrian 2
    {"type": 2, "idx": 14, "lat": 35.648287, "lng": 128.3995602}
]


class PubTest:
    def __init__(self):
        rospy.init_node('PubTest')
        self.pub_arrive_info = rospy.Publisher(
            '/arrive_info', Int16MultiArray, queue_size=1)
        self.arrive_info = Int16MultiArray()

        self.pub_stage1_mission = rospy.Publisher(
            '/stage1_mission', PoseArray, queue_size=1)
        self.stage1_mission = PoseArray()

    def arrive_info_publisher(self):
        self.arrive_info.data = [1, 0]
        self.pub_arrive_info.publish(self.arrive_info)

    def stage1_mission_publisher(self):
        for i in range(len(STAGE1_NODES_PHASE1)):
            pose = Pose()
            pose.orientation.x = int(STAGE1_NODES_PHASE1[i]["type"])
            pose.orientation.y = int(STAGE1_NODES_PHASE1[i]["idx"])
            pose.orientation.z = float(STAGE1_NODES_PHASE1[i]["lat"])
            pose.orientation.w = float(STAGE1_NODES_PHASE1[i]["lng"])
            self.stage1_mission.poses.append(pose)
        self.pub_stage1_mission.publish(self.stage1_mission)


if __name__ == '__main__':
    pt = PubTest()
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pt.stage1_mission_publisher()
        rate.sleep()

    rospy.spin()
