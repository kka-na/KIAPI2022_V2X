import rospy
from std_msgs.msg import Int16MultiArray


class PubTest:
    def __init__(self):
        rospy.init_node('PubTest')
        self.pub_arrive_info = rospy.Publisher(
            '/arrive_info', Int16MultiArray, queue_size=1)
        self.arrive_info = Int16MultiArray()

    def publisher(self):
        self.arrive_info.data = [0, 1]
        self.pub_arrive_info.publish(self.arrive_info)


if __name__ == '__main__':
    pt = PubTest()
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pt.publisher()
        rate.sleep()

    rospy.spin()
