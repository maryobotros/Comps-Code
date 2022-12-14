// Servo Object
# include <Servo.h>
Servo myServo;
// Servo angle 
int angle;

// Potetiometer pin
int const potPin = A0;
// Potentiometer value
int potVal;

// Phototransistor pin
const int lightSensor = A1;
// Phototransistor value
int lightVal;

// Temperature pin
const int tempSensor = A5;
// Temperature value
int tempVal;
// Variables for converting temp from Celsius to Farenheit
float voltage;
float tempInC;
float tempInF;

// Potentiometer threshold
int threshold = 90; // set at 90 by default because it's in-between 0 and 180
// Temperature threshold
int tempThreshold = 90.0; // set at 90 degrees Farenheit by default
// Upper limit
int upperLimit = 180; // set at 180 by default because this is the max angle of the servo
// Lower limit
int lowerLimit = 0; // se at 0 by default bevause this is the min angle of the servo

// LEDs for debugging
int lightPin = 2;
int yellowLight = 7;
int greenLight = 3;

// Variable for controlling modes
int x = 0;


// setup() code runs once
void setup() {
  // Initialize serial monitor
  Serial.begin(9600);
  
  // Servo motor is attatched to pin 9
  myServo.attach(9);

  // All LEDs are outputs 
  pinMode(lightPin, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(greenLight, OUTPUT);
}


// Main code in loop() runs repeatedly:
// Code within this loop turns LEDs on and off for debugging and LEDs are not required for the program to run
void loop() {
  // Potentiometer Mode
  if(x == 0){
    // Send info to the webpage that potentiometer mode is selected
    Serial.println("Mode: Potentiometer");
    digitalWrite(lightPin, LOW);

    // Read the value of the potetiometer
    potVal = analogRead(potPin);

    // Scale the potentiometer's value and set the angle to it
    angle = map(potVal, 0, 1023, 0, 180);

    // Set servo to the angle
    myServo.write(angle);
    delay(15);

    // If angle of the servo is greater than the threshold
    if(angle >= threshold){
      // Send the webpage info that the blinds are open
      Serial.println("Open");
      digitalWrite(yellowLight, LOW);
      digitalWrite(greenLight, HIGH);
    }
    // Otherwise
    else if(angle < threshold){
      // Send the webpage info that the blinds are closed
      Serial.println("Closed"); 
      digitalWrite(yellowLight, HIGH);
      digitalWrite(greenLight, LOW);
    }
  }
  
  // Phototransistor Mode
  else if(x == 1){
    // Send info to the webpage that phototransistor mode is selected
    Serial.println("Mode: Phototransistor");
    digitalWrite(lightPin, HIGH);

    // Read the value from the phototransistor
    lightVal = analogRead(lightSensor);
    
    // If the light value is greater than 600 (default threshold for sensing light)
    if(lightVal > 600){
       // Move the servo to the upper limit and 
       // send the webpage info that the blinds are open
       myServo.write(upperLimit);
       Serial.println(upperLimit);
       Serial.println("Open"); 
       digitalWrite(yellowLight, LOW);
       digitalWrite(greenLight, HIGH);
     }
     // Otherwise
     else{
       // Move the servo to the lower limit and 
       // send the webpage info that the blinds are closed
       myServo.write(lowerLimit);
       Serial.println(lowerLimit);
       Serial.println("Closed"); 
       digitalWrite(yellowLight, HIGH);
       digitalWrite(greenLight, LOW);
     }

     delay(1000);
  }
  
  // Temperature Mode
  else if(x == 2){
    // Send info to the webpage that the temperature mode is selected
    Serial.println("Mode: Temperature");
    digitalWrite(lightPin, LOW);

    // Calculate the value of the temperature from the temp sensor
    // and convert it to Farenheit
    tempVal = analogRead(tempSensor);
    voltage = (tempVal / 1024.0) * 5.0;
    tempInC = (voltage - .5) * 100;
    tempInF = (tempInC * 1.8) + 32;
    Serial.println(tempInF);

    // If the temperature is greater than the temperatur threshold
    if(tempInF >= tempThreshold){
      // Move the servo to the lower limit and 
      // send the webpage info that the blinds are closed
      myServo.write(lowerLimit);
      Serial.println(lowerLimit);
      Serial.println("Closed"); 
      digitalWrite(yellowLight, HIGH);
      digitalWrite(greenLight, LOW);
    }
    else{
      // Move the servo to the upper limit and 
      // send the webpage info that the blinds are open
      myServo.write(upperLimit);
      Serial.println(upperLimit);
      Serial.println("Open"); 
      digitalWrite(yellowLight, LOW);
      digitalWrite(greenLight, HIGH);
    }

    delay(1000);  
  }

  // Temperature or Phototransistor Mode
  else if(x == 3){
    Serial.println("Mode: Temperature or Light");
    
    // Calculate the value of the temperature from the temp sensor
    // and convert it to Farenheit
    tempVal = analogRead(tempSensor);
    voltage = (tempVal / 1024.0) * 5.0;
    tempInC = (voltage - .5) * 100;
    tempInF = (tempInC * 1.8) + 32;
    Serial.println(tempInF);

    // Get the light value from the phototransistor
    lightVal = analogRead(lightSensor);

    // If the temperature is above the temperature threshold or the light is below the threshold
    if(tempInF >= tempThreshold || lightVal < 600){
       // Move the servo to the lower limit and 
       // send the webpage info that the blinds are closed
       myServo.write(lowerLimit);
       Serial.println(lowerLimit);
       Serial.println("Closed"); 

       digitalWrite(yellowLight, HIGH);
       digitalWrite(greenLight, LOW);
     }
     // Otherwise
     else{
      // Move the servo to the upper limit and 
      // send the webpage info that the blinds are open
      myServo.write(upperLimit);
      Serial.println(upperLimit);
      Serial.println("Open"); 

      digitalWrite(yellowLight, LOW);
      digitalWrite(greenLight, HIGH);
    }

    delay(1000);
  }

  // Temperature and Phototransistor Mode
  else if(x == 4){
    Serial.println("Mode: Temperature and Light");
    
    // Calculate the value of the temperature from the temp sensor
    // and convert it to Farenheit
    tempVal = analogRead(tempSensor);
    voltage = (tempVal / 1024.0) * 5.0;
    tempInC = (voltage - .5) * 100;
    tempInF = (tempInC * 1.8) + 32;
    Serial.println(tempInF);

    // Get the light value from the phototransistor
    lightVal = analogRead(lightSensor);

    // If the temperature is greater than or equal to the temperature threshold and there is no light
    if(tempInF >= tempThreshold && lightVal < 600){
       // Move the servo to the lower limit and 
       // send the webpage info that the blinds are closed
       myServo.write(lowerLimit);
       Serial.println(lowerLimit);
       Serial.println("Closed"); 

       digitalWrite(yellowLight, HIGH);
       digitalWrite(greenLight, LOW);
     }
     else{
      // Move the servo to the upper limit and 
      // send the webpage info that the blinds are open
      myServo.write(upperLimit);
      Serial.println(upperLimit);
      Serial.println("Open"); 

      digitalWrite(yellowLight, LOW);
      digitalWrite(greenLight, HIGH);
    }

    delay(1000);
  }

  // Open blinds
  else if(x == 5){
    Serial.println("Mode: Open");

    // Move the servo to the upper limit and 
    // send the webpage info that the blinds are open
    myServo.write(upperLimit);
    Serial.println("Open");

    digitalWrite(yellowLight, LOW);
    digitalWrite(greenLight, HIGH);

    delay(1000);
  }

  // Close blinds 
  else if(x == 6){
    Serial.println("Mode: Close");

    // Move the servo to the lower limit and 
    // send the webpage info that the blinds are closed
    myServo.write(lowerLimit);
    Serial.println("Closed"); // Comment out in order to be able to be able to debug

    digitalWrite(yellowLight, HIGH);
    digitalWrite(greenLight, LOW);
    
    delay(1000);
  }
  

  // Check to make sure Arduino is receiving information over serial port
  if(Serial.available() > 0){
    String receivedString = "";

    // Loop that constantly reads information that is sent from the webapp through the serialport
    // and puts it into receivedString variable 
    while(Serial.available() > 0){
      receivedString += char(Serial.read());
    }

    // User selects "Potentiometer Mode"
    if(receivedString == "0"){
      // Set mode to "Potentiometer Mode"
      x = 0;
    }

    // User selects "Phototransistor Mode"
    else if (receivedString == "1"){
      // Set mode to "Phototransistor Mode"
      x = 1;
    }

    // User selects "Temperature Mode"
    else if(receivedString == "2"){
      // Set mode to "Temperature Mode"
      x = 2;
    }

    // User selects "Temperature or Phototransistor" mode
    else if(receivedString == "3"){
      // Set mode to "Temperature or Phototransistor"
      x = 3;
    }

    // User selects "Temperature and Phototransistor" mode
    else if(receivedString == "4"){
      // Set mode to "Temperature and Phototransistor"
      x = 4;
    }

    // User clicks "Open" button
    else if(receivedString == "5"){
      // Open blinds 
      x = 5;
    }
    
    // User clicks "Close" button 
    else if(receivedString == "6"){
      // Close blinds
      x = 6;
    }

    // User sets potentiometer threshold
    else if(receivedString.substring(0, 1) == "P"){
      // Change potentiometer threshold to the value in the input box
      threshold = receivedString.substring(2, 4).toInt();
    }

    // User sets temperature threshold
    else if(receivedString.substring(0, 1) == "T"){
      // Change temperature threshold to value in the input box 
      tempThreshold = receivedString.substring(2, 4).toInt();
    }

    // User sets upper limit 
    else if(receivedString.substring(0, 1) == "U"){
       // Change upper limit to value in the input box
       upperLimit = receivedString.substring(2, 5).toInt();
    }

    // User sets lower limit 
    else if(receivedString.substring(0, 1) == "L") {
       // Change lower limit to value in the input box
       lowerLimit = receivedString.substring(2, 4).toInt();
    }
  }
}
