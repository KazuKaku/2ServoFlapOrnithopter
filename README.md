# Arduino CODE for 2ServoFlapOrnithopter

![230711-2 Pterasaur3small](/image/230711-2%20Pterasaur3small%20.jpg)

The Servo Flap Ornithopter(SFO) flies by flapping its wings, which are programmed through an Arduino board and operated using Servo.
Unlike model airplanes which use propellers to fly, this allows for a more natural flight.
However, because it uses the weak power of the Servo to flap its wings, it needs to be as lightweight as possible, which demands advanced building techniques.
 This is the code for an Ornithopter which uses two Servos.

 ![250110Ornithopters](/image/250110Ornithopters.jpg)

## Flight controls for an Ornithopter with a horizontal stabilizer

![2210830 ServoFlap system of 2 serevo](/image/210830%20ServoFlap%20system%20of%202%20serevo.jpg)

## New Servo Flap System by K.Kakuta

1 Bilateral Servo Flap between Max high point and Max low point

2 Change max flap point (throttle stick 3ch ) 
 
  and change Flapping frequency(5ch)

3 Change center of Flapping angle Horizontal (Ch1 aileron stick) and Vertical (Ch2 elevator stick)

4 Change flapping amplitude on each Servo (Ch4 rudder stick)
Increase flap amplitude of one servo and decrease flap amplitude of another servo

## Setting : 

Set elevator and rudder and aileron stick Center-- 1500microsesond

Set 5Ch at Slider or Volume or other switch

Set throttle stick at low max --1000 microsecond

## Need :
   Small ppm output Receiver(over 5 channels)

   High power High speed Digital Servo

   -----BLUEARROW AF D43S-6.0-MG Micro Metal Gear Digital Servo is best
       
       When using a servo with a high supply voltage (such as HVServo), 
       
                                          please use wiring appropriate for that servo.

   Arduino Pro mini board  
   
   ---(Seeeduino XIAO SAMD21 can also be used if the power supply to the receiver is set to 5V)

   Lipo1cell (high discharge rate 20C)

## Wiring

PPM Receiver-- RX PPM signal input to D2 pin

right Servo --D5 pin

left Servo--D6 pin

A voltage of 6V is supplied to the RX.

![210811-2 New ServoFlap system  Wiring](/image/210811-2%20New%20ServoFlap%20system%20%20Wiring.jpg)


Ground -GND pin

6V -RAW pin ( 6-6.2V from Step down DC converter or Step up DC converter for 6V servo)




## Wiring 5V for ppm RX
Wire 5V power supply from Pro Mini Board VCC to 5V ppm RX

![224023 New ServoFlap system for 5VRX  Wiring](/image/224023%20New%20ServoFlap%20system%20for%205VRX%20%20Wiring.jpg)
## My setting

Lipo: 70-150mAh1cell Lipo battery

Servo: BLUEARROW AF D43S-6.0-MG Micro Metal Gear Digital Servo

Arduino Pro mini board

DC step up converter from 1cell Lipo 3.7V to 6V output




## Flap motion and Wing control VTR 
ServoFlapOrnithopterAmericanKestrel112 Flap motion and Flight Control
(https://www.youtube.com/watch?v=uDIVc77CtyM)

## Making VTR 
Making of ServoFlapOrnithopterAmericanKestrel112 
(https://www.youtube.com/watch?v=oznn0DH_pOc)

All List
(https://www.youtube.com/playlist?list=PLErvdRrwWuPooowmNbeGPEB-HlgC26jC-)


## My YouTube channel 
 Various ServoFlapOrnithopters have been uploaded.
(https://www.youtube.com/@BZH07614)

## My Website of ornithopter
 (http://kakutaclinic.life.coocan.jp/HabatakE.htm)

## Request site for production of Kazu Ornithpter
(http://kakutaclinic.life.coocan.jp/KOrniSSt.html)
 



## Flight controls for an Ornithopter without a horizontal stabilizer (such as a butterfly type)

The operation of the Aileron is the opposite of that for an Ornithopter with a horizontal stabilizer.

![230706 ServoFlap system of 2 serevo for Butterfly &DragonFly](/image/230706%20ServoFlap%20system%20of%202%20serevo%20for%20Butterfly%20&DragonFly.jpg)

