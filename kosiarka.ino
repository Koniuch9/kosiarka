#include <Servo.h>
#include <TridentTD_7Segs74HC595.h>

// * KONSTANTY *
#define SERVO_MIN_ANGLE 45
#define SERVO_MAX_ANGLE 135 
#define GAZ_MIN 20
#define GAZ_MAX 1000
#define MIN_RPM 1000
#define REFRESH_RATE 500

//* PINY *
#define CZUJNIK 3
#define SCLK 5
#define RCLK 6
#define DIO 7
#define GAZ A1
#define ENCODER_A 2
#define ENCODER_B 9
#define ENCODER_BUTTON 10
#define SERVO 4

// * GLOBALSY *
TridentTD_7Segs74HC595  dsp( SCLK, RCLK, DIO, 1);
Servo gaz;
volatile byte rpm_set_level = 0;
volatile byte rpm_tick_counter = 0;
bool mode = false; //true - zdalne, false - sterowanie samemu 
int angle = 0;
long prev_time = 0;
volatile bool setChanged = false;

void enc()
{
    if(digitalRead(ENCODER_B))
      if(rpm_set_level < 255)rpm_set_level++;
    else
      if(rpm_set_level > 0)rpm_set_level--; 
    setChanged = true;   
}
 
void tick()
{
    rpm_tick_counter++;
}

void setup()
{  
  pinMode(CZUJNIK,INPUT_PULLUP);  
  pinMode(GAZ,INPUT);
  pinMode(ENCODER_A,INPUT_PULLUP);
  pinMode(ENCODER_B,INPUT_PULLUP);
  pinMode(ENCODER_BUTTON,INPUT);
  
  dsp.addModule("rpm_set");
  //dsp.addModule("rpm_real");
  
  dsp.init();
  dsp.setNumber("rpm_set",MIN_RPM,0);
  attachInterrupt(digitalPinToInterrupt(CZUJNIK),tick,FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A),enc,FALLING);
  gaz.attach(SERVO);  
}

void loop() 
{   
  if(setChanged)
  {        
    setChanged = false;
    dsp.setNumber("rpm_set", MIN_RPM + rpm_set_level*25,0);  
  }
  if(mode){
      
  } else {      
    angle = analogRead(GAZ);
    angle = map(angle,GAZ_MIN,GAZ_MAX,SERVO_MIN_ANGLE,SERVO_MAX_ANGLE);
    gaz.write(angle);        
    delay(15);
  }
}
