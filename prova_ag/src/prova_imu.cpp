//il dato proveniente dall'imu lo considero come angolazione assoluta dalla posa iniziale e non come succssione di incrementi
#include "ros/ros.h"
#include <sstream>
#include "geometry_msgs/PoseStamped.h"
#include "sensor_msgs/Imu.h"
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>

void inizia();
void log();
void odometria_imu();

//published data
geometry_msgs::Pose pose;
geometry_msgs::Point point;

//imu data
float or_x,or_y,or_z,or_w,acc_x,acc_y,acc_z;
float angle;
float alfa=1;

//encoder data
float D=0;
float S=0;

float Psy=0;//qui devo inserire l'orientazione iniziale,se la lascio zero è ok solo se parte dal punto di inizio dei test QR+IMU
float Psy0=0;
float Psy1=0;

float odom_D=0;
float odom_S=0;

float deltaC=0;
float Cm=0.000261799;//m passo encoder
float b=0.54;//m interasse tra ruote

float deltaD=0;
float deltaS=0;

//Dir data
char dir[1];

float X=0;
float Y=0;
float dX=0;
float dY=0;

float dXa,dYa,Xa,Ya;

int indS=0;
int indD=0;
//posizione iniziale
float Xa0=0;
float Ya0=0;
float theta=0;

int start=0;

//////////////
void DCallback(const std_msgs::Float32& msg)
{	
	odom_D=msg.data;
	indD++;
}

void SCallback(const std_msgs::Float32& msg)
{
	odom_S=msg.data;
	indS++;
}

void DirCallback(const std_msgs::String& msg)
{
	dir[0]=msg.data[0];
}

void imuPoseCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
	or_x=msg->orientation.x;
	or_y=msg->orientation.y;
	or_z=msg->orientation.z;
	or_w=msg->orientation.w;
	acc_x=msg->linear_acceleration.x;
	acc_y=msg->linear_acceleration.y;
	acc_z=msg->linear_acceleration.z;	
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "encoder_imu");
  ros::NodeHandle n_encoder_imu;
  ros::Subscriber subD = n_encoder_imu.subscribe("encoder_D", 1000, DCallback);
  ros::Subscriber subS = n_encoder_imu.subscribe("encoder_S", 1000, SCallback);
  ros::Subscriber subDir = n_encoder_imu.subscribe("DIR", 1000, DirCallback);
  ros::Subscriber subImu = n_encoder_imu.subscribe("/imu/data", 1000, imuPoseCallback);
  ros::Publisher pose_Publisher = n_encoder_imu.advertise<geometry_msgs::Pose>("/estimated_imu_odom_pose/Pose", 10);
  ros::Rate loop_rate(1);
  inizia();
 
 while (ros::ok())
 {

	ros::spinOnce();

	angle=atan2f(2*(or_x*or_y+or_z*or_w),1-2*(or_y*or_y+or_z*or_z));

	if(angle!=0){	
			if(start<=3)start++;
			odometria_imu();}

	point.x=Xa;
	point.y=Ya;
	point.z=0;
	pose.position=point;
	pose_Publisher.publish(pose);
	ROS_INFO("posizione X:%f  Y:%f",Xa,Ya);
	log();
	loop_rate.sleep() ;
	
}
  return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
void inizia(){
	
	char s[30];
	float num[3];
 			
	/*FILE *log;	
	log=fopen("/home/giovanni/catkin_ws/src/test/prova_ag/src/pos.txt","r");

	fgets(s,10,log);
	num[1]=atof(s);
				
	fgets(s,10,log);
	num[2]=atof(s);
		
	fgets(s,10,log);
	num[3]=atof(s);
		
	fclose(log);
*/
	Xa0=num[1];
	Ya0=num[2];
	theta=num[3];

	ROS_INFO("pos. iniziale X:%f  Y:%f  theta:%f",Xa0,Ya0,theta);

	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void log(){

		FILE *logq;	
		logq=fopen("/home/giovanni/Scrivania/log/encoder_imu/X.txt","a");	
		fprintf(logq," %f\n",Xa);
		fclose(logq);

		FILE *logq1a;	
		logq1a=fopen("/home/giovanni/Scrivania/log/encoder_imu/Y.txt","a");	
		fprintf(logq1a," %f\n",Ya);
		fclose(logq1a);
		
		FILE *logq1;	
		logq1=fopen("/home/giovanni/Scrivania/log/encoder_imu/PSY.txt","a");	
		fprintf(logq1," %f\n",Psy);
		fclose(logq1);		

return;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void odometria_imu(){

	if(start==1){Psy0=angle;}
	Psy1=angle;
//spostamento percorso dalla singola ruota
	deltaD=Cm*(odom_D);
	deltaS=Cm*(odom_S);
//spostamento percorso dal centro
	deltaC=(deltaD+deltaS)/2;
//controllo di stop per quando lavoro offline su rosbag
if(indD==0 || indS==0){
deltaD=0;
deltaS=0;
deltaC=0;}
////////////////////////////////////////////////////////////////
	dX=deltaC*cos(Psy);
	dY=deltaC*sin(Psy);

	X=X+dX;
	Y=Y+dY;	
//angolo di orientazione nuov
	Psy=Psy1-Psy0;
//matrice di rotazione
	dXa=X*cos(theta)-Y*sin(theta);
	dYa=X*sin(theta)+Y*cos(theta);
//posa in riferimento globale
	Xa=Xa0+dXa;
	Ya=Ya0+dYa;	

	indD=0;
	indS=0;
return;
}
