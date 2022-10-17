import rospy
from geometry_msgs.msg import Vector3
from std_msgs.msg import Int16MultiArray
from sbg_driver.msg import SbgEkfEuler, SbgGpsPos


###
# hello 
class PubTest:
    def __init__(self):
        rospy.init_node('PubTest')
        self.pub_gps_pos = rospy.Publisher(
            '/sbg/gps_pos', SbgGpsPos, queue_size=1)
        self.pub_ekf_euler = rospy.Publisher(
            '/sbg/ekf_euler', SbgEkfEuler, queue_size=1)
        self.pub_can_record = rospy.Publisher(
            '/can_record', Int16MultiArray, queue_size=1)

        self.pub_laneid = rospy.Publisher(
            '/current_LaneID', Int16MultiArray, queue_size=1)

        self.gps_pos = SbgGpsPos()
        self.ekf_euler = SbgEkfEuler()
        self.can_record = Int16MultiArray()
        
        self.laneid = Int16MultiArray()

    def publisher(self):
        self.gps_pos.latitude = 37.450585
        self.gps_pos.longitude = 126.656955
        self.gps_pos.altitude = 10.22
        self.ekf_euler.angle.x = 0
        self.ekf_euler.angle.y = 0
        self.ekf_euler.angle.z = 30.300
        self.can_record.data = [0, 0, 0, 1, 0, 46]

        self.laneid.data = {27}
        
        self.pub_gps_pos.publish(self.gps_pos)
        self.pub_ekf_euler.publish(self.ekf_euler)
        self.pub_can_record.publish(self.can_record)

        self.pub_laneid.publish(self.laneid)

if __name__ == '__main__':
    pt = PubTest()
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pt.publisher()
        rate.sleep()

    rospy.spin()
