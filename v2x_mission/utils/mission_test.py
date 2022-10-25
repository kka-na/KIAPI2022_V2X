import rospy
import time
from std_msgs.msg import Int16MultiArray


class MissionTest:
    def __init__(self):
        rospy.init_node('MissionTest')
        self.stage1_state_pub = rospy.Publisher(
            '/stage1_state', Int16MultiArray, queue_size=10)
        rospy.Subscriber('/arrive_info', Int16MultiArray, self.arrive_info_cb)
        self.arrive_info = [0, 0]
        self.arr = Int16MultiArray()
        self.stage1_state = []

    def arrive_info_cb(self, msg):
        self.arrive_info = msg.data

    def updateState(self):
        rate = rospy.Rate(10)
        start_time = time.time()
        phase1_time = None
        phase2_time = None
        self.stage1_state = [1, 1, 0, 0]

        while not rospy.is_shutdown():
            if(self.stage1_state[1] and not self.stage1_state[2] and not self.stage1_state[3]):
                if(phase1_time == None and len(self.arrive_info) != 0):
                    if(self.arrive_info[0] == 1):
                        phase1_time = time.time()
                if(phase1_time != None):
                    if(time.time()-phase1_time > 5):
                        self.stage1_state = [1, 1, 1, 0]
            elif(self.stage1_state[1] and self.stage1_state[2] and not self.stage1_state[3]):
                if(phase2_time == None and len(self.arrive_info) != 0):
                    if(self.arrive_info[1] == 1):
                        phase2_time = time.time()
                if(phase2_time != None):
                    if(time.time()-phase2_time > 5):
                        self.stage1_state = [1, 1, 1, 1]
            elif(self.stage1_state[1] and self.stage1_state[2] and self.stage1_state[3]):
                self.stage1_state = [1, 0, 0, 0]
                self.arrive_info = [0, 0]
                break
            self.arr.data = self.stage1_state
            self.stage1_state_pub.publish(self.arr)
            rate.sleep()
        rospy.spin()


if __name__ == '__main__':
    t = MissionTest()
    try:
        t.updateState()
    except rospy.ROSInternalException as e:
        # pass
        print(e)