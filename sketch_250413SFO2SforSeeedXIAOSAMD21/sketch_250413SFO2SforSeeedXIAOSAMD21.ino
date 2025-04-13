//250413 SFO2servo for SeeedXIAO CODE by K.Kakuta
#include <Servo.h>
#include "src/PPMReader/PPMReader.h"// <PPMReader.h>
//#include <InterruptHandler.h>// 2022/01/27 Delete for more good move

// In use of Flap system,cut "Serial.print "(put ”//”)  
//and write code to Arduino board.
int interruptPin = 2;
int channelAmount = 6;
PPMReader ppm(interruptPin, channelAmount);

int servo_left_pin = 6;
int servo_right_pin = 5;

volatile int elevator = 0;
volatile int flapamp = 0;
//volatile int delaytime = 100;// Servo speed low-increase Servo speed high-decrease this.
                          // unit: micro second
volatile float delaytime = 100;// Servo speed low-increase Servo speed high-decrease this.
                          // unit: micro second
float elapsed_time = 0;
float dt;
unsigned long current_time, prev_time;

volatile int ch3value = 1000;//Ch3
volatile int ch1value = 1500;//Ch1
volatile int ch2value = 1500;//Ch2
volatile int ch4value = 1500;//Ch4: Set Scale of 4ch of TX to 70%
volatile int ch5value = 1500;//Ch5
static int servo_comm1 = 0;// Left or Right Servo high point and low point
static int servo_comm2 = 0; // Left or Right Servo high point and low point
volatile int rudder = 0;
float glide_deg = 0; // Gliding angle 0=0 degree 500=90degree
static float servo_zero1 = 0;//flap angle adjust
static float servo_zero2 = 0; //flap angle adjust

Servo servo_left, servo_right; // create servo object to control a servo

void setup() {
  Serial.begin(9600);

  pinMode(servo_left_pin, OUTPUT);
  pinMode(servo_right_pin, OUTPUT);

  servo_left.attach(servo_left_pin);//output pin No
  servo_right.attach(servo_right_pin); //output pin No
delay(2000);//Avoid abnormal positions at startup-wait 2 second until RX starts220306
}

void loop() {
prev_time = current_time;      
  current_time = micros();      
  dt = (current_time - prev_time)/1000000.0;

  elapsed_time = elapsed_time + dt; // total time spent in the main loop since beginning one upstroke/downstroke

  ch3value = ppm.rawChannelValue(3);//Ch3
  ch1value = ppm.rawChannelValue(1);//Ch1
  ch2value = ppm.rawChannelValue(2);//Ch2
  ch4value = ppm.rawChannelValue(4);//Ch4
ch5value = ppm.rawChannelValue(5);//Ch5


    Serial.print("ch3value ");Serial.print(ch3value);
    Serial.print(",\t"); 
    Serial.print("ch1value ");Serial.print(ch1value);
    Serial.print(",\t");
    Serial.print("ch2value ");Serial.print(ch2value);
Serial.print(",\t");
    Serial.print("ch4value ");Serial.print(ch4value);
Serial.print(",\t");
    Serial.print("ch5value ");Serial.print(ch5value);
    Serial.println(",\t");

 rudder=(int)(ch1value-1500);//Ch1  Flap angle incline-- AileronStick
 elevator=(int)(ch2value-1500);//Ch2 Flap Angle bilateral UP&Down
 flapamp=(int)(ch4value-1500);//Ch4 Right and left Flap angle difference from3to2
 delaytime=(int)((ch5value-950)/5);//Ch5 Flapping frequency 
// you can change UP or Down direction by your transmitter Reverse setting of each Channel

    //Serial.print("rudder");Serial.print(rudder);
    //Serial.print(",\t");
    //Serial.print("elevator");Serial.print(elevator);
    //Serial.print(",\t");
    //Serial.print("flapamp");Serial.print(flapamp);
//Serial.print(",\t");
    //Serial.print("delaytime");Serial.print(delaytime);
    //Serial.print(",\t");

 if (ch3value > 1080) {
if (elapsed_time < delaytime/1000) {

  servo_comm1 = (int)( (ch3value -1000)/2+1500 + rudder - elevator+ servo_zero1+ flapamp);
  servo_comm2 = (int)(1000 + (2000 - ((ch3value -1000)/2+1500)) + rudder + elevator- servo_zero2+flapamp);  
   
  servo_left.writeMicroseconds(servo_comm1); // servo position in variable 'pos'
  servo_right.writeMicroseconds(servo_comm2); // servo position in variable 'pos'
}
//Wait 1second=1000mseconds

if ((elapsed_time > delaytime/1000) && ( elapsed_time < (delaytime + delaytime)/1000)) {

servo_comm1 = (int)( (ch3value -1000)/2+1500 + rudder + elevator+ servo_zero1-flapamp);
  servo_comm2 = (int)(1000 + (2000 - ((ch3value -1000)/2+1500)) + rudder - elevator- servo_zero2-flapamp);  

  servo_left.writeMicroseconds(servo_comm2); // servo position in variable 'pos'
  servo_right.writeMicroseconds(servo_comm1); // servo position in variable 'pos'

      //Serial.print("servo_comm1");Serial.print(servo_comm1);
    //Serial.print(",\t");
    //Serial.print("servo_comm2");Serial.print(servo_comm2);
    //Serial.println(",\t");
  
//Wait 1second=1000mseconds
 }

  }
else{
  
  servo_comm1=(int)(1500+rudder-elevator+glide_deg);
  servo_comm2=(int)(1500+rudder+elevator-glide_deg);  

  servo_left.writeMicroseconds(servo_comm1); // servo position in variable 'pos'
  servo_right.writeMicroseconds(servo_comm2); // servo position in variable 'pos' 

    //Serial.print("servo_comm1");Serial.print(servo_comm1);
    //Serial.print(",\t");
    //Serial.print("servo_comm2");Serial.print(servo_comm2);
    //Serial.println(",\t");

}
     if (elapsed_time > (delaytime + delaytime)/1000) {// one full flap is finisheddt
         elapsed_time = 0; // start next flapping cycle
}

}
