import rospy
from std_msgs.msg import Int16MultiArray


class MissionTest:
    def __init__(self):
        rospy.init_node('MissionTest')
        self.stage2_state_pub = rospy.Publisher(
            '/stage2_state', Int16MultiArray, queue_size=10)
        self.arr = Int16MultiArray()
        self.stage2_state = []

    def arrive_info_cb(self, msg):
        self.arrive_info = msg.data

    def updateState(self):
        rate = rospy.Rate(10)
        self.stage2_state = [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        while not rospy.is_shutdown():
            self.arr.data = self.stage2_state
            self.stage2_state_pub.publish(self.arr)
            rate.sleep()
        rospy.spin()


if __name__ == '__main__':
    t = MissionTest()
    try:
        t.updateState()
    except rospy.ROSInternalException:
        pass
