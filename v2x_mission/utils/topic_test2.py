import rospy
from std_msgs.msg import Int16MultiArray
from geometry_msgs.msg import Pose, PoseArray


STAGE2_ITEMS = [
    # {"idx": 0, "type": 0, "score": 0, "lat": 35.649286, "lng": 128.4013599},  # Start
    {"idx": 1, "type": 1, "score": 5, "lat": 35.6491704, "lng": 128.4013386},
    {"idx": 2, "type": 1, "score": 5, "lat": 35.6501853, "lng": 128.4001426},
    {"idx": 3, "type": 1, "score": 5, "lat": 35.6549743, "lng": 128.3938531},
    {"idx": 4, "type": 1, "score": 10, "lat": 35.6564015, "lng": 128.3919018},
    {"idx": 5, "type": 1, "score": 5, "lat": 35.6552222, "lng": 128.3948077},
    {"idx": 6, "type": 1, "score": 10, "lat": 35.6531639, "lng": 128.3970079},
    {"idx": 7, "type": 1, "score": 8, "lat": 35.6531859, "lng": 128.3970374},
    {"idx": 8, "type": 1, "score": 5, "lat": 35.6532066, "lng": 128.3970722},
    {"idx": 9, "type": 2, "score": -5, "lat": 35.6512138, "lng": 128.3989831},
    {"idx": 10, "type": 2, "score": -5, "lat": 35.6522498, "lng": 128.3979354},
    {"idx": 11, "type": 2, "score": -5, "lat": 35.6542104, "lng": 128.3959423},
    {"idx": 12, "type": 2, "score": -10, "lat": 35.6541667, "lng": 128.3958819},
    {"idx": 13, "type": 3, "score": 10, "lat": 35.6522011, "lng": 128.3981997},
]


class PubTest:
    def __init__(self):
        rospy.init_node('PubTest')
        self.pub_stage2_mission = rospy.Publisher(
            '/stage2_mission', PoseArray, queue_size=1)

    def stage2_mission_publisher(self):
        self.stage2_mission = PoseArray()

        for node in STAGE2_ITEMS:
            pose = Pose()
            '''
            orientation.x : type
            orientation.y : extend
            orientation.z : latitude
            orientation.w : longitude
            position.x : item id
            position.y : speed(socre)
            position.z : duration

            node_info = {"id": int(node.position.y), 
            "type": int(node.orientation.x),  
            "score": int(node.position.y), 
            "lat": float(node.orientation.z), 
            "lng": float(node.orientation.w), 
            "duration": int(node.position.z), 
            "extend": int(node.orientation.y)}
                       vvv

            '''
            pose.orientation.x = int(node["type"])
            pose.orientation.y = 10
            pose.orientation.z = float(node["lat"])
            pose.orientation.w = float(node["lng"])
            pose.position.x = int(node["idx"])
            pose.position.y = int(node["score"])
            pose.position.z = 5
            self.stage2_mission.poses.append(pose)
        self.pub_stage2_mission.publish(self.stage2_mission)


if __name__ == '__main__':
    pt = PubTest()
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pt.stage2_mission_publisher()
        rate.sleep()

    rospy.spin()
