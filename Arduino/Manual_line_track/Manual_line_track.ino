
#include <Arduino.h>
#include "PinChangeInt.h"
#include "Pins.h"
#include "mode.h"
#include "BalanceCar.h"
#include <Wire.h>

int x_cord;
String randi;
unsigned long start_prev_time = 0;
boolean carInitialize_en = true;
bool readM = true;
bool readC = false;

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

  randi = Wire.read();
  Serial.println(randi);
}

void loop()
{ 
  char character;
  character= Serial.read();

  if (character == 'M' || character == 'm' || readM)
  {
    readC = false;
    readM = true;
    keyEventHandle_manual();
    setMotionState_manual();
  }
  if (character == 'C' || character == 'c' || readC)
  {
    readC =true;
    readM = false;
    keyEventHandle_correction(x_cord);
    setMotionState_correction();
  }

  
  

  functionMode();
  static unsigned long print_time;
  if (millis() - print_time > 100)
  {
    

    print_time = millis();

    
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
