#include <ros.h>
#include <Wire.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <sabertooth_ctrl/msg/MotorStr.h>

#define TA 8 // transmition address
#define P_S 6 // Pin select low for left high for right

ros::NodeHandle nh;

void Send_To_Motors(int LEFT, int RIGHT);                 // sends ROS commands to motors thru slave device (arduino)
void motor_cmd_str_callback(const std_msgs::String& msg); // processes the differential wheel velocity messages to values within (-127, 127)

int ch3Value = 0;
int ch2Value = 0;
int receivedInteger;


void setup()
{
  nh.initNode();
  nh.subscribe(sub);
  Serial.begin(9600);
  pinMode(P_S, OUTPUT);
  Wire.begin(); // join I2C bus (address optional for master)

}

void loop()
{
  // continuously send current velocity commands to wheel-driving Arduino
  Send_To_Motors(ch3Value, ch2Value);
  nh.spinOnce();
  delay(100);

  if (Serial.available() >= 8) {  // Adjust the number of bytes based on the data size
    int32_t received_data[2];

    // Read the binary data into an array of 32-bit integers
    Serial.readBytes((char*)received_data, 8);

    // Unpack and print the received integers
    Serial.print("Received Integers: ");
    cmd_left = received_data[0]
    cmd_right = received_data[1]

    ch3Value = 1.27 * cmd_left;
    ch2Value = 1.27 * cmd_right;

    Serial.print("Left velocity: "+" "+cmd_left+ "  "+"Right velocity: "+" "+cmd_right)

    Send_To_Motors(ch3Value, ch2Value);
  }
}


void Send_To_Motors(int LEFT, int RIGHT)
{
  int motor1_mapped_speed = map(LEFT, -127, 127, 0, 255);
  int motor2_mapped_speed = map(RIGHT, -127, 127, 0, 255);
  Wire.beginTransmission(TA);
  Wire.write(motor1_mapped_speed);
  Wire.write(motor2_mapped_speed);
  Wire.endTransmission();
  delay(10);
}
