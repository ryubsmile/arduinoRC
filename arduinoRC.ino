const int trigPin1 = 7;
const int echoPin1 = 6;
const int trigPin2 = 5;
const int echoPin2 = 4;
  
const int red_LED = 13;
const int yellow_LED = 12;
const int green_LED = 11;

int lit_up = green_LED;
int man_count = 0;

long BackSensor, FrontSensor;

bool oneAction = true;
int pressedState = 0; 
//0 = non, 1 = front, 2 = back

/*
roomState value schemes
------------------------
greenLED = 11 = normal
yellowLED = 12 = need caution
redLED = 13 = alert
------------------------

//Back -> Front = exit 
//Front -> Back = enter
*/

void setup()
{
  Serial.begin(9600);
  pinMode(red_LED, OUTPUT);
  pinMode(yellow_LED, OUTPUT);
  pinMode(green_LED, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  
  Serial.println("");
}

void loop()
{
  //display light according to the # of people in the room.
  //first int = up to that, green LED.
  //second int = up to that, yellow LED.
  roomCount(4,8); 
  
  //detect the distance from the front sensor.
  FrontSensor = SonarSensor(trigPin1, echoPin1);
  
  //detect back sensor distance.
  BackSensor = SonarSensor(trigPin2, echoPin2);
  
  //for test:
  Serial.print("\n F:");
  Serial.print(FrontSensor);
  //Serial.println("");
  Serial.print(" B:");
  Serial.print(BackSensor);
  
  //front detected -> back detected: person enters the room.
  if(inRange(FrontSensor) && oneAction && (pressedState == 0 || pressedState == 1)){
    pressedState = 1;
    if(inRange(BackSensor)){
        man_count++;
        Serial.print(man_count);
        Serial.println(" people in the room.");
        oneAction = false;
        digitalWrite(lit_up, LOW);
      delay(250);
    }
  }
  
  //back detected -> front detected: person leaves the room.
  if(inRange(BackSensor) && oneAction && (pressedState == 0 || pressedState == 2)){
    pressedState = 2;
    if(inRange(FrontSensor)){
      if(man_count > 0){
        man_count--;
        Serial.print(man_count);
        Serial.println(" people in the room.");
        oneAction = false;
        digitalWrite(lit_up, LOW);
      delay(250);
      }
    }
  }
  
  if(!inRange(FrontSensor) && !inRange(BackSensor)){
    oneAction = true;
    pressedState = 0;
  }
  
}

//led flashes acccording to the # of people in room.
void roomCount(int green_limit, int yellow_limit){
  if(man_count < green_limit + 1){
    blink(green_LED);
    lit_up = green_LED;
  }else if(man_count < yellow_limit + 1){
    blink(yellow_LED);
    lit_up = yellow_LED;
  }else{
    blink(red_LED);
    lit_up = red_LED;
  }
}

//led flasher
void blink(int led_Pin){
  digitalWrite(led_Pin, HIGH);
//  delay(250);
//  digitalWrite(led_Pin, LOW);
//  delay(250);
}

//detects the distance.
long SonarSensor(int trigPin,int echoPin)
{
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

//in the range of 20~200 cm?
bool inRange(long sensorValue){
  if(5 < sensorValue && sensorValue < 200){
    return true;
  }else{
    return false;
  }
}
