#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <control_msgs/JointTrajectoryAction.h>
#include <moveit_msgs/MoveGroupActionGoal.h>
#include <moveit_msgs/MoveGroupAction.h>
#include <moveit_msgs/GetPositionIK.h>
#include <moveit_msgs/KinematicSolverInfo.h>
#include <moveit_msgs/PositionIKRequest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
//#include "../../industrial_core/industrial_robot_client/include/industrial_robot_client/joint_trajectory_downloader.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char **argv){

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
   /* if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 1070;//atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    bzero(buffer,256);
    //n = read(newsockfd,buffer,255);
    //if (n < 0) error("ERROR reading from socket");
    //printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"4",18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);

    // zaciatok prace s ROSOM
  ros::init(argc, argv, "test");
  ros::NodeHandle node_handle;

  //industrial_robot_client::joint_trajectory_downloader::JointTrajectoryDownloader jtd;
  //  jtd.send_to_robot();
  actionlib::SimpleActionClient<moveit_msgs::MoveGroupAction> ac("/move_group/", true);

/*
  ros::service::waitForService("/compute_ik");
  ros::ServiceClient ik_client = rh.serviceClient<moveit_msgs::GetPositionIK>("/compute_ik");
  
  moveit_msgs::KinematicSolverInfo request;
  moveit_msgs::KinematicSolverInfo response;

  // define the service messages
  moveit_msgs::PositionIKRequest ik_request;
  ik_request.group_name = "manipulator"; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


  moveit_msgs::GetPositionIK::Request  gpik_req;
  moveit_msgs::GetPositionIK::Response gpik_res;
*/
  ROS_INFO("Waiting for action server to start!");

  ac.waitForServer();

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  moveit_msgs::MoveGroupGoal goal;
    
  goal.request.group_name = "manipulator";  //!!!!!!!!!!!!!!!!!!!!!!
//  goal.request.num_planning_attempts = 1;
//  goal.request.allowed_planning_time = 10; //!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
//  gpik_req.timeout = ros::Duration(5.0);
//  gpik_req.ik_request.ik_link_name = "r_wrist_roll_link";

//  gpik_req.ik_request.pose_stamped.header.frame_id = "torso_lift_link";
/*
  gpik_req.ik_request.pose_stamped.pose.position.x = 0.75;
  gpik_req.ik_request.pose_stamped.pose.position.y = 0.5;
  gpik_req.ik_request.pose_stamped.pose.position.z = 0.5;
  gpik_req.ik_request.group_name = "manipulator";

  gpik_req.ik_request.pose_stamped.pose.orientation.x = 0.5;
  gpik_req.ik_request.pose_stamped.pose.orientation.y = 0.5;
  gpik_req.ik_request.pose_stamped.pose.orientation.z = 0.5;
  gpik_req.ik_request.pose_stamped.pose.orientation.w = 1.0;
  gpik_req.ik_request.robot_state.joint_state.position.resize(response.joint_names.size());
  gpik_req.ik_request.robot_state.joint_state.name = response.joint_names;

  for(unsigned int i=0; i< response.joint_names.size(); i++)
  {
    gpik_req.ik_request.robot_state.joint_state.position[i] = (response.limits[i].min_position + response.limits[i].max_position)/2.0;
  }

  if(ik_client.call(gpik_req, gpik_res))
  {
    if(gpik_res.error_code.val == gpik_res.error_code.SUCCESS)
      for(unsigned int i=0; i < gpik_res.solution.joint_state.name.size(); i ++)
        ROS_INFO("Joint: %s %f",gpik_res.solution.joint_state.name[i].c_str(),gpik_res.solution.joint_state.position[i]);
    else
      ROS_ERROR("Inverse kinematics failed");
  }
  else
    ROS_ERROR("Inverse kinematics service call failed"); */
