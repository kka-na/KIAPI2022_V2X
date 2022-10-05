from threading import Thread
import rospy
from std_msgs.msg import Int16MultiArray
from geometry_msgs.msg import PoseArray
import time


class Test2:
    def __init__(self):
        rospy.init_node('Test2')

        rospy.Subscriber('/stage2_mission', PoseArray,
                         self.stage2_mission_callback)
        rospy.Subscriber('/stage2_state', Int16MultiArray,
                         self.stage2_state_callback)

        self.stage2_state = []
        self.stage2_mission = []

    def stage2_mission_callback(self, msg):
        self.stage2_mission = msg.poses

    def stage2_state_callback(self, msg):
        self.stage2_state = msg.data

    def control(self):
        if(len(self.stage2_state) != 0):
            phase = []
            if self.stage2_state[0] == 1 or self.stage2_state[0] == 4:
                print(self.stage2_state[1:])
                if all(state == 0 for state in self.stage2_state[1:]):
                    print("[STATUS]WAITING")
                    phase = self.doPlanning()
                elif all(state == 1 for state in self.stage2_state[1:]):
                    print("[STATUS]FINISH")
                else:
                    print("[STATUS]STARTING")
                    self.passItem(phase)
            else:
                print("Not Yet")

    def doPlanning(self):
        phase = []
        for item in self.stage2_mission:
            phase.append(item)
        return phase

    def passItem(self, phase):
        for item in phase:
            if int(item.orientation.x) == 1:
                print("[ITEM]LAP Time Reducing")
            if int(item.orientation.x) == 2:
                print("[ITEM]LAP Time Increasing")
            if int(item.orientation.x) == 3:
                print("[ITEM]Booster while {} sec ".format(item.position.z))
            time.sleep(0.25)

    def update(self):
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            self.control()
            rate.sleep()
        rospy.spin()


if __name__ == '__main__':
    t = Test2()

    try:
        t.update()
    except rospy.ROSInternalException:
        pass
