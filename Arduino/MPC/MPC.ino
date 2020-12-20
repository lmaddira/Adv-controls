/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-12 14:51:36
 * @LastEditTime: 2019-10-11 16:39:57
 * @LastEditors: Please set LastEditors
 */
#include <Arduino.h>
#include "PinChangeInt.h"
#include "Pins.h"
#include "mode.h"
#include "BalanceCar.h"
#include <Wire.h>
#include "KalmanFilter.h"

class KalmanFilter;

int x_cord;
unsigned long start_prev_time = 0;
boolean carInitialize_en = true;
bool readM = true;
bool readC = false;

// Self defined
int a;
int input;
//
float cp = 0.0;

unsigned long startTime_vel = millis();


float num_of_encoder_counts_per_rev = 780.0;
float thousand_by_num_of_encoder_counts_per_rev = 1000.0/num_of_encoder_counts_per_rev;


float velocity = 0;
float distance = 0;
float angular_velocity= 0;
float motor_ang_vel = 0;

//



void functionMode()
{
  switch (function_mode)
  {
  case IDLE:
    break;
  default:
    break;
  }
}

void setMotionState_manual()
{
  switch (motion_mode)
  {
  case FORWARD:
    setting_car_speed = 20;
    setting_turn_speed = 0;
    break;
  case BACKWARD:
      setting_car_speed = -20;
      setting_turn_speed = 0;
    break;
  case TURNLEFT:
      setting_car_speed = 0;
      setting_turn_speed = 20;
    break;
  case TURNRIGHT:
      setting_car_speed = 0;
      setting_turn_speed = -20;
    break;
  case STANDBY:
    setting_car_speed = 0;
    setting_turn_speed = 0;
    break;
  case STOP:
    if (millis() - start_prev_time > 1000)
    {
      function_mode = IDLE;
      if (balance_angle_min <= kalmanfilter_angle && kalmanfilter_angle <= balance_angle_max)
      {
        motion_mode = STANDBY;
      }
    }
    break;
  case START:
    if (millis() - start_prev_time > 2000)
    {
      if (balance_angle_min <= kalmanfilter_angle && kalmanfilter_angle <= balance_angle_max)
      {
        car_speed_integeral = 0;
        setting_car_speed = 0;
        motion_mode = STANDBY;
      }
      else
      {
        motion_mode = STOP;
        carStop();
      }
    }
    break;
  default:
    break;
  }
}


void setMotionState_correction()
{
  switch (motion_mode)
  {
  case FORWARD:
    setting_car_speed = 5;
    setting_turn_speed = 0;
    break;
  case BACKWARD:
      setting_car_speed = -5;
      setting_turn_speed = 0;
    break;
  case TURNLEFT:
      setting_car_speed = 0;
      setting_turn_speed = 15;
    break;
  case TURNRIGHT:
      setting_car_speed = 0;
      setting_turn_speed = -15;
    break;
  case STANDBY:
    setting_car_speed = 0;
    setting_turn_speed = 0;
    break;
  case STOP:
    if (millis() - start_prev_time > 1000)
    {
      function_mode = IDLE;
      if (balance_angle_min <= kalmanfilter_angle && kalmanfilter_angle <= balance_angle_max)
      {
        motion_mode = STANDBY;
      }
    }
    break;
  case START:
    if (millis() - start_prev_time > 2000)
    {
      if (balance_angle_min <= kalmanfilter_angle && kalmanfilter_angle <= balance_angle_max)
      {
        car_speed_integeral = 0;
        setting_car_speed = 0;
        motion_mode = STANDBY;
      }
      else
      {
        motion_mode = STOP;
        carStop();
      }
    }
    break;
  default:
    break;
  }
}


void setMotionState_mpc(float input)
{
  switch (motion_mode)
  {
  case FORWARD:
    setting_car_speed = input;
    setting_turn_speed = 0;
    break;
  case BACKWARD:
      setting_car_speed = input;
      setting_turn_speed = 0;
    break;
  case STANDBY:
    setting_car_speed = 0;
    setting_turn_speed = 0;
    break;
  case STOP:
    if (millis() - start_prev_time > 1000)
    {
      function_mode = IDLE;
      if (balance_angle_min <= kalmanfilter_angle && kalmanfilter_angle <= balance_angle_max)
      {
        motion_mode = STANDBY;
      }
    }
    break;
  case START:
    if (millis() - start_prev_time > 2000)
    {
      if (balance_angle_min <= kalmanfilter_angle && kalmanfilter_angle <= balance_angle_max)
      {
        car_speed_integeral = 0;
        setting_car_speed = 0;
        motion_mode = STANDBY;
      }
      else
      {
        motion_mode = STOP;
        carStop();
      }
    }
    break;
  default:
    break;
  }
}


