#include <Wire.h>
#include <Pixy2.h>

Pixy2 pixy;

// Include the required Wire library for I2C<br>#include 

void setup() {
  // Start the I2C Bus as Master
  Wire.begin(); 
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pixy.init();
  Serial.println(pixy.changeProg("line"));
}

void loop() {

  int8_t x_cord, x_cord_last;
  int8_t res;
  char buf[128];
 
  res = pixy.line.getMainFeatures();

  if (res&LINE_VECTOR)
  {
    x_cord = pixy.line.vectors->m_x1;
    x_cord_last = x_cord;
    Serial.println(x_cord);
  } 
  else
  {
    if (x_cord_last>44);
    {
      x_cord = 78;
    }
    if (x_cord_last < 34);
    {
      x_cord = 1;
    }
    //x_cord = 39;
    Serial.println(x_cord);
  }

  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(x_cord);              // sends x 
  Wire.endTransmission();    // stop transmitting
  delay(500);
}