//    ros::shutdown();
/*
  ros::ServiceClient service_client = node_handle.serviceClient<moveit_msgs::GetPositionIK> ("compute_ik");

  robot_model_loader::RDFLoader robot_model_loader("robot_description"); 
robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
robot_state::JointStateGroup* joint_state_group = kinematic_state->getJointStateGroup("right_arm");
service_request.ik_request.robot_state.joint_state.name = joint_state_group->getJointModelGroup()->getJointModelNames();
joint_state_group->setToRandomValues();
joint_state_group->getVariableValues(service_request.ik_request.robot_state.joint_state.position);

while(!service_client.exists())
{
  ROS_INFO("Waiting for service");
  sleep(1.0);
}

  moveit_msgs::GetPositionIK::Request service_request;
moveit_msgs::GetPositionIK::Response service_response;
service_request.ik_request.group_name = "manipulator";
service_request.ik_request.pose_stamped.header.frame_id = "base";  
service_request.ik_request.pose_stamped.pose.position.x = 0.235;
service_request.ik_request.pose_stamped.pose.position.y = -0.2996;
service_request.ik_request.pose_stamped.pose.position.z = -0.3011;
service_request.ik_request.pose_stamped.pose.orientation.x = 0.674056;
service_request.ik_request.pose_stamped.pose.orientation.y = -0.73868;
service_request.ik_request.pose_stamped.pose.orientation.z = 0.0;
service_request.ik_request.pose_stamped.pose.orientation.w = 0.0;

   if(service_client.call(service_request, service_response)){
      for(unsigned int i=0; i< service_response.solution.joint_state.name.size(); i++)
    {
      ROS_DEBUG("Joint: %d %s",i,service_response.solution.joint_state.name[i].c_str());
    }
   }
*/
//   ROS_INFO("Joint: 1 %s",service_response.solution.joint_state.name[0]);
   sleep(2);
//##########################################################################  
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
  g0.joint_constraints.resize(6);
  g0.joint_constraints[0].joint_name= "joint_1";
  g0.joint_constraints[0].position = 0;// -0.157835049197; 
  g0.joint_constraints[0].tolerance_above = 0.0001;
  g0.joint_constraints[0].weight = 1.0; 

  g0.joint_constraints[1].joint_name= "joint_2";
  g0.joint_constraints[1].position = 0;//-0.157835049197;
  g0.joint_constraints[1].tolerance_above = 0.0001;
  g0.joint_constraints[1].weight = 1.0;

  g0.joint_constraints[2].joint_name= "joint_3";
  g0.joint_constraints[2].position = 0;//-0.157835049197;
  g0.joint_constraints[2].tolerance_above = 0.0001;
  g0.joint_constraints[2].weight = 1.0;

  g0.joint_constraints[3].joint_name= "joint_4";
  g0.joint_constraints[3].position = 0;//-0.157835049197;
  g0.joint_constraints[3].tolerance_above = 0.0001;
  g0.joint_constraints[3].weight = 1.0;

  g0.joint_constraints[4].joint_name= "joint_5";
  g0.joint_constraints[4].position = 0;//-0.157835049197;
  g0.joint_constraints[4].tolerance_above = 0.0001;
  g0.joint_constraints[4].weight = 1.0;

  g0.joint_constraints[5].joint_name= "joint_6";
  g0.joint_constraints[5].position = 0;//-0.157835049197;
  g0.joint_constraints[5].tolerance_above = 0.0001;
  g0.joint_constraints[5].weight = 1.0; 

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
/*
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
*/
   char input;
   std::cout << "Zadaj znak: "; 
   while(true){
     std::cin >> input;
     if (input == 'k')
     {
        ac.sendGoal(goal);
        if(!ac.waitForResult(ros::Duration(10))) {
           ROS_INFO_STREAM("Apparently returned early");
        }
        if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
          ROS_INFO("It worked!");
        else
        ROS_WARN_STREAM("Fail: " << ac.getState().toString() << ": " << ac.getState().getText());
        std::cout << *ac.getResult() << std::endl;
        sleep(5);
        break;
     } 
   }
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
