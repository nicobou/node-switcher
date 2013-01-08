var socket;
var firstconnect = true;

function connect(){
    if(firstconnect){
	socket = io.connect(null);
	
	socket.on("message", function(data) { message(data);});
	socket.on("connect", function() { status_update("Connected to server")});
	socket.on("disconnect", function() { status_update("Disconnected from server")});
	socket.on('reconnect', function(){ status_update("Reconnected to Server"); });
	socket.on('reconnecting', function( nextRetry ){ status_update("Reconnecting in "
								       + nextRetry + " seconds"); });
	socket.on("reconnect_failed", function(){ message("Reconnect Failed"); });
	socket.on("moveSlider1", function(data){
	    $("#slider1").slider("value", data);
	    $("#number1").text($("#slider1").slider("value"));
	});
	// $("#slider1").change(function(){
	//     socket.emit("moveSlider1", $("#slider1").slider("value")); 
	// });
	firstconnect = false;
    }
    else{
	socket.socket.reconnect();
    }
}

function disconnect() {
    socket.disconnect();
}

function message(data) {
    $("#message").text = "Server says: " +data;
    //document.getElementById("message").innerHTML = "Server says: " +data;
}

function status_update(txt){
    //document.getElementById("status").innerHTML = "Status: " + txt;
    $("#status").html(txt);
}

function send() {
    socket.send("Hey there, server!");
}

$(function() {
    $("#slider1").slider({
	min: 0,
	max: 21,
	orientation: "vertical",
	slide: function(event, ui){
	    $("#number1").text($("#slider1").slider("value"));
	    socket.emit("moveSlider1", $("#slider1").slider("value")); 
	}
    });
});
$(function() {
    $("#slider2").slider({
	min: 0,
	max: 21,
	orientation: "vertical",
	slide: function(event, ui){
	    $("#number2").text($("#slider2").slider("value"));
	}
    });
});
