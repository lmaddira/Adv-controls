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


unsigned long start_prev_time = 0;
boolean carInitialize_en = true;

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

void setMotionState()
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
      setting_turn_speed = 50;
    break;
  case TURNRIGHT:
      setting_car_speed = 0;
      setting_turn_speed = -50;
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

void keyEventHandle()
{
  char character;
  if(Serial.available()){ // verify RX is getting data
  character= Serial.read();
  if(character=='W'){
    motion_mode = FORWARD;    
  }
  else if (character=='A'){
    motion_mode = TURNLEFT;    
  }
  else if (character=='D'){
    motion_mode = TURNRIGHT;    
  }
  else if (character=='S'){
    motion_mode = BACKWARD;    
  }
//  else {
//    motion_mode = STANDBY;
//  }
 }
}
  


void setup()
{
  Serial.begin(9600);
  start_prev_time = millis();
  carInitialize();
}

void loop()
{
  keyEventHandle();
  setMotionState();
  functionMode();
  static unsigned long print_time;
  if (millis() - print_time > 100)
  {
    print_time = millis();
    Serial.print(kalmanfilter.angle);
    Serial.print('\t');
    Serial.print(balance_control_output);
    Serial.print('\t');
    Serial.print(speed_control_output);
    Serial.print('\t');
    Serial.print(rotation_control_output);
    Serial.print('\t');
    Serial.print(motion_mode);
    Serial.print('\t');
    Serial.print(setting_car_speed);
    Serial.println();
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
