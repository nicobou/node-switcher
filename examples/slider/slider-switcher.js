/*
 * Copyright (C) 2012-2013 Nicolas Bouillot (http://www.nicolasbouillot.net)
 *
 * This file is part of node-switcher.
 *
 * node-switcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * node-switcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with node-switcher.  If not, see <http://www.gnu.org/licenses/>.
 */

var switcher_addon = require('../../build/Release/switcher_addon');

process.on('exit', function () {
    switcher_addon.close();
    console.log('About to exit.');
});
process.on('SIGINT', function () {
    switcher_addon.close();
    console.log('Got SIGINT.  About to exit.');
    process.exit(0);
});

var videosource = switcher_addon.create("videotestsrc")
console.log( 'create videotestsource:', videosource );

var videosink = switcher_addon.create("xvimagesink")
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
	switcher_addon.set(videosource, "videotestsrc/pattern", String(data) );
	socket.broadcast.emit("moveSlider1", data);
    });
});


console.log( 'connecting videosink to video source:',switcher_addon.invoke(videosink, "connect", ["/tmp/switcher_nodeserver_videotestsrc0_video"] ));

console.log( 'now visit http://localhost:8082')
