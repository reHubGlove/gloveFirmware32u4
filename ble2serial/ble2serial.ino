#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include <Wire.h>
#include <MPU6050k.h>

MPU6050 mpu;
/**************************************************/

#define LEFT
//#define RIGHT

//#define CALIBRATION
#define USERMPU

/**************************************************/
                      //   LEFT      // RIGHT
#define ThPos     5   // 200 - 130   250 - 130
#define ThPosRol  11  // 220 - 180   240 - 220
#define ThFlex    A8  
#define InFlex    A11 // 700 - 370   800 - 450
#define MiFlex    A4  // 790 - 460   785 - 400
#define RiFlex    A2  // 700 - 400   740 - 400
#define LiFlex    A0  // 400 - 180   380 - 180

#define ThPress   A9  // 0 - 1023
#define InPress   A10 // 0 - 1023
#define MiPress   A5  // 0 - 1023
#define RiPress   A3  // 0 - 1023
#define LiPress   A1  // 0 - 1023

/**************************************************/

//unsigned long timer = 0;
float timeStep        = 0.01;

float pitchG    = 0;
float rollG     = 0;
float yawG      = 0;
int pitchA      = 0;
int rollA       = 0;

float pitch     = 0; // -35 - 45 
float roll      = 0; // -65 - 40
float yaw       = 0; // -8 - 8

int thumb       = 0;
int thumbrol    = 0;
int index       = 0;
int middle      = 0;
int ring        = 0;
int little      = 0;

int thumbpad    = 0;
int indexpad    = 0;
int middlepad   = 0;
int ringpad     = 0;
int littlepad   = 0;

/**************************************************/

#ifdef LEFT
  char gType  = 'l';
#endif
  
#ifdef RIGHT
  char gType  = 'r';
#endif

/**************************************************/

void setup() {
  Serial.begin(115200);
  
  #ifdef CALIBRATION
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  #endif
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite( LED_BUILTIN,1 );

  pinMode(ThPos, OUTPUT);
  pinMode(ThPosRol, OUTPUT);

  digitalWrite( ThPos,0 );
  digitalWrite( ThPosRol,0 );
  
  #ifdef USERMPU
    while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))  {
      Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
      delay(500);
    }
    mpu.calibrateGyro();
    mpu.setThreshold(3);
  #endif
  
  Serial.print( "Set Name:" );
  
  digitalWrite( LED_BUILTIN,0 );
  delay( 500 );
  
  #ifdef LEFT
    ble_set_name("LeftGlove");
    Serial.println( "LeftGlove" );
  #endif
  
  #ifdef RIGHT
    ble_set_name("RightGlove");
    Serial.println( "RightGlove" );
  #endif
  
  ble_begin();
  delay( 2000 );
  Serial.println( "Setup Success !!\n\n" );
}

/**************************************************/

void loop() {
  //timer = millis();
  
  ble_do_events();
  #ifdef USERMPU
    Vector gyro  = mpu.readNormalizeGyro();
    Vector accel = mpu.readNormalizeAccel();
  #endif
  //pitchA = -(atan2(accel.XAxis, sqrt(accel.YAxis*accel.YAxis + accel.ZAxis*accel.ZAxis))*180.0);
  //rollA = (atan2(accel.YAxis, accel.ZAxis)*180.0);
  //pitchG = pitchG + gyro.YAxis * timeStep;
  //rollG = rollG + gyro.XAxis * timeStep;
  //yawG = yawG + gyro.ZAxis * timeStep;

  /** NEW **/
  #ifdef USERMPU
    pitchA = accel.XAxis;
    rollA  = accel.YAxis;
    yawG   = yawG + gyro.ZAxis * timeStep;
  #endif
  /*
  float pitch = pitch + ((pitchA+pitchG)/2);
  float roll  = roll + ((rollA+rollG)/2);
  float yaw   = yaw + yawG;
  */

  int pitch = (pitch + pitchA);
  int roll  = (roll + rollA);
  int yaw   = (yaw + yawG);
  
  pitch = map(pitch,10,-10,0,165);
  roll  = map(roll,9,-9,0,165);
  yaw   = map(yaw,8,-8,0,165);
  
  getFlexSensor();
  getPressSensor();
  
  #ifdef CALIBRATION

  Serial.print( "Pitch: " ); Serial.println( pitch );
  Serial.print( "Roll : " ); Serial.println( roll );
  Serial.print( "Yaw  : " ); Serial.println( yaw );

  Serial.print( "Thumb: " ); Serial.println( thumb );
  Serial.print( "ThuRo: " ); Serial.println( thumbrol );
  Serial.print( "Index: " ); Serial.println( index );
  Serial.print( "Middl: " ); Serial.println( middle );
  Serial.print( "Ring : " ); Serial.println( ring );
  Serial.print( "Littl: " ); Serial.println( little );

  Serial.print( "ThuPa: " ); Serial.println( thumbpad );
  Serial.print( "IndPa: " ); Serial.println( indexpad );
  Serial.print( "IndPa: " ); Serial.println( middlepad );
  Serial.print( "RinPa: " ); Serial.println( ringpad );
  Serial.print( "LitPa: " ); Serial.println( littlepad );

  Serial.println( "-------------------" );
  Serial.println( "\n" );

  #endif
  
  if(ble_connected()){
    
    unsigned char myData[15] = {gType,pitch,roll,yaw,thumb,thumbrol,index,middle,ring,little,thumbpad,indexpad,middlepad,ringpad,littlepad};
    ble_write_bytes( myData,15 );
    
    pinMode(13, true);
    digitalWrite(13, true); 
  }
  else{
    pinMode(13, false);
    digitalWrite(13, false);
  }
  //delay((timeStep*1000) - (millis() - timer));
}

