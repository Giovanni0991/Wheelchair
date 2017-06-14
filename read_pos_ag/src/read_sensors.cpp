#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/sensors/ImuSensor.hh>
#include <iostream>
#include <fstream>
// Gazebo's API has changed between major releases. These changes are
// accounted for with #if..#endif blocks in this file.
#if GAZEBO_MAJOR_VERSION < 6
#include <gazebo/gazebo.hh>
#else
#include <gazebo/gazebo_client.hh>
#endif

using namespace std;

//Listen to Gazebo imu info
void imuCallback(ConstIMUPtr &_msg);
//Listen to ROS scan info
void scanCallback(const sensor_msgs::LaserScan::ConstPtr &_msg);

ofstream imufile;	//variabile globale per memorizzare i dati imu
ofstream scanfile;	//variabile globale per momorizzare i dati dell'hokuyo

int main(int argc, char** argv)
{
	//Inizializzazione del nodo ROS di nome "read_imu"
	ros::init(argc, argv, "read_sensors");
	//creazione dell'oggetto nodo per comunicare con ROS system
	ros::NodeHandle node_obj;
	ros::Subscriber scan_sub = node_obj.subscribe("/scan",1,scanCallback);
//###################################################################
	// Load gazebo as a client
	#if GAZEBO_MAJOR_VERSION < 6
  	gazebo::setupClient(argc, argv);
	#else
  	gazebo::client::setup(argc, argv);
	#endif
	// Create our node for communication
  	gazebo::transport::NodePtr node(new gazebo::transport::Node());
	node->Init();
	// Subscriber to the  velodyne topic 
	gazebo::transport::SubscriberPtr imu_sub = node->Subscribe("~/wchair/base_link/imu_sensor/imu", imuCallback);
//####################################################################
	//aprimo il file per scrivere i dati dalla imu in modalità scrittura|append
    imufile.open("src/ros_wheelchair/read_pos_ag/imuData.txt", ios::out | ios::app);
    scanfile.open("src/ros_wheelchair/read_pos_ag/scanData.txt", ios::out | ios::app);
    if(imufile.is_open() && scanfile.is_open())
    {
    	// Busy wait loop...replace with your own code as needed.
		while(ros::ok())
		{
			gazebo::common::Time::MSleep(1000);
			//aggiorna tutti i topic ROS, e richiama le callback
			ros::spin();
		}
		
    }else{
    		cout<<"Errore nell'apertura dei file";
    }
    //chiudiamo i files 
    imufile.close();
    scanfile.close();
//####################################################################
	 // Make sure to shut everything down.
	#if GAZEBO_MAJOR_VERSION < 6
  	gazebo::shutdown();
	#else
  	gazebo::client::shutdown();
	#endif
//#################################################################### 
 return 0 ;
}

// Function is called every time a message is received.
void imuCallback(ConstIMUPtr &_msg)
{	
	/*//cout << _msg->DebugString();
	//ORIENTATION - Quaternion Math
    double o0 = (double)(_msg->orientation().x());
    double o1 = (double)(_msg->orientation().y());
    double o2 = (double)(_msg->orientation().z());
    double o3 = (double)(_msg->orientation().w());
    
    //ANGULAR_VELOCITY
    double av0 = (double)(_msg->angular_velocity().x());
    double av1 = (double)(_msg->angular_velocity().y());
    double av2 = (double)(_msg->angular_velocity().z());
    
    //LINEAR_ACCELERATION
    double la0 = (double)(_msg->linear_acceleration().x());
    double la1 = (double)(_msg->linear_acceleration().y());
    double la2 = (double)(_msg->linear_acceleration().z());
    
	imufile<<"Orientation:\n";
    imufile<<"\tx: "<<o0<<" , y: "<<o1<<" , z: "<<o2<<" , w: "<<o3<<"\n\n";
    imufile<<"Angular_Velocity:\n";
    imufile<<"\tx: "<<av0<<" , y: "<<av1<<" , z: "<<av2<<"\n\n";
    imufile<<"Linear_Acceleration:\n";
    imufile<<"\tx: "<<la0<<" , y: "<<la1<<" , z: "<<la2<<"\n\n";
    cout<<"Orientation:\n";
    cout<<"\tx: "<<o0<<" , y: "<<o1<<" , z: "<<o2<<" , w: "<<o3<<"\n\n";
    cout<<"Angular_Velocity:\n";
    cout<<"\tx: "<<av0<<" , y: "<<av1<<" , z: "<<av2<<"\n\n";
    cout<<"Linear_Acceleration:\n";
    cout<<"\tx: "<<la0<<" , y: "<<la1<<" , z: "<<la2<<"\n\n";
    imufile<<"____________________________________________________________________\n\n";
    */
    imufile<<_msg->DebugString();
}
void scanCallback(const sensor_msgs::LaserScan::ConstPtr &_msg)
{
	scanfile<<*_msg;
	//cout<<*_msg<<endl;
}
    

