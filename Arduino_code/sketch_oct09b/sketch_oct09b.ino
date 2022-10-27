// 3
// My first attempt to try to fix the issue using variable x

# include <Servo.h>
Servo myServo;
int lightPin = 2;
int const potPin = A0;
int angle;
const int lightSensor = A1;
int lightVal;
int potVal;
int x = 0;

const int tempSensor = A5;
int tempVal;
float voltage;
float tempInC;
float tempInF;

void setup() {
  pinMode(lightPin, OUTPUT);
  Serial.begin(9600);
  myServo.attach(9);
}

void loop() {
  // Reading in real time
  if(x == 1){
    digitalWrite(lightPin, HIGH);
    
    lightVal = analogRead(lightSensor);
    if(lightVal > 600){
       myServo.write(180);
       Serial.println("Open"); // Comment out in order to be able to be able to debug
     }
     else{
       myServo.write(0);
       Serial.println("Closed"); // Comment out in order to be able to be able to debug
     }
  }
  else if(x == 0){
    digitalWrite(lightPin, LOW);

    potVal = analogRead(potPin);
//    Serial.print("potVal: ");
//    Serial.print(potVal);

    angle = map(potVal, 0, 1023, 0, 180);
//    Serial.print(" , angle: ");
//    Serial.println(angle);

    myServo.write(angle);
    delay(15);

    if(angle >= 90){
      Serial.println("Open"); // Comment out in order to be able to be able to debug
    }
    else if(angle < 90){
      Serial.println("Closed"); // Comment out in order to be able to be able to debug
    }
  }
  else if(x == 2){
    digitalWrite(lightPin, LOW);

    tempVal = analogRead(tempSensor);
    voltage = (tempVal / 1024.0) * 5.0;
    tempInC = (voltage - .5) * 100;
    tempInF = (tempInC * 1.8) + 32;

      
    if(tempInF > 80.0){
      myServo.write(180);
    }
    else{
      myServo.write(0);
    }      
    
  }
  

  // Reading from nodejs
  if(Serial.available() > 0){
    String receivedString = "";

    while(Serial.available() > 0){
      receivedString += char(Serial.read());
    }

//    Serial.println(receivedString);

    if(receivedString == "1"){
      x = 1;
      digitalWrite(lightPin, HIGH);
      Serial.println("Mode: Phototransisor");
      
      lightVal = analogRead(lightSensor);
      if(lightVal > 600){
         myServo.write(180);
       }
       else{
        myServo.write(0);
//        Serial.println("Closed");
      }
    }
    else if (receivedString == "0"){
      x = 0;
      digitalWrite(lightPin, LOW);
      Serial.println("Mode: Potentiometer");
      
      potVal = analogRead(potPin);
//      Serial.print("potVal: ");
//      Serial.print(potVal);

      angle = map(potVal, 0, 1023, 0, 179);
//      Serial.print(" , angle: ");
//      Serial.println(angle);

      myServo.write(angle);
      delay(15);
    }
    else if(receivedString == "2"){
      x = 2;
      digitalWrite(lightPin, LOW);

      tempVal = analogRead(tempSensor);
      voltage = (tempVal / 1024.0) * 5.0;
      tempInC = (voltage - .5) * 100;
      tempInF = (tempInC * 1.8) + 32;

      
      if(tempInF > 75){
         myServo.write(180);
       }
       else{
        myServo.write(0);
      }      
    }
  }
}
