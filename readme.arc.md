
# Architecture Overview
There are three main files in the project, index.ejs, index.html, and sketch_oct09b.ino. These files communicte together in order to send data between the webapp and the Arduino robot.

Simplied implified overview of the purpose of each file: 
- `index.html`: code for webapp 
- `sketch_oct09.ino`: code for the Arduino robot
- `index.ejs`: code that passes information between the webapp and the Arduino robot over the Node.js server

The webpage can pass information to the Arduino Robot through a Node.js server. 
- Information is passed back and forth between the webpage and Node.js using Socket.io
- Infomrtion is passed back and forth between the Arduino robot and Node.js using Serialport
![Architecture](Architecture.png)


WEBAPP <-(socket.io)-> NODE.JS <-(serialport)-> ARDUINO ROBOT

## index.ejs
The `index.ejs` file listens for a message from the Arduino over the USD port and then passes a message onto the HTML/JavaScript using Socket.io. 
```javascript
// Start the server using http library and file system library to grab the index page
var http = require('http');
var fs = require('fs');
var index = fs.readFileSync( 'index.html');

// Load serialport library into SerialPort variable
var SerialPort = require('serialport');

// Parser is created and tells Node.js to receive data coming in through serialport, reading each line
const parsers = SerialPort.parsers;
const parser = new parsers.Readline({
    delimiter: '\r\n'
});

// Port has to be opened up with the location of the port connected to the Arduino passed in as an argument.
// This argument passed in on the line below '/dev..' will need to be substituted with your own port address
// Connect Arduino to computer and use the command ls /dev/{tty,cu}.* in comand line to find your port address
var port = new SerialPort('/dev/tty.usbmodem14101',{
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false
});

// Attatch port to parser object
port.pipe(parser);

// Server named 'app' is craeted and it will return the contents of the index.html file.
var app = http.createServer(function(req, res) {
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.end(index);
});

// Socket has to be initialized, which will send the information from the Node.js server to the webpage.
var io = require('socket.io').listen(app);

// Connection event is executed
// When a button is clicked on a webpage one of the functions inside this event
// will send data to the Arduino to be read
io.on('connection', function(socket) {
    // Inidcate that Node is using port upon connection
    console.log('Node is listening to port');
    
    // Function for sending selected mode 
    socket.on('modeSelector',function(data){
        console.log( data );
        port.write( data.status );
    });

    // Function for sending potentiometer threshold
    socket.on('input-text', function(data){
        port.write('P ' + data);
        console.log('P ' + data);
    });

    // Function for sending temperature threshold
    socket.on('input-temp', function(data){
        port.write('T ' + data);
        console.log('T ' + data);
    });

    // Function for sending upper limit
    socket.on('input-upper', function(data){
        port.write('U ' + data);
        console.log('U ' + data);
    });
    
    // Function for sending lower limit
    socket.on('input-lower', function(data){
        port.write('L ' + data);
        console.log('L ' + data);
    });
});

// Parser is given an on event whenever data is received
// When the parser receives data, it will run the function below and this data will be console logged. 
//Additionally, when data is received from the parser, which is data coming from the serialport, a message should be emitted. 
// That message will be called ‘data’ and the data that will be passed will be the data variable.
parser.on('data', function(data){
    console.log('Received data from port: ' + data);

    io.emit('data', data);
})

app.listen(3000);
```

