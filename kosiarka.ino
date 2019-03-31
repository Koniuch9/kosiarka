#include <TridentTD_7Segs74HC595.h>

// * KONSTANTY *
#define MIN_RPM 1000
#define REFRESH_RATE 500

#define RESOLUTION 50

//* PINY *
const int CZUJNIK = A1;
#define SCLK 5
#define RCLK 6
#define DIO 8
#define ENCODER_A 2
#define ENCODER_B 4
#define ENCODER_BUTTON 3

// * GLOBALSY *
TridentTD_7Segs74HC595  dsp( SCLK, RCLK, DIO, 2);
volatile byte rpm_set_level = 0;
volatile int rpm_tick_counter = 0;
bool mode = false; //true - zdalne, false - sterowanie samemu 
int angle = 0;
long rpm = 0;
volatile long prev_time = 0;
volatile long sumTime = 0;
volatile long encTime;
unsigned volatile long diffTime = 0;
//volatile bool setChanged = false;

unsigned volatile long lastTime = 0;
unsigned volatile long diffRpm = 0;

void enc()
{  
    encTime = millis();
    if((diffTime = encTime - prev_time) >= 90){
    if(digitalRead(ENCODER_B)){      
      if(rpm_set_level <251)
      rpm_set_level += diffTime<180?4:1;
    }          
    else{
      if(rpm_set_level > 3)
      rpm_set_level -= diffTime<180?4:1;
    }     
    prev_time = encTime;
    }    
}
 
//void tick()
//{
//    unsigned long timeNow = millis();
//    diffRpm = timeNow - lastTime;
//    lastTime = timeNow;
//    //rpm_tick_counter++;
//    //Serial.println(rpm_tick_counter);
//    //sumTime += diffRpm;
//}


void setup()
{  
  pinMode(CZUJNIK,INPUT);  
  pinMode(ENCODER_A,INPUT_PULLUP);
  pinMode(ENCODER_B,INPUT_PULLUP);
  pinMode(ENCODER_BUTTON,INPUT);
  
  dsp.addModule("Rpmreal");
  dsp.addModule("Rpmset");
  
  dsp.init();  
  attachInterrupt(digitalPinToInterrupt(ENCODER_A),enc,FALLING); 
}
byte x = 0;
unsigned long refreshTime = 0;
int lastGaz = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // R1(100K)
float R2 = 10000.0; //  R2 (10K)
int value = 0;
byte count = 0;
//long times[2] = {0,0};
unsigned long timeNow = 0;
void loop() 
{          
        if(rpm_set_level != x)
        {
          x = rpm_set_level;
          dsp.setNumber("Rpmset",MIN_RPM+rpm_set_level*RESOLUTION,0);              
        }
        value = analogRead(CZUJNIK);
        vout = (value * 5) / 1024.0;
        vin = vout / (R2 / (R1 + R2));
        //Serial.println(vin);
        if (vin < 50.)
        {
          if(micros() - timeNow >= REFRESH_RATE*500)rpm = 0;
        } else {
          long rpm_pom = 100000000/(micros()-timeNow)/2;
          if(rpm_pom < 4000)
          rpm = rpm_pom;
          rpm /= RESOLUTION;
          rpm *= RESOLUTION;
          timeNow = micros();    
        }

        if(millis() - refreshTime >= REFRESH_RATE)
        {
          //noInterrupts();
//          timeNow = micros();
//          do{
//            value = analogRead(CZUJNIK);
//            vout = (value * 5) / 1024.0;
//            vin = vout / (R2 / (R1 + R2));  
//            if(vin > 50.){                      
//            rpm = 60000000/(micros()-timeNow)/2;                                 
//            timeNow = micros();
//            }
//          }while(rpm > 4000);
//          rpm /= RESOLUTION;
//          rpm *= RESOLUTION;
          //interrupts();
          dsp.setNumber("Rpmreal",rpm,0);
          refreshTime = millis();  
        }       
        
}
