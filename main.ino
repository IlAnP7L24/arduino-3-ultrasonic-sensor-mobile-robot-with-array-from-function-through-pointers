const int trigPin = 9; //sensors' trigger pins to Pin 9
const int echoPin[3] = {10, 11, 12}; //right sensor echo pin to Pin 10

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0)); //read noise on contact for randomisation functions
  for(int i=5; i<=9; i++) //compact setting of output pins for sensors' triggers and motor driver
	{
	pinMode(i, OUTPUT);
	};
  for(int i=10; i<=12; i++) //compact setting of input pins for sensors' echo
	{
	pinMode(i, INPUT);
	};
  Serial.println("Checking sensors first time");
  SensorCheck();
  Serial.println("Loop start");
  Serial.println(" ");
}

void MoveBackward()
{
  Serial.println("Backward");
  digitalWrite(5, LOW); //left clockwise (off)
  digitalWrite(6, HIGH); //left counterclockwise (on)
  digitalWrite(7, LOW); //right clockwise (off)
  digitalWrite(8, HIGH); //right counterclockwise (on)
}

void MoveFront()
{
  Serial.println("Forward");
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
}

void MoveLeft()
{
  Serial.println("Left");
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
}

void MoveRight()
{
  Serial.println("Right");
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
}

void MoveStop()
{
  Serial.println("STOP");
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}

double * SensorCheck() //returning array from a function through pointers
{
  int duration[3]={0, 0, 0};
  static double distance_cm[3]; //static - is saved for the entirety of program process, outside of the box
  //Read the signal from the sensor: a HIGH pulse whose
  //duration is the time (in microseconds) from the sending
  //of the ping to the reception of its echo off of an object.
  for(int i=0; i<3; i++)
  {
    //The sensor is triggered by a HIGH pulse of 10 or more microseconds sent to the trigger.
    //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    Serial.print("Checking #");
    Serial.println(i+1);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration[i] = pulseIn(echoPin[i], HIGH);
    distance_cm[i] = (duration[i]/2)/29.1; //distance - duration (in microseconds) from the sending of the ping to the reception of its echo off of an object divided by two halves (from sensor to target and back) divided by speed (29.1 is the speed of ultrasound in centimeter/microseconds)

    Serial.print(distance_cm[i]);
    Serial.println(" cm ");
  }
  return distance_cm;
}

void loop()
{
  double *P_distance_cm;
  P_distance_cm=SensorCheck();
  Serial.println(" ");

   if ( (*(P_distance_cm+0)<=10) || (*(P_distance_cm+2)<=10) || (*(P_distance_cm+2)<=10))
    {
		MoveBackward();
        delay(2000);
      	int randNumber = random(2)+1; //randomly gives out 1 or 2 to decide where to turn if obstacles are everywhere in the front
  		Serial.println("Randomly deciding to turn "); 
  		if(randNumber%2==0)
        {
         Serial.println("to the right.");
         MoveRight();
         delay(2000);
        }
       	else
        {
          Serial.println("to the left.");
          MoveLeft();
          delay(2000);
         }
    }
  else
  if (*(P_distance_cm+1)<=30)
   {
    if (*(P_distance_cm+0)<=30)
    {
      if (*(P_distance_cm+2)<=30)
    	{
      		int randNumber = random(2)+1; //randomly gives out 1 or 2 to decide where to turn if obstacles are everywhere in the front
  			Serial.println("Randomly deciding to turn "); 
  			if(randNumber%2==0)
            {
            Serial.println("to the right.");
            MoveRight();
            delay(2000);
            }
        	else
            {
            Serial.println("to the left.");
            MoveLeft();
            delay(2000);
            }
    	}
      	else
        MoveLeft();
     }
    else
    MoveRight();
   }
  else
  MoveFront();
  Serial.println(" ");
}
