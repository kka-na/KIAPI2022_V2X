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
        lat0 = 35.64588122580907
        # lon0 = 128.40214778762413
        # h0 = 47.256
        node_info = {"type":0, "idx":1, "lat":35.64588122580907, "lng":128.40214778762413}
        for node in self.stage1_mission:
            node_info["type"] = node.orientation.x
            node_info["idx"] = node.orientation.y
            node_info["lat"] = node.orientation.z
            node_info["lng"] = node.orientation.w 
            phase.append(node_info)
            if(node.orientation.x == 1.0):
                phase = phase2
        return phase1, phase2



if __name__ == '__main__':
    mp = MissionParse()
    try:
        mp.planner()

    except rospy.ROSInitException:
        pass
