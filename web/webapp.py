import eventlet, games, importlib, glob, importlib, pygame, socket
from flask import *
from flask_socketio import *
from functools import wraps
from eventlet import *
from time import sleep

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
sock = SocketIO(app,async_handlers=True)
q = eventlet.queue.Queue()
cnc = eventlet.queue.Queue()

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
    sock.emit("clientpush",json)
    eventlet.spawn(getattr(games, json).run())


print("\n\033[1m\033[93m  F L I P D O T -- A C T I V A T E D\033[0m\n")
