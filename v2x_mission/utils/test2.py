from threading import Thread
import rospy
from std_msgs.msg import Int16MultiArray
from geometry_msgs.msg import PoseArray


class Test:
    def __init__(self):
        rospy.init_node('Test')

        self.arrive_info_pub = rospy.Publisher(
            '/arrive_info', Int16MultiArray, queue_size=1)

        rospy.Subscriber('/stage1_mission', PoseArray,
                         self.stage1_mission_callback)
        rospy.Subscriber('/stage1_state', Int16MultiArray,
                         self.stage1_state_callback)
        # rospy.Subscriber('/stage2_mission', PoseArray,
        #                  self.stage2_mission_callback)
        # rospy.Subscriber('/stage2_state', Int16MultiArray,
        #                  self.stage2_state_callback)

        self.arrive_info = [0, 0]
        self.stage1_state = 0
        self.stage1_mission = []

        self.stage2_state = 0
        self.stage2_mission = []

    def publisher(self):
        arr = Int16MultiArray()
        arr.data = self.arrive_info
        self.arrive_info_pub.publish(arr)

    def stage1_mission_callback(self, msg):
        self.stage1_mission = msg.poses

    def stage1_state_callback(self, msg):
        self.stage1_state = msg.data[0]
        print(msg.data[0])

    def control(self):
        if self.stage1_state == 1 or self.stage1_state == 5:
            print("Start")
            print(self.stage1_mission)
        else:
            print("Not Yet")

    def set(self):
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            self.publisher()
            rate.sleep()
        rospy.spin()

    def update(self):
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            self.control()
            rate.sleep()
        rospy.spin()


if __name__ == '__main__':
    t = Test()

    try:
        th1 = Thread(target=t.set)
        th2 = Thread(target=t.update)

        th1.start()
        th2.start()
    except rospy.ROSInternalException:
        pass