## index.html
```javascript
<!doctype html>
<html>
   <head>
      <title>Communicating from Node.js to an Arduino</title>
      <script src='https://cdnjs.cloudflare.com/ajax/libs/socket.io/2.0.4/socket.io.js'></script>
   </head>
   <body>

      <h1 id="title">Smart Blinds</h1>

      <!-- Status heading -->
      <h3 id="statusHeading">Status</h3>

      <!-- Status indicators -->
      <div>
         <p id="mode">Mode: Select a mode</p> 
         <p id="status"></p>
      </div>
      </br>

      <!-- Quick functions heading-->
      <h3 id="quickFunctionsHeading">Quick Functions</h3>
      <div>
         <button id="openBlinds" onclick="openBlinds()">Open blinds</button>
         <button id="closeBlinds" onclick="closeBlinds()">Close blinds</button>
      </div>
      </br>

      <!-- Modes heading -->
      <h3 id="modesHeading">Modes</h3>

      <!-- Buttons for the different modes  -->
      <div>
         <button id="dial" onclick="potentiometer()">Potentiometer Mode</button>
         <button id="light" onclick="phototransistor()">Phototransistor Mode</button>
         <button id="temp" onclick="temperature()">Temperature Mode</button>
         <button id="tempOrLight" onclick="tempOrLight()">Temperature or Phototransistor</button>
         <button id="tempAndLight" onclick="tempAndLight()">Temperature and Phototransistor</button>
      </div>
      </br>
      
       
      <!-- Settings heading -->
      <h3 id="settingsHeading">Settings</h3>

      <!-- Adjust settings for the potentiometer -->
      <div>
         <input id="input-text" type="text" placeholder="10-60" maxlength="32">
         <input id="send-text" type="submit" value="Set potentiometer threshold">
      </div>

      <!-- Adjust settings for the temperature -->
      </br>
      <div>
         <input id="input-temp" type="text" placeholder="10-130" maxlength="32">
         <input id="send-temp" type="submit" value="Set temperature (Farenheit) threshold"> 
      </div>

      <!-- Adjust settings for the upper limit -->
      </br>
      <div>
         <input id="input-upper" type="text" placeholder="100-180" maxlength="32">
         <input id="send-upper" type="submit" value="Set upper limit">
      </div>

      <!-- Adjust settings for lower limit -->
      </br>
      <div>
         <input id="input-lower" type="text" placeholder="0-99" maxlength="32">
         <input id="send-lower" type="submit" value="Set lower limit">
      </div>

      <script>
         // Initializing socket variable
         var socket = io();

         // Logging all data gathered from socket
         socket.on('data', function(data){
            console.log(data);

            if(data == "Closed"){
               document.getElementById("status").innerHTML = "Status: Closed";
            }
            else if(data == "Open"){
               document.getElementById("status").innerHTML = "Status: Open";
            }
         });

         // When potentiometer is clicked
         function potentiometer(){
            document.getElementById("mode").innerHTML = "Mode: Potentiometer";
            socket.emit('modeSelector', { "status":"0" });
         }

         // When phototransistor is clicked
         function phototransistor(){
            document.getElementById("mode").innerHTML = "Mode: Phototrasistor";
            socket.emit('modeSelector', { "status":"1" });
         }

         // When temperature is clicked
         function temperature(){
            document.getElementById("mode").innerHTML = "Mode: Temperature";
            socket.emit('modeSelector', {"status":"2"});
         }

         // When teperature or photransistor is clicked
         function tempOrLight(){
            document.getElementById("mode").innerHTML = "Mode: Temperature or Light";
            socket.emit('modeSelector', {"status":"3"});
         }

         // When temperature and phototransistor is clicked
         function tempAndLight(){
            document.getElementById("mode").innerHTML = "Mode: Temperature and Light";
            socket.emit('modeSelector', {"status":"4"});
         }

         // When open blinds is clicked
         function openBlinds(){
            
            socket.emit('modeSelector', {"status":"5"});
         }

         // When close blinds is clicked
         function closeBlinds(){
            
            socket.emit('modeSelector', {"status":"6"});
         }

         // Setting the potentiometer threshold
         var sendTextButton = document.getElementById("send-text");

         sendTextButton.addEventListener("click", function(){
            var sendText = document.getElementById("input-text").value;
            socket.emit('input-text', sendText)
         });

         // Setting the temperature threshold
         var sendTempButton = document.getElementById("send-temp");

         sendTempButton.addEventListener("click", function(){
            var sendTemp = document.getElementById("input-temp").value;
            socket.emit('input-temp', sendTemp)
         });

         // Setting the upper limit
         var sendUpperButton = document.getElementById("send-upper");

         sendUpperButton.addEventListener("click", function(){
            var sendUpper = document.getElementById("input-upper").value;
            socket.emit('input-upper', sendUpper)
         });

         // Setting the lower limit
         var sendLowerButton = document.getElementById("send-lower");

         sendLowerButton.addEventListener("click", function(){
            var sendLower = document.getElementById("input-lower").value;
            socket.emit('input-lower', sendLower)
         });

      </script>


      <style>
         *{
            background-color: black;
         }

         #title{
            color:white;
         }
         
         #light, #dial, #temp, #tempOrLight, #tempAndLight, #openBlinds, #closeBlinds{
            background-color:rgb(255, 255, 255);
            color:rgb(82, 82, 82);
            border-radius:5px;
            border-color:gray;
         }

         #mode, #status, #quickFunctionsHeading, #statusHeading, #modesHeading, #settingsHeading, #input-text, #send-text, #input-temp, #send-temp, #input-upper, #send-upper, #input-lower, #send-lower{
            color:white;
         }
      </style>

   </body>
</html>
```

## skecth_oct09b.ino
```c++
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
```


