#include <TridentTD_7Segs74HC595.h>

// * KONSTANTY *

#define MIN_RPM 1000
#define REFRESH_RATE 333

#define RESOLUTION 25

//* PINY *
#define CZUJNIK 3
#define SCLK 5
#define RCLK 6
#define DIO 7
#define ENCODER_A 2
#define ENCODER_B 4
#define ENCODER_BUTTON 10

// * GLOBALSY *
TridentTD_7Segs74HC595  dsp( SCLK, RCLK, DIO, 2);
volatile byte rpm_set_level = 0;
volatile byte rpm_tick_counter = 0;
bool mode = false; //true - zdalne, false - sterowanie samemu 
int angle = 0;
int rpm = 0;
volatile long prev_time = 0;
volatile long sumTime = 0;
volatile long encTime;
volatile long diffTime;
volatile bool setChanged = false;

unsigned volatile long lastTime = 0;
unsigned volatile long diffRpm = 0;

void enc()
{  
    encTime = millis();
    if((diffTime = encTime - prev_time) >= 90){
    if(!digitalRead(ENCODER_B))
      rpm_set_level += diffTime<180?4:1;    
    else
      rpm_set_level -= diffTime<180?4:1;     
    prev_time = encTime;
    }
    setChanged = true;
}
 
void tick()
{
    unsigned long timeNow = millis();
    diffRpm = timeNow - lastTime;
    lastTime = timeNow;
    rpm_tick_counter++;
    sumTime += diffRpm;
}


void setup()
{  
  pinMode(CZUJNIK,INPUT_PULLUP);  
  pinMode(ENCODER_A,INPUT_PULLUP);
  pinMode(ENCODER_B,INPUT_PULLUP);
  //pinMode(ENCODER_BUTTON,INPUT);
  
  dsp.addModule("Rpmreal");
  dsp.addModule("Rpmset");
  
  dsp.init();
  
  attachInterrupt(digitalPinToInterrupt(CZUJNIK),tick,FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A),enc,FALLING); 
}
byte x = 0;
unsigned long refreshTime = 0;
int lastGaz = 0;
bool flagServo = true;
int servoCounter = 0;
void loop() 
{          
        if(rpm_set_level != x)
        {
          x = rpm_set_level;
          dsp.setNumber("Rpmset",MIN_RPM+rpm_set_level*RESOLUTION,0);              
        }

        if(millis() - refreshTime >= REFRESH_RATE)
        {
          noInterrupts();
          if(sumTime == 0 || rpm_tick_counter == 0)
          {
            rpm = 0;  
          }
          else{
            float avgTime = (float)sumTime/(float)rpm_tick_counter;
            rpm = (int)(60000.0/avgTime)/RESOLUTION*RESOLUTION;
          }
          rpm_tick_counter = 0;
          sumTime = 0;          
          interrupts();
          dsp.setNumber("Rpmreal",rpm,0);
          refreshTime = millis();  
        }       
        
}
