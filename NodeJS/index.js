var http = require('http');
var fs = require('fs');
var index = fs.readFileSync( 'index.html');

var SerialPort = require('serialport');
const parsers = SerialPort.parsers;

const parser = new parsers.Readline({
    delimiter: '\r\n'
});

var port = new SerialPort('/dev/tty.usbmodem14101',{ 
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false
});

port.pipe(parser);

var app = http.createServer(function(req, res) {
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.end(index);
});

var io = require('socket.io').listen(app);

io.on('connection', function(socket) {
    console.log('Node is listening to port');
    
    socket.on('modeSelector',function(data){
        console.log( data );
        port.write( data.status );
    });


    socket.on('input-text', function(data){
        port.write('P ' + data);
        console.log('P ' + data);
    });

    socket.on('input-temp', function(data){
        port.write('T ' + data);
        console.log('T ' + data);
    });

    socket.on('input-upper', function(data){
        port.write('U ' + data);
        console.log('U ' + data);
    });
    
    socket.on('input-lower', function(data){
        port.write('L ' + data);
        console.log('L ' + data);
    });
});

parser.on('data', function(data){
    console.log('Received data from port: ' + data);

    io.emit('data', data);
})

app.listen(3000);