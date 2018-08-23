
var c=document.getElementById("c");
var ctx=c.getContext("2d");
var c2=document.getElementById("c2");
var ctx2=c2.getContext("2d");
socket = io.connect('http://' + document.domain + ':' + location.port);
socket.on('connect', function() {
    socket.emit('client_connected', {data: 'Connected'});
});

socket.on('pixels', function (data) {
var pix = new Uint8Array(data);
var arrayLength = pix.length;

var imgData=ctx.createImageData(96,34);
var x = 0;
for (var i=0;i<imgData.data.length;i+=4){
    var y = 0;
    var b = 0;
    if (x>1535 && x<1728) b = 255;
    if(pix[x]) y = 255;
    imgData.data[i+0]=y;
    imgData.data[i+1]=y;
    imgData.data[i+2]=b;
    imgData.data[i+3]=255;
    x++;
}
ctx.putImageData(imgData,0,0);
ctx2.drawImage(c, 0, 0);
});
var classname = document.getElementsByClassName("modeselect");
Array.from(classname).forEach(function(element) {
  element.addEventListener('click', function(){
      socket.emit('modeselect',this.textContent);
  });
});

function resize() {
	// Our canvas must cover full height of screen
	// regardless of the resolution
	var width = window.innerWidth;
	
	// So we need to calculate the proper scaled width
	// that should work well with every resolution
	var ratio = c2.width/c2.height;
	var height = width / ratio;
	
	c2.style.width = width+'px';
	c2.style.height = height+'px';
}

window.addEventListener('load', resize, false);
window.addEventListener('resize', resize, false);

document.onkeydown = checkKey;

function checkKey(e) {

    e = e || window.event;

    if (e.keyCode == '38') {
        socket.emit('control', 'up')
        // up arrow
    }
    else if (e.keyCode == '40') {
        socket.emit('control', 'down')
        // down arrow
    }
    else if (e.keyCode == '37') {
        socket.emit('control', 'left')
       // left arrow
    }
    else if (e.keyCode == '39') {
        socket.emit('control', 'right')
       // right arrow
    }

}
