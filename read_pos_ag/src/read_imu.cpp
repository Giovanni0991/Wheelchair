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

//Listen to Gazebo pose info topic
void imuCallback(ConstIMUPtr &_msg);

ofstream imufile;

int main(int argc, char** argv)
{
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
	gazebo::transport::SubscriberPtr sub = node->Subscribe("~/wchair/base_link/imu_sensor/imu", imuCallback);
	
    ofstream imufile;
    imufile.open("imuData.txt", ios::out);
    
    if(imufile.is_open())
    {
	  	cout<<"Apertura file";
    	// Busy wait loop...replace with your own code as needed.
		while(true)
			gazebo::common::Time::MSleep(10);
		
    }else{
    		cout<<"Errore nell'apertura";
    }
	 // Make sure to shut everything down.
	#if GAZEBO_MAJOR_VERSION < 6
  	gazebo::shutdown();
	#else
  	gazebo::client::shutdown();
	#endif
	imufile.close();
    cout<<"Chiusura file";
    
 return 0 ;
}

// Function is called every time a message is received.
void imuCallback(ConstIMUPtr &_msg)
{	
	cout<<"callback";
	//cout << _msg->DebugString();
	//ORIENTATION - Quaternion Math
    double o0 = (double)(_msg->orientation().x());
    double o1 = (double)(_msg->orientation().y());
    double o2 = (double)(_msg->orientation().z());
    double o3 = (double)(_msg->orientation().w());
    
    //ANGULAR_VELOCITY
    double av0 = (double)(_msg->angular_velocity().x());
    double av1 = (double)(_msg->angular_velocity().y());
    double av2 = (double)(_msg->angular_velocity().z());
    
    //LINEAR_VELOCITY
    double lv0 = (double)(_msg->linear_acceleration().x());
    double lv1 = (double)(_msg->linear_acceleration().y());
    double lv2 = (double)(_msg->linear_acceleration().z());
    
	imufile<<"Orientation \n";
    imufile<<"\tx: "<<o0<<"\n"<<"\ty: "<<o1<<"\n"<<"\tz: "<<o2<<"\n"<<"\tw: "<<o3<<"\n";
    imufile<<"______________________________________________________\n";    
}
    

