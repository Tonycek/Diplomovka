#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <control_msgs/JointTrajectoryAction.h>
#include <moveit_msgs/MoveGroupActionGoal.h>
#include <moveit_msgs/MoveGroupAction.h>
//#include <Constraints.h>

int main(int argc, char **argv){
  ros::init(argc, argv, "test");

  actionlib::SimpleActionClient<moveit_msgs::MoveGroupAction> ac("/move_group/", true);

  ROS_INFO("Waiting for action server to start!");

  ac.waitForServer();

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  moveit_msgs::MoveGroupGoal goal;

  goal.request.group_name = "manipulator";
//  goal.request.num_planning_attempts = 1;
  goal.request.allowed_planning_time = 5;
  
/*
  geometry_msgs::PoseStamped pose;
  pose.header.frame_id = "torso_lift_link";
  pose.pose.position.x = 0.75;
  pose.pose.position.y = -0.2;
  pose.pose.position.z = 0.0;
  pose.pose.orientation.x = 0.0;
  pose.pose.orientation.y = 0.0;
  pose.pose.orientation.z = 0.0;
  pose.pose.orientation.w = 1.0;
  geometry_msgs::Vector3 positione;
  positione.x = 0.75;
  positione.y = -0.2;
  positione.z = 0.0;*/

  moveit_msgs::Constraints g0;
  g0.joint_constraints.resize(1);
  g0.joint_constraints[0].joint_name= "joint_1";
  g0.joint_constraints[0].position = -0.157835049197; 
  g0.joint_constraints[0].tolerance_above = 0.0001;
  g0.joint_constraints[0].weight = 1.0;  

  goal.request.goal_constraints.resize(1);
  goal.request.goal_constraints[0] = g0;
//  g0.target_point_offset = positione;
/*
  goal.request.workspace_parameters.header.seq = 0;
  goal.request.workspace_parameters.header.frame_id = "/base_link";  

  goal.request.workspace_parameters.min_corner.x = -1;
  goal.request.workspace_parameters.min_corner.y = -1;
  goal.request.workspace_parameters.min_corner.z = -1;  

  goal.request.goal_constraints.resize(1);
  goal.request.goal_constraints[0].joint_constraints.resize(1);
  goal.request.goal_constraints[0].joint_constraints[0].joint_name = "joint111";
  goal.request.goal_constraints[0].joint_constraints[0].position = -0.30;
  goal.request.goal_constraints[0].joint_constraints[0].tolerance_above = 0.0001;
  goal.request.goal_constraints[0].joint_constraints[0].tolerance_below = 0.0001;
  goal.request.goal_constraints[0].joint_constraints[0].weight = 1;
*/ 

/*
  while (ros::ok())
  {
     ac.sendGoal(goal);
  }
*/

  ac.sendGoal(goal);
   if(!ac.waitForResult(ros::Duration(5))) {
     ROS_INFO_STREAM("Apparently returned early");
   }
   if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
     ROS_INFO("It worked!");
   else
   ROS_WARN_STREAM("Fail: " << ac.getState().toString() << ": " << ac.getState().getText());
   std::cout << *ac.getResult() << std::endl;
   sleep(5);
/*  
  goal.trajectory.joint_names.resize(7);
  goal.trajectory.joint_names.push_back("r_shoulder_pan_joint");
    goal.trajectory.joint_names.push_back("r_shoulder_lift_joint");
    goal.trajectory.joint_names.push_back("r_upper_arm_roll_joint");
    goal.trajectory.joint_names.push_back("r_elbow_flex_joint");
    goal.trajectory.joint_names.push_back("r_forearm_roll_joint");
    goal.trajectory.joint_names.push_back("r_wrist_flex_joint");
    goal.trajectory.joint_names.push_back("r_wrist_roll_joint");
  
  goal.trajectory.points.resize(2);
   
  // First trajectory point
  // Positions
  int ind = 0;
  goal.trajectory.points[ind].positions.resize(7);
  goal.trajectory.points[ind].positions[0] = 0.0;
  goal.trajectory.points[ind].positions[1] = 0.0;
  goal.trajectory.points[ind].positions[2] = 0.0;
  goal.trajectory.points[ind].positions[3] = 0.0;
  goal.trajectory.points[ind].positions[4] = 0.0;
  goal.trajectory.points[ind].positions[5] = 0.0;
  goal.trajectory.points[ind].positions[6] = 0.0;


  goal.trajectory.points[ind].velocities.resize(7);
    for (size_t j = 0; j < 7; ++j)
    {
      goal.trajectory.points[ind].velocities[j] = 0.0;
    }
    // To be reached 1 second after starting along the trajectory
    goal.trajectory.points[ind].time_from_start = ros::Duration(1.0);

    // Second trajectory point
    // Positions
    ind += 1;
    goal.trajectory.points[ind].positions.resize(7);
    goal.trajectory.points[ind].positions[0] = -0.3;
    goal.trajectory.points[ind].positions[1] = 0.2;
    goal.trajectory.points[ind].positions[2] = -0.1;
    goal.trajectory.points[ind].positions[3] = -1.2;
    goal.trajectory.points[ind].positions[4] = 1.5;
    goal.trajectory.points[ind].positions[5] = -0.3;
    goal.trajectory.points[ind].positions[6] = 0.5;
    // Velocities
    goal.trajectory.points[ind].velocities.resize(7);
    for (size_t j = 0; j < 7; ++j)
    {
      goal.trajectory.points[ind].velocities[j] = 0.0;
    }
    
    goal.trajectory.points[ind].time_from_start = ros::Duration(2.0);
  */ 

 return 0;
}
