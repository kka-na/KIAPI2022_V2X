import rospy
from geometry_msgs.msg import Vector3
from std_msgs.msg import Int16MultiArray
from sbg_driver.msg import SbgEkfEuler, SbgGpsPos
from std_msgs.msg import Float32


###
# hello 
class PubTest:
    def __init__(self):
        rospy.init_node('PubTest')
        # self.pub_gps_pos = rospy.Publisher(
        #     '/sbg/gps_pos', SbgGpsPos, queue_size=1)
        # self.pub_ekf_euler = rospy.Publisher(
        #     '/sbg/ekf_euler', SbgEkfEuler, queue_size=1)
        self.pub_car_v = rospy.Publisher(
            '/car_v', Float32, queue_size=1)

        self.pub_laneid = rospy.Publisher(
            '/current_LaneID', Int16MultiArray, queue_size=1)

        # self.gps_pos = SbgGpsPos()
        # self.ekf_euler = SbgEkfEuler()
        self.car_v = Float32()
        
        self.laneid = Int16MultiArray()

    def publisher(self):
        # self.gps_pos.latitude = 37.450585
        # self.gps_pos.longitude = 126.656955
        # self.gps_pos.altitude = 10.22
        # self.ekf_euler.angle.x = 0
        # self.ekf_euler.angle.y = 0
        # self.ekf_euler.angle.z = 30.300
        self.car_v.data = 15.0


        self.laneid.data = {60}
        '''
        # SignalGroup[STR,LEF,PED]

                13,14,15
        22,23,24        16,17,18
                19,20,21
        '''
        '''
        # KIAPI-3     KIAPI-2     KIAPI-1
        '''
        # self.pub_gps_pos.publish(self.gps_pos)
        # self.pub_ekf_euler.publish(self.ekf_euler)
        self.pub_car_v.publish(self.car_v)

        self.pub_laneid.publish(self.laneid)

if __name__ == '__main__':
    pt = PubTest()
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pt.publisher()
        rate.sleep()

    rospy.spin()
