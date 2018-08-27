import eventlet, sqlalchemy, games, importlib, glob, importlib, pygame, socket, time, binascii, numpy
from flask import *
from flask_socketio import *
from functools import wraps
from eventlet import *
from time import sleep
from flask_sqlalchemy import SQLAlchemy


framerate = 18

# Print IP Address
print("\n  \033[1m\033[7m "+(([ip for ip in socket.gethostbyname_ex(socket.gethostname())[2] if not ip.startswith("127.")] or [[(s.connect(("8.8.8.8", 53)), s.getsockname()[0], s.close()) for s in [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1]]) + ["no IP found"])[0]+" \033[0m")

# Autoload game modules from directory
gamelist = []
print("")
for f in glob.iglob("games/*.py"):
    modname = f.split(".")[0].split("/")[1]
    gamelist.append(modname)
    print("  Loading ... \x1b[32m"+modname+"\033[0m")
    mod = importlib.import_module("."+modname, "games")

pool = eventlet.GreenPool()
eventlet.monkey_patch()
app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:////tmp/gamestate.db'
db = SQLAlchemy(app)
sock = SocketIO(app,async_handlers=True)
q = eventlet.queue.Queue()
padinput = 0
cnc = eventlet.queue.Queue()

currentgame = eventlet.spawn(games.cube.run)

def check_auth(username, password):
    return username == 'admin' and password == 'electronics'
def authenticate():
    return Response(
            'Go away and break something else. :)', 401, {'WWW-Authenticate': 'Basic realm="Login Required"'})
def requires_auth(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        auth = request.authorization
        if not auth or not check_auth(auth.username, auth.password):
            return authenticate()
        return f(*args, **kwargs)
    return decorated

@app.route('/admin')
@requires_auth
def admin():
    return render_template('admin.html',gamelist=gamelist)

@app.route('/')
def index():
    return render_template('index.html')


@app.route('/games.js')
def gamesjs():
    # Collect Game View JS
    gamejs = ""
    for f in glob.iglob("games/*.js"):
        with open(f, 'r') as game:
            data = game.read()
            gamejs += data + "\n\n"
            game.close()
    return Response(gamejs, mimetype='text/javascript')

@sock.on('client_connected')
def handle_client_connect_event(json):
    print('Client: {0}'.format(str(json)))

@sock.on('modeselect')
def handle_alert_event(json):
    print(json)
    global currentgame
    sock.emit("clientpush",json)
    currentgame.kill()
    sock.sleep(0.3)
    currentgame = eventlet.spawn(getattr(games, json).run)
@sock.on('control')
def handle_json_button(json):
    # it will forward the json to all clients.
    q.put(json,False)
    global padinput
    padinput = json
    sock.sleep(0.01)
class sendbitmap:
    global framerate
    def __init__(self):
        self.lastframe = time.perf_counter()
        self.panels = [pygame.Surface((32,16)) for i in range(6)]
    def canvastobits(self,canvas):
        return pygame.surfarray.array2d(pygame.transform.flip(pygame.transform.rotate(canvas.convert(8),90),False,True)).astype(bool)
    def pixels(self,p):
        if (time.perf_counter() - self.lastframe > 1/framerate):
            sock.emit('pixels',self.canvastobits(p).tobytes())
            self.panels[0].blit(p,[0,0],[0,0,32,16])
            self.panels[1].blit(p,[0,0],[32,0,32,16])
            self.panels[2].blit(p,[0,0],[64,0,32,16])
            self.panels[3].blit(p,(0,0),(64,16,32,16))
            self.panels[3] = pygame.transform.rotate(self.panels[3],180)
            self.panels[4].blit(p,(0,0),(32,16,32,16))
            self.panels[4] = pygame.transform.rotate(self.panels[4],180)
            self.panels[5].blit(p,(0,0),(0,16,32,16))
            self.panels[5] = pygame.transform.rotate(self.panels[5],180)

            #for addr,panel in enumerate(self.panels):
                #print(str(addr)+"B"+binascii.hexlify(numpy.packbits(self.canvastobits(panel))).decode('ascii').upper())
                #print(numpy.array(self.canvastobits(i).astype(int)))
            #print("*C\n")

            self.lastframe = time.perf_counter()
print("\n\033[1m\033[93m  F L I P D O T -- A C T I V A T E D\033[0m\n")
