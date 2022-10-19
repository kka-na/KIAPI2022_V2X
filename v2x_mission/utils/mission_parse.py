from curses import nocbreak
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
        rate = rospy.Rate(0.5)
        while not rospy.is_shutdown():
            if(self.stage1_state[0] and not self.stage1_state[1]):
                self.parseMission()
                if(len(self.stage1_nodes_phase1) != 0 and len(self.stage1_nodes_phase2) != 0):
                    self.stage1_state[1] = 1
            elif(self.stage1_state[0] and self.stage1_state[1]):
                print("Mission Selected")
                print(self.stage1_nodes_phase1)
                print(self.stage1_nodes_phase2)
                break
            rate.sleep()
        rospy.spin()

    def parseMission(self):
        for node in self.stage1_mission:
            node_info = {"type": int(node.orientation.x), "idx": int(
                node.orientation.y), "lat": float(node.orientation.z), "lng": float(node.orientation.w)}
            self.stage1_nodes_phase1.append(node_info)
            if(int(node.orientation.x) == 1):
                break
        for node in self.stage1_mission:
            if(int(node.orientation.x) == 1):
                continue
            node_info = {"type": int(node.orientation.x), "idx": int(
                node.orientation.y), "lat": float(node.orientation.z), "lng": float(node.orientation.w)}
            self.stage1_nodes_phase2.append(node_info)
            if(int(node.orientation.x) == 2):
                break


if __name__ == '__main__':
    mp = MissionParse()
    try:
        mp.planner()

    except rospy.ROSInitException:
        pass