void keyEventHandle_correction(int x)
{
  if ((x-39)<-5)
  {
    motion_mode = TURNLEFT;
  }
  else if ((x-39)>5) 
  {
    motion_mode = TURNRIGHT;
  }
  else if ((x-39)<=5 && (x-39)>=-5)
    {
    motion_mode = FORWARD;
  }
}

void keyEventHandle_manual()
{
  char character;
  character= Serial.read();
  if(character=='W' || character == 'w'){
    motion_mode = FORWARD;    
  }
  else if (character=='A'|| character == 'a'){
    motion_mode = TURNLEFT;    
  }
  else if (character=='D'|| character == 'd'){
    motion_mode = TURNRIGHT;    
  }
  else if (character=='S'|| character == 's'){
    motion_mode = BACKWARD;    
  }
  else if (character == 'Q' || character == 'q')
  {
    motion_mode = STANDBY;
  }
}


void keyEventHandle_mpc(float input)
{

  if(input>0){
    motion_mode = FORWARD;    
  }
//  else if (character=='A'|| character == 'a'){
//    motion_mode = TURNLEFT;    
//  }
//  else if (character=='D'|| character == 'd'){
//    motion_mode = TURNRIGHT;    
//  }
  else if (input<0){
    motion_mode = BACKWARD;    
  }
//  else if (character == 'Q' || character == 'q')
//  {
//    motion_mode = STANDBY;
//  }
  else{
    motion_mode = STANDBY;
  }
}


void setup()
{
  Serial.begin(9600);
  start_prev_time = millis();
  carInitialize();
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9);
  Wire.onReceive(receiveEvent); 

}

void receiveEvent(int bytes) {
  x_cord = Wire.read();    // read one character from the I2C
}

void update_states()
{ 
  // Encoder Calculations
  // angular velocity = (encoder_reading/num_of_counts_per_rotation)*(2*pi/sampling_time)
  
  float car_speed = float(left_encoder + right_encoder)/2.0*(input>0?1:-1);
  
  motor_ang_vel = (float) 2 * 3.1415 * car_speed * (float)thousand_by_num_of_encoder_counts_per_rev / (float)(millis() - startTime_vel);  
 
  
  velocity = (motor_ang_vel)*0.0335;
  distance = distance + velocity*(double(millis()-startTime_vel))/1000.0;
  angular_velocity = kalmanfilter.Gyro_x*(PI/180.0);

  startTime_vel = millis();
    
  left_encoder = 0;
  right_encoder = 0;

}

void loop()
{ 

  if (Serial.available() > 0) {
                a = Serial.parseInt();// read the incoming data as string
                input = a;
                //input = a.toFloat();
                }
 
  input = constrain(input,-20,20);
 keyEventHandle_mpc(input);
 setMotionState_mpc(input);
  

  
  
  //setMotionState();
  functionMode();
  static unsigned long print_time = 0;
  if (millis() - print_time > 100)
  {
    

update_states();
    
  String zero = String(distance,DEC);
  String one = String(velocity, DEC);
  String two = String(kalmanfilter.angle * (PI/180.0), DEC);
  String three = String(angular_velocity * (PI/180.0), DEC);
  String four = String(input,DEC);
  
  String delim = String('$');
  String delim2 = String('#');

  String bot_in = zero+delim+one+delim+two+delim+three+delim+four+delim2;
  Serial.println(bot_in);
  //Serial.println("\n");//bot_in);
  }
  
  static unsigned long start_time;
  if (millis() - start_time < 10)
  {
    function_mode = IDLE;
    motion_mode = STOP;
    carStop();
  }
  if (millis() - start_time == 2000) // Enter the pendulum, the car balances...
  {
    motion_mode = START;
  }
  
  
}
