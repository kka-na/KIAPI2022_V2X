import rospy
from geometry_msgs.msg import Vector3
from std_msgs.msg import Int16MultiArray


class PubTest:
    def __init__(self):
        rospy.init_node('PubTest')
        self.pub_gps_pos = rospy.Publisher('/gps_pos', Vector3, queue_size=1)
        self.pub_ekf_euler = rospy.Publisher(
            '/ekf_euler', Vector3, queue_size=1)
        self.pub_can_record = rospy.Publisher(
            '/can_record', Int16MultiArray, queue_size=1)

        self.gps_pos = Vector3()
        self.ekf_euler = Vector3()
        self.can_record = Int16MultiArray()

    def publisher(self):
        self.gps_pos.x = 37.450585
        self.gps_pos.y = 126.656955
        self.gps_pos.z = 10.22
        self.ekf_euler.x = 0
        self.ekf_euler.y = 0
        self.ekf_euler.z = 30.300
        self.can_record.data = [0, 0, 0, 1, 0, 46]

        self.pub_gps_pos.publish(self.gps_pos)
        self.pub_ekf_euler.publish(self.ekf_euler)
        self.pub_can_record.publish(self.can_record)


if __name__ == '__main__':
    pt = PubTest()
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pt.publisher()
        rate.sleep()

    rospy.spin()
