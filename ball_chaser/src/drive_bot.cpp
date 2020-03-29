#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

using namespace std;

//ROS::Publisher motor_command_publisher: 
ros::Publisher motor_command_publisher;
//Create a handle drive request
bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res){

ROS_INFO("Drive to target service requested. Starting...");
//Create a motor command object of type geometry_msgs:Twist
geometry_msgs::Twist motor_command;

//set wheel velocites
motor_command.linear.x = req.linear_x;
motor_command.angular.z = req.angular_z;

//Publish drive angles
motor_command_publisher.publish(motor_command);

//send command check error
res.msg_feedback = "Velocites were set to: linear_x: " + to_string((double)motor_command.linear.x) + ", angular_z: " + to_string((double)motor_command.angular.z);


ROS_INFO("DriveToTarget recieved - L_x:%1.2f, A_z:%1.2f", (float)req.linear_x, (float)req.angular_z);

ROS_INFO_STREAM(res.msg_feedback);

return true;
}

int main(int argc, char** argv)
{
	//Initialize an ROS node
	ros::init(argc, argv, "drive_bot"); 
	//Create a ROS NodeHandle object
	ros::NodeHandle n;

	//Inform ROS master that we will be publishing a message of type geometry_msgs
	motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);

	ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot/",handle_drive_request);

	//while(ros::ok()) {
	//create a motor  command object
	//geometry_msgs::Twist motor_command;
	//Set  wheel  velocites,  forward 0.5,0.5
	//motor_command.linear.x =  0.5;
	//motor_command.angular.z = 0.0;
	//Publish angles to the drive bot
	//motor_command_publisher.publish(motor_command);

	//} 


	ROS_INFO("Ready to send velocity commands");
	ros::spin();

	return 0;


}
