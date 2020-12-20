# Adv-controls

#### Pre-requisites
MATLAB 2019Rb


### Overview
This  paper  presents  the  work  done  on  designingand  implementing  a  nonlinear  model  predictive  control  (MPC)based  controller  on  a  self-balancing  two  wheeled  mobile  robotfor a dynamic environment. The inverted pendulum model usedin  this  study  is  naturally  an  unstable  system  when  subjected  tonatural  disturbances  due  to  environmental  loading  conditions.This  model  has  been  studied  extensively  in  the  past  for  PDcontrollers  but  MPC  has  been  relatively  left  unexplored.  Inthis   paper   we   propose   various   controllers,   PD   controller,MPC   controller   as   feedback   and   MPC   reference   trackingfeed  forward  controller  with  a  inner  PD  controller,  to  drivethe  balancing  two  wheeled  robot  in  a  dynamic  environment.Simulation  results  demonstrate  the  feasibility  and  efficiency  ofour  proposed  design  and  experimental  results  on  the  hardwarevalidate the these results. A follower robot and obstacle detectionwas  added  in  the  final  demonstration  to  show  multi  roboticsinteraction.The  PD  controller  with  live  camera  feedback  wasable  to  demonstatet  robust  line  tracking,  obstacle  detection  andmulti-robot   interaction.The   MPC   based   self   balancing   robotwas  able  to  track  a  straight  line  with  full  state  feedback.

### Please read the report file to understand the project better 

### Instructions to run the files
1. nonlinear_plant.mlx contain all the nonlinear plant parameters required to run nonlinear_plant.slx Simulink model. nonlinear_plant.slx Simulink model contain both linear and nonlinear plant simulink models with a controller in loop to check for stability and reference tracking. Please run nonlinear_plant.mlx before and then run nonlinear_plant.slx to see simulation of output
2. Run MPC_feedback_position.mlx first and then MPC_feedback_position.slx to visualise the simscape nonlinear model behaviour. In this model we have position, veocity, tilt and rate of tilt as the statespace and here we track position using MPC feedback module.
3. Run MPC_feedback_velocity_ref.mlx first and then MPC_feedback_velocity.slx to visualise the simscape nonlinear model behaviour. In this model we have veocity, tilt and rate of tilt as the statespace and here we track velocity using MPC feedback module.
4. Run MPC_feedforward_position.mlx first and then MPC_feedforward_position.slx to visualise the simscape nonlinear model behaviour. In this model we have position, veocity, tilt and rate of tilt as the statespace and here we track position using MPC feedforward reference tracking with a innerloop PD controller module.
5. Run MPC_feedforward_velocity.mlx first and then MPC_feedforward_velocity.slx to visualise the simscape nonlinear model behaviour. In this model we have veocity, tilt and rate of tilt as the statespace and here we track position using MPC feedforward reference tracking with a innerloop PD controller module.
6. Position Tracking Feedback_linearplant.mlx implements feedback controller with MPC statespace as position,velocity,tilt and rate of tilt and additional PD controller for heading angle. Here we update the MPC with linearised plant equations instead of nonlinear plant.Please run the Position Tracking Feedback_linearplant.mlx to implement and visualise the tracking.
7. Position Tracking Feedforward_linearplant.mlx implements feedback controller with feedforward reference tracking MPC statespace as position,velocity,tilt and rate of tilt with inner loop PID controller and an additional PD controller for heading angle. Here we update the MPC with linearised plant equations instead of nonlinear plant.Please run the Position Tracking Feedback_linearplant.mlx to implement and visualise the tracking.
8. Refer to Arduino folder README.md to understand the implemtation of Aurdino and PI modules on Hardware


### Hardware
#### 1. Manual_line_track.ino
  (Please upload manual_line_track_slave.ino to the slave Nano first)
  (Upload Manual_line_track.ino to the master Nano. Set up hotspot on a mobile phone. On laptop open Putty and connect to the Raspberry Pi over the hotspot. This will open up     mini-com which the user can use to give manual commands.)
  There are two possible modes of operation of the robot.
  ##### 1.1. Manual Mode 
  When switched on for the first time, robot boots up in the manual mode. In this mode the robot moves as follows:
  1.1.a. Press 'W' or 'w' to move the robot forward.
  1.1.b. Press 'S' or 's' to move the robot in reverse.
  1.1.c. Press 'A' or 'a' to turn the robot left in its current position.
  1.1.d. Press 'D' or 'd' to turn the robot right in its current position.
  1.1.e. Press 'Q' or 'q' to balance the robot in its current position.
  


### Conclusions
In conclusion the Tumbller was able to balance and followlines  with  a  PD  controller  effectively.  For  the  final  demothe  team  was  able  to  incorporate  obstacle  detection  and  afollower  robot.  In  terms  of  MPC  the  robot  was  able  to  tracka  line  of  fixed  distance.  Full  robot  MPC  was  not  achieveddue  to  the  communication  delays  encountered  in  parsing  theincoming data stream. From the limited testing performed dueto restricted on campus access the robot had a wide range offunctionality.  This  aligned  with  the  team’s  goal  of  making  ascaled  version  of  a  self  balancing  multi-agent  robot  systemthat is able to function in as an automated guided vehicle.

### Future work
In retrospect, based on the communication delays observedin  running  the  paring  scripts  on  MATLAB,  the  controllerwould  have  been  better  implemented  if  run  locally  on  theRaspberry Pi either through MATLAB or an embedded scriptderived from MATLAB Coder. By generating C and C++ codefor the Raspberry Pi platform along with libraries for the IMUand Tumbller source code, a standalone version of the programcould be generated.







