#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <CustomDualG2HighPowerMotorShield.h>

ros::NodeHandle nh;

DualG2HighPowerMotorShield18v22 md;

double w_r=0, w_l=0;

//wheel_rad is the wheel radius ,wheel_sep is
double wheel_rad = 0.1397, wheel_sep = 0.725; // in meters

int lowSpeed = 100;
int highSpeed = 200;
double speed_ang=0, speed_lin=0;

void messageCb( const geometry_msgs::Twist& msg){
  speed_ang = msg.angular.z;
  speed_lin = msg.linear.x;
  w_r = (speed_lin/wheel_rad) + ((speed_ang*wheel_sep)/(2.0*wheel_rad));
  w_l = (speed_lin/wheel_rad) - ((speed_ang*wheel_sep)/(2.0*wheel_rad));
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );

void stopIfFault() {
  if (md.getM1Fault()) {
    md.disableDrivers();
    delay(1);
    while (1);
  }
  if (md.getM2Fault()) {
    md.disableDrivers();
    delay(1);
    while (1);
  }
  if (md.getM3Fault()) {
    md.disableDrivers();
    delay(1);
    while (1);
  }
  if (md.getM4Fault()) {
    md.disableDrivers();
    delay(1);
    while (1);
  }
}

void setup(){
  Serial.begin(115200);
  
  md.init();
  // Uncomment to flip a motor's direction:
  md.flipM3(true);
  md.flipM4(true);

  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);
}

void loop(){
  md.enableDrivers();
  delay(1);
  
  md.setM1Speed(w_l*15); //BL
  md.setM2Speed(w_l*15); //FL
  md.setM3Speed(w_r*15); //FR
  md.setM4Speed(w_r*15); //BR
  stopIfFault();
  
  nh.spinOnce();
}
