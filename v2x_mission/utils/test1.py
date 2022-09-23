from threading import Thread
import time
import rospy
from std_msgs.msg import Int16MultiArray
from geometry_msgs.msg import PoseArray


class Test:
    def __init__(self):
        rospy.init_node('Test')

        self.arrive_info_pub = rospy.Publisher(
            '/arrive_info', Int16MultiArray, queue_size=10)

        rospy.Subscriber('/stage1_mission', PoseArray,
                         self.stage1_mission_callback)
        rospy.Subscriber('/stage1_state', Int16MultiArray,
                         self.stage1_state_callback)

        self.arrive_info = [0, 0]
        self.stage1_state = []
        self.stage1_mission = []

    def publisher(self):
        arr = Int16MultiArray()
        arr.data = self.arrive_info
        self.arrive_info_pub.publish(arr)

    def stage1_mission_callback(self, msg):
        self.stage1_mission = msg.poses

    def stage1_state_callback(self, msg):
        self.stage1_state = msg.data

    def control(self):
        if(len(self.stage1_state) != 0):
            phase1 = []
            phase2 = []
            if self.stage1_state[0] == 0:
                print("[STATUS]WAITING")
                phase1, phase2 = self.doPlanning()
            elif self.stage1_state[0] == 1 or self.stage1_state[0] == 5:  # 5 is test
                phase1, phase2 = self.doPlanning()
                if(self.stage1_state[1] and not self.stage1_state[2] and not self.stage1_state[3]):
                    print("[STATUS]MISSION ACCEPTED")
                    self.passNode(phase1)
                elif(self.stage1_state[1] and self.stage1_state[2] and not self.stage1_state[3]):
                    print("[STATUS]DEAPRTURE ACCEPTED")
                    self.passNode(phase2)
                elif(self.stage1_state[1] and self.stage1_state[2] and self.stage1_state[3]):
                    print("[STATUS]MISSION FINISH")
                    self.arrive_info = [0, 0]
                else:
                    print("[STATUS]STARTING")
            elif(self.stage1_state[0] == 4):
                print("[STATUS]FINISH")
            else:
                print("Not Yet")

    def doPlanning(self):
        phase1 = []
        phase2 = []
        phase = phase1
        for node in self.stage1_mission:
            phase.append(node.orientation)
            if(node.orientation.x == 1.0):
                phase = phase2
        return phase1, phase2

    def passNode(self, phase):
        last_node = {}
        for node in phase:
            last_node = node
            time.sleep(0.25)
            pass
        self.arrive_info[int(last_node.x)-1] = 1

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
