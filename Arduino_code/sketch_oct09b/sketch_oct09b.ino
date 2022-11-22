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

int threshold;
int tempThreshold;

void setup() {
  pinMode(lightPin, OUTPUT);
  Serial.begin(9600);
  myServo.attach(9);
}

void loop() {
  // Reading in real time
  if(x == 0){
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
  else if(x == 1){
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
  else if(x == 2){
    digitalWrite(lightPin, LOW);
    Serial.println("Mode: Temperature");

    tempVal = analogRead(tempSensor);
    voltage = (tempVal / 1024.0) * 5.0;
    tempInC = (voltage - .5) * 100;
    tempInF = (tempInC * 1.8) + 32;
    Serial.println(tempInF);
  
    if(tempInF >= 90.0){
      myServo.write(0);
      Serial.println("Closed"); // Comment out in order to be able to be able to debug
    }
    else{
      myServo.write(180);
      Serial.println("Open"); // Comment out in order to be able to be able to debug
    }

    delay(1000);  
  }
  else if(x == 3){
    Serial.println("Mode: Temperature or Light");
    
    // Get the temperature
    tempVal = analogRead(tempSensor);
    voltage = (tempVal / 1024.0) * 5.0;
    tempInC = (voltage - .5) * 100;
    tempInF = (tempInC * 1.8) + 32;
    Serial.println(tempInF);

    // Get the light
    lightVal = analogRead(lightSensor);

    // Check temperature and light
    if(tempInF >= 90.0 || lightVal > 600){
       myServo.write(0);
       Serial.println("Closed"); // Comment out in order to be able to be able to debug
     }
     else{
      myServo.write(180);
      Serial.println("Open"); // Comment out in order to be able to be able to debug
    }

    delay(1000);
  }
  else if(x == 4){
    Serial.println("Mode: Temperature and Light");
    
    // Get the temperature
    tempVal = analogRead(tempSensor);
    voltage = (tempVal / 1024.0) * 5.0;
    tempInC = (voltage - .5) * 100;
    tempInF = (tempInC * 1.8) + 32;
    Serial.println(tempInF);

    // Get the light
    lightVal = analogRead(lightSensor);

    // Check temperature and light
    if(tempInF >= 90.0 && lightVal > 600){
       myServo.write(0);
       Serial.println("Closed"); // Comment out in order to be able to be able to debug
     }
     else{
      myServo.write(180);
      Serial.println("Open"); // Comment out in order to be able to be able to debug
    }

    delay(1000);
  }
  else if(x == 5){
    potVal = analogRead(potPin);

    angle = map(potVal, 0, 1023, 0, 180);

    myServo.write(angle);
    delay(15);

    if(angle >= threshold){
      Serial.println("Open"); // Comment out in order to be able to be able to debug
    }
    else if(angle < threshold){
      Serial.println("Closed"); // Comment out in order to be able to be able to debug
    }

    delay(100);
  }
  else if(x == 6){
    Serial.println("Mode: Temperature cutom");
    tempVal = analogRead(tempSensor);
    voltage = (tempVal / 1024.0) * 5.0;
    tempInC = (voltage - .5) * 100;
    tempInF = (tempInC * 1.8) + 32;
    Serial.println(tempInF);
      
    if(tempInF >= tempThreshold){
      myServo.write(0);
      Serial.println("Closed"); // Comment out in order to be able to be able to debug
    }
    else{
      myServo.write(180);
      Serial.println("Open"); // Comment out in order to be able to be able to debug
    }

    delay(1000);
  }
  
  

  // Reading from nodejs
  if(Serial.available() > 0){
    String receivedString = "";

    while(Serial.available() > 0){
      receivedString += char(Serial.read());
    }

//    Serial.println(receivedString);

    if(receivedString == "0"){
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
    else if (receivedString == "1"){
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
    else if(receivedString == "2"){
      x = 2;
      digitalWrite(lightPin, LOW);
      Serial.println("Mode: Temperature");

      tempVal = analogRead(tempSensor);
      voltage = (tempVal / 1024.0) * 5.0;
      tempInC = (voltage - .5) * 100;
      tempInF = (tempInC * 1.8) + 32;

      
      if(tempInF >= 90.0){
         myServo.write(0);
       }
       else{
        myServo.write(180);
      }

      delay(1000);
    }
    else if(receivedString == "3"){
      x = 3;
      Serial.println("Mode: Temperature or Light");

      // Get the temperature
      tempVal = analogRead(tempSensor);
      voltage = (tempVal / 1024.0) * 5.0;
      tempInC = (voltage - .5) * 100;
      tempInF = (tempInC * 1.8) + 32;

      // Get the light
      lightVal = analogRead(lightSensor);

      // Check temperature and light
      if(tempInF >= 90.0 || lightVal > 600){
         myServo.write(0);
       }
       else{
        myServo.write(180);
      }
    }
    else if(receivedString == "4"){
      x = 4;
      Serial.println("Mode: Temperature and Light");

      // Get the temperature
      tempVal = analogRead(tempSensor);
      voltage = (tempVal / 1024.0) * 5.0;
      tempInC = (voltage - .5) * 100;
      tempInF = (tempInC * 1.8) + 32;

      // Get the light
      lightVal = analogRead(lightSensor);

      // Check temperature and light
      if(tempInF >= 90.0 && lightVal > 600){
         myServo.write(0);
       }
       else{
        myServo.write(180);
      }
    }
    else if(receivedString.substring(0, 1) == "P"){
      x = 5;
      threshold = receivedString.substring(2, 4).toInt();
    }
    else if(receivedString.substring(0, 1) == "T"){
      x = 6;
      tempThreshold = receivedString.substring(2, 4).toInt();
    }
  }
}
