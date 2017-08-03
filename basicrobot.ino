
/*
Basic robot with two wheels and a 
Ping sensor to determine distance to obstacles
Servo to rotate the ping sensor 180 degrees

*/

#include <Servo.h> 

const int trigPin=2;
const int echoPin=4;
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position
int minSafeDist = 25 ;
int centerDist, leftDist, rightDist, backDist; 
long duration,inches,cm;


/*
  setup arduino
  - initialize pins and motors
*/
void setup(){
   // initialize serial communication:
   Serial.begin(9600);
   myservo.attach(9);  // attaches the servo on pin 9 to the servo object
   myservo.write(90) ; // center the servo 

   //Setup Channel A
   pinMode(12, OUTPUT); //Initiates Motor Channel A pin
   pinMode(9, OUTPUT); //Initiates Brake Channel A pin

   //Setup Channel B
   pinMode(13, OUTPUT); //Initiates Motor Channel B pin
   pinMode(8, OUTPUT);  //Initiates Brake Channel B pin
}

/*
   Forever loop
*/
void loop()
{
   doPingSensor();
   delay(100);
   doServoScan() ;
   delay(100) ;
   doMotorTest() ;
   delay(1000) ;
  
   lookAhead();
   Serial.print(cm);
   Serial.println(" centimeters"); // Prints a line in the serial monitor
   if(cm < minSafeDist && cm >= 0) /* If the inches in front of an object is greater than or equal to the minimum safe distance (11 inches), react*/
   {
      robotStop();
      adjustRight();
      delay(2);
    
   }
   else // If not:
   {
      robotForward(); // All wheels forward
      delay(110); // Wait 0.11 seconds

      robotStop(); // Stop all motors
      lookAround(); // Check your surroundings for best route
      if(rightDist > leftDist) // If the right distance is greater than the left distance , turn right
      {
         turnRight();
      }
      else if (leftDist > rightDist) // If the left distance is greater than the right distance , turn left
      {
         turnLeft();
      }
      else if (leftDist&&rightDist<minSafeDist) // If the left and right distance is smaller than the min safe distance (11 inch) go back
      {
        robotBackward();
      }
     
  }

}

void doPingSensor() {
   // establish variables for duration of the ping, 
   // and the distance result in inches and centimeters:
  

   // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
   // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
   pinMode(trigPin,OUTPUT);
   digitalWrite(trigPin,LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin,HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin,LOW);

   // Read the signal from the sensor: a HIGH pulse whose
   // duration is the time (in microseconds) from the sending
   // of the ping to the reception of its echo off of an object.
   pinMode(echoPin,INPUT);
   duration=pulseIn(echoPin,HIGH);

   // convert the time into a distance
   inches=microsecondsToInches(duration);
   cm=microsecondsToCentimeters(duration);

   Serial.print(inches);
   Serial.print(" in, ");
   Serial.print(cm);
   Serial.print(" cm");
   Serial.println();
}

long microsecondsToInches(long microseconds) {
// According to Parallax's datasheet for the PING))), there are
// 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
// second).  This gives the distance travelled by the ping, outbound
// and return, so we divide by 2 to get the distance of the obstacle.
// See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
   return microseconds/74/2;
}

long microsecondsToCentimeters(long microseconds) {
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
   return microseconds/29/2;
}

void robotStop() {
  
  digitalWrite(9, HIGH);  //Engage the Brake for Channel A
  digitalWrite(8, HIGH);  //Engage the Brake for Channel B
}

void robotForward() {
   //Motor A forward @ full speed
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);   //Spins the motor on Channel A at full speed

  //Motor B backward @ half speed
  digitalWrite(13, HIGH);  //Establishes backward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);    //Spins the motor on Channel B at half speed
}

void robotBackward() {
    
  //Motor A backward @ full speed
  digitalWrite(12, LOW);  //Establishes backward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);    //Spins the motor on Channel A at half speed
  
  //Motor B backward @ full speed
  digitalWrite(13, LOW); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);   //Spins the motor on Channel B at full speed
   delay(1600) ;
}

void turnRight() {
  // motor A forward
   digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);   //Spins the motor on Channel A at full speed
   //Motor B backward @ full speed
  digitalWrite(13, LOW); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);   //Spins the motor on Channel B at full speed
  delay(200) ;
}

void adjustRight() {
  // motor A forward
   digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);   //Spins the motor on Channel A at full speed
   //Motor B backward @ full speed
  digitalWrite(13, LOW); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);   //Spins the motor on Channel B at full speed
  delay(2) ;
}

void turnLeft() {
    // motor A backward
   digitalWrite(12, LOW); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);   //Spins the motor on Channel A at full speed
   //Motor B forward @ full speed
  digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);   //Spins the motor on Channel B at full speed
  delay(1600) ;
}

void lookAhead() {
 // myservo.write(90);// angle to look forward
 // delay(175); // wait 0.175 seconds
  doPingSensor();
}

void lookAround() {
  myservo.write(180); // 180° angle
  delay(320); // wait 0.32 seconds
  doPingSensor();
  rightDist = inches; //get the right distance
  myservo.write(0); // look to the other side
  delay(620); // wait 0.62 seconds
  doPingSensor();
  leftDist = inches; // get the left distance
  myservo.write(90); // 90° angle
  delay(275); // wait 0.275 seconds

  // Prints a line in the serial monitor
   Serial.print("RightDist: ");
   Serial.println(rightDist);
   Serial.print("LeftDist: ");
   Serial.println(leftDist);
   Serial.print("CenterDist: ");
   Serial.println(centerDist);
}


 
void doServoScan() { 
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}




void doMotorTest() {

  
  //Motor A forward @ full speed
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);   //Spins the motor on Channel A at full speed
  
   delay(3000);

  //Motor B backward @ half speed
  digitalWrite(13, HIGH);  //Establishes backward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);    //Spins the motor on Channel B at half speed

  
  delay(3000);

  
  digitalWrite(9, HIGH);  //Engage the Brake for Channel A
  digitalWrite(8, HIGH);  //Engage the Brake for Channel B


  delay(1000);
  
  
  //Motor A forward @ full speed
  digitalWrite(12, LOW);  //Establishes backward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);    //Spins the motor on Channel A at half speed
  
  //Motor B forward @ full speed
  digitalWrite(13, LOW); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);   //Spins the motor on Channel B at full speed
  
  delay(3000);
  
  digitalWrite(9, HIGH);  //Engage the Brake for Channel A
  digitalWrite(8, HIGH);  //Engage the Brake for Channel B
  
  delay(1000);
  
}

