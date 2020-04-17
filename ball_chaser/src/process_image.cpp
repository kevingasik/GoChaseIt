  
#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
//Credited  towards franz fjp, I tried my best to solve this problem  but ended up looking at  how he  solved  it. I learneda great deal from this creator. 


// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // Call the command_robot service and pass the requested motor commands
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    int count = 0;

    bool found_ball = false;
    // TODO: Loop through each pixel in the image and check if there's a bright white one
    int row = 0;
    int step = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    //ROS_INFO("height: %d, width: %d, step: %d", img.height, img.width, img.step);
    //ROS_INFO("HEIGHT: %f, STEP: %f", img.height, img.step);
    for (row = 0; row < img.height && found_ball == false; row++)
    {
        for (step = 0; step < img.step && found_ball == false; ++step)
        {   
            i = (row*img.step)+step;
	    j = (row*img.step)+step+1;
	    k = (row*img.step)+step+2;
            //ROS_INFO("row: %d, step: %d, i: %d", row, step, i);
            if ((img.data[i] == white_pixel)&& (img.data[j]== white_pixel) && (img.data[k] == white_pixel))
            {   

                found_ball = true;
		
            }
	}
    }
    if (found_ball)
    {
        // Then, identify if this pixel falls in the left, mid, or right side of the image
        int imgThird = img.width/3;
        int col = step/3;
        //ROS_INFO("col: %d", col);
        if (col < imgThird) 
        {
            drive_robot(0.1, 0.1);
            //ROS_INFO("LEFT");
        } 
        else if (col >= imgThird && col < 2*imgThird)
        {
            drive_robot(0.5, 0.0);
            //ROS_INFO("MID");
        }
        else if (col >= 2*imgThird)
        {
            drive_robot(0.1, -0.1);
            //ROS_INFO("RIGHT");
        }
        // Depending on the white ball position, call the drive_bot function and pass velocities to it
    }
  	else 
    {
            // Request a stop when there's no white ball seen by the camera
        drive_robot(0.0, 0.0);
        //ROS_INFO("STOP");
    }
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
