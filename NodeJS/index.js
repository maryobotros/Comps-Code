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