var timer;
var angle = 0;

var gn;

function init_gn() {
var args = {
	frequency:100,					// ( How often the object sends the values - milliseconds )
	gravityNormalized:true,			// ( If the gravity related values to be normalized )
	orientationBase:GyroNorm.GAME,		// ( Can be GyroNorm.GAME or GyroNorm.WORLD. gn.GAME returns orientation values with respect to the head direction of the device. gn.WORLD returns the orientation values with respect to the actual north direction of the world. )
	decimalCount:0,					// ( How many digits after the decimal point will there be in the return values )
	logger:logger,					// ( Function to be called to log messages from gyronorm.js )
	screenAdjusted:false			// ( If set to true it will return screen adjusted values. )
};
  gn = new GyroNorm();

  gn.init(args).then(function() {
    var isAvailable = gn.isAvailable();
    if(!isAvailable.deviceOrientationAvailable) {
      logger({message:'Device orientation is not available.'});
    }

    if(!isAvailable.accelerationAvailable) {
      logger({message:'Device acceleration is not available.'});
    }

    if(!isAvailable.accelerationIncludingGravityAvailable) {
      logger({message:'Device acceleration incl. gravity is not available.'});
    } 

    if(!isAvailable.rotationRateAvailable) {
      logger({message:'Device rotation rate is not available.'});
    }

    start_gn();
  }).catch(function(e){
    console.log(e);
  });
}
function start_gn() {
      gn.start(gnCallBack);
    }
    function logger(data) {
      $('#error-message').append(data.message + "\n");
    }
function gnCallBack(data) {
    socket.emit('control', data.do.beta);
    console.log(data.do.beta);
    //timer = setTimeout(tilt, 100);
}
init_gn();
const Pong = {
    data: function(){
        return{};
    },
    created: function(){
        //timer = setTimeout(tilt, 1000);
    },
    beforeDestroy: function(){
        clearTimeout(timer);
    },
    template: `
        <div>
          <h1>Pong Controls</h1>
        </div>
    `,
};
