#include <Servo.h>
#include <TridentTD_7Segs74HC595.h>

// * KONSTANTY *
#define SERVO_MIN_ANGLE 45
#define SERVO_MAX_ANGLE 135 
#define GAZ_MIN 20
#define GAZ_MAX 1000
#define MIN_RPM 1000
#define REFRESH_RATE 200

//* PINY *
#define CZUJNIK 3
#define SCLK 5
#define RCLK 6
#define DIO 7
#define GAZ A1
#define ENCODER_A 2
#define ENCODER_B 3
#define ENCODER_BUTTON 10
#define SERVO 4

// * GLOBALSY *
TridentTD_7Segs74HC595  dsp( SCLK, RCLK, DIO, 2);
Servo gaz;
volatile byte rpm_set_level = 0;
volatile byte rpm_tick_counter = 0;
bool mode = false; //true - zdalne, false - sterowanie samemu 
int angle = 0;
volatile long prev_time = 0;
volatile long encTime;
volatile long diffTime;
volatile bool setChanged = false;

void enc()
{  
    encTime = millis();
    if((diffTime = encTime - prev_time) >= 90){
    if(digitalRead(ENCODER_A))
      rpm_set_level += diffTime<180?4:1;    
    else
      rpm_set_level -= diffTime<180?4:1;     
    prev_time = encTime;
    }
}
 
void tick()
{
    
}

void setup()
{  
  //Serial.begin(9600);
  pinMode(CZUJNIK,INPUT_PULLUP);  
  pinMode(GAZ,INPUT);
  pinMode(ENCODER_A,INPUT_PULLUP);
  pinMode(ENCODER_B,INPUT_PULLUP);
  pinMode(ENCODER_BUTTON,INPUT);
  
  dsp.addModule("Rpmset");
  dsp.addModule("Rpmreal");
  
  dsp.init();
  //dsp.setNumber("rpm_set",MIN_RPM,0);
  //attachInterrupt(digitalPinToInterrupt(ENCODER_B),tick,FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B),enc,FALLING);
  //gaz.attach(SERVO);  
}
byte x = 0;
void loop() 
{   
  //Serial.println(1000+rpm_set_level*25);
  //if(millis() - prev_time >= REFRESH_RATE){
    //prev_time = millis();
    //noInterrupts();
      //Serial.println((float)rpm_set_level);
       //if((pom=(float)rpm_set_level) != x){
        //x = pom;
        if(rpm_set_level != x){
          x = rpm_set_level;
       dsp.setNumber("Rpmset",1000+rpm_set_level*25,0);
       //dsp.setNumber("Rpmreal",1.0,0);
        }
//       Serial.print("A: ");
//       Serial.println(digitalRead(ENCODER_A));
//       Serial.print("B: ");
//       Serial.println(digitalRead(ENCODER_B));
       
       //dsp.setNumber("Rpmset",(float)rpm_set_level);
       //dsp.setText("Rpmreal","oooo");
       //interrupts();
       //delay(2000);
       
       //}
      // interrupts();
  //}
     
    
//    //setChanged = false;
//    //if(rpm_set_level != x){
//      noInterrupts();
//     float x = rpm_set_level;
//    
//    //delay(200);
//    //Serial.println(rpm_set_level);
//    dsp.setNumber("Rpmreal", x+10,0);  
//    
//    delay(1300);
//    //Serial.println("BUTTON");
//    interrupts();
//    //}
    
    
//  if(mode){
//      
//  } else {      
//    angle = analogRead(GAZ);
//    angle = map(angle,GAZ_MIN,GAZ_MAX,SERVO_MIN_ANGLE,SERVO_MAX_ANGLE);
//    gaz.write(angle);        
//    delay(15);
//  }
}