/**************************************************/

void ble_print(char *str){
  char i = 0;
  while(char c = str[i]){
    ble_write(str[i]);
    i++;
  }
}

/**************************************************/

char buf[12];
void ble_printInt(int num){
  ble_print(itoa(num, buf, 10));
}

/**************************************************/

void getFlexSensor() {
  digitalWrite( ThPos,1 );
  digitalWrite( ThPosRol,0 );
  delay(50);
  thumb = analogRead(ThFlex);
  int thumbl = map((thumb > 200) ? 200:thumb,200,130,10,220);
  int thumbr = map((thumb > 250) ? 250:thumb,250,130,10,220);
  digitalWrite( ThPos,0 );
  digitalWrite( ThPosRol,1 );
  delay(50);
  thumbrol  = analogRead(ThFlex);
  int thumbroll  = map((thumbrol > 220) ? 220:thumbrol,220,180,5,25);
  int thumbrolr  = map((thumbrol > 240) ? 240:thumbrol,240,220,5,25);
  digitalWrite( ThPos,0 );
  digitalWrite( ThPosRol,0 );
  
  index     = analogRead(InFlex);
  middle    = analogRead(MiFlex);
  ring      = analogRead(RiFlex);
  little    = analogRead(LiFlex);

  #ifdef CALIBRATION
    #ifdef LEFT
      thumbl     = thumb;
      thumbroll  = thumbrol;
    #endif
    #ifdef RIGHT
      thumbr     = thumb;
      thumbrolr  = thumbrol;
    #endif
  #endif

  #ifdef LEFT
    thumb     = thumbl;
    thumbrol  = thumbroll;
    index     = map((index  > 700) ? 800:index, 800,370,10,220);
    middle    = map((middle > 790) ? 790:middle,790,460,10,220);
    ring      = map((ring   > 700) ? 700:ring,  700,400,10,220);
    little    = map((little > 400) ? 400:little,400,180,10,220);
  #endif
  
  #ifdef RIGHT
    thumb     = thumbr;
    thumbrol  = thumbrolr;
    index     = map((index  > 720) ? 720:index, 720,410,10,220);
    middle    = map((middle > 750) ? 750:middle,750,380,10,220);
    ring      = map((ring   > 740) ? 740:ring,  740,400,10,220);
    little    = map((little > 370) ? 370:little,370,175,10,220);
  #endif

  #ifdef CALIBRATION
    index     = analogRead(InFlex);
    middle    = analogRead(MiFlex);
    ring      = analogRead(RiFlex);
    little    = analogRead(LiFlex);
  #endif
}

/**************************************************/

void getPressSensor() {
  thumbpad    = map(analogRead(ThPress),0,1023,0,100);
  indexpad    = map(analogRead(InPress),0,1023,0,100);
  middlepad   = map(analogRead(MiPress),0,1023,0,100);
  ringpad     = map(analogRead(RiPress),0,1023,0,100);
  littlepad   = map(analogRead(LiPress),0,1023,0,100);

  #ifdef CALIBRATION
    thumbpad    = analogRead(ThPress);
    indexpad    = analogRead(InPress);
    middlepad   = analogRead(MiPress);
    ringpad     = analogRead(RiPress);
    littlepad   = analogRead(LiPress);
  #endif
}

/**************************************************/
