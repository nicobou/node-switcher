// Including libraries
var addon = require('../../build/Release/switcher_addon');

var videosource = addon.create("videotestsrc")
console.log( 'create videotestsource:', videosource );

var videosink = addon.create("xvimagesink")
console.log( 'create video window:', videosink );

var app = require('http').createServer(handler),
io = require('socket.io').listen(app),
static = require('node-static'); // for serving files

// This will make all the files in the current folder
// accessible from the web
var fileServer = new static.Server('./');

// This is the port for our web server.
// you will need to go to http://localhost:8082 to see it
app.listen(8082);

// If the URL of the socket server is opened in a browser
function handler (request, response) {
    
    request.addListener('end', function () {
	fileServer.serve(request, response); // this will return the correct file
    });
}

io.sockets.on('connection', function (socket) {
    console.log("Connection: " + socket.id + " accepted.");
    socket.on('message', function (data) {
	console.log("Received message: " + data + " from client " + socket.id);
    });
    socket.on("disconnect", function(){
	console.log("Connection " + socket.id + " terminated.")
    });
    socket.on("moveSlider1", function(data){
	console.log("Slider1 moving: " + data + " at client " + socket.id);
	addon.set(videosource, "videotestsrc/pattern", String(data) );
	socket.broadcast.emit("moveSlider1", data);
    });
});


console.log( 'connecting videosink to video source:',addon.invoke(videosink, "connect", ["/tmp/switcher_nodeserver_videotestsrc0_video"] ));

console.log( 'now visit http://localhost:8082')
