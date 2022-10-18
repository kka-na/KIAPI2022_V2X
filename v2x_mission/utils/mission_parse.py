import rospy
from geometry_msgs.msg import PoseArray


class MissionParse:
    def __init__(self):
        rospy.init_node('MissionParse')

        rospy.Subscriber('/stage1_mission', PoseArray,
                         self.stage1_mission_callback)
        self.stage1_mission = []
        self.stage1_state = [1, 0]
        self.stage1_nodes_phase1 = []
        self.stage1_nodes_phase2 = []

    def stage1_mission_callback(self, msg):
        self.stage1_mission = msg.poses

    def planner(self):
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            if(self.stage1_state[0] and not self.stage1_state[1]):
                self.stage1_nodes_phase1, self.stage2_nodes_phase2 = self.parseMission()
            elif(self.stage1_state[0] and self.stage1_state[1]):
                print("Mission Selected")
                print(self.stage1_nodes_phase1)
                print(self.stage1_nodes_phase2)
                break
        rospy.spin()

    def parseMission(self):
        phase1 = []
        phase2 = []
        phase = phase1
        for node in self.stage1_mission:


if __name__ == '__main__':
    mp = MissionParse()
    try:
        mp.planner()

    except rospy.ROSInitException:
        pass
