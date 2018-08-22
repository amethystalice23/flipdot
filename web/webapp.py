import eventlet, games, importlib, glob, importlib, pygame
from flask import *
from flask_socketio import *
from functools import wraps
from eventlet import *

# Autoload game modules from directory
gamelist = []
print("")
for f in glob.iglob("games/*.py"):
    modname = f.split(".")[0].split("/")[1]
    gamelist.append(modname)
    print("  Loading ... \x1b[32m"+modname+"\033[0m")
    mod = importlib.import_module("."+modname, "games")
print("\n\033[1m\033[93m  F L I P D O T -- A C T I V A T E D\033[0m\n")

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
    return render_template('index.html',gamelist=gamelist)

@app.route('/')
def index():
    return render_template('index.html',gamelist=gamelist)

@sock.on('client_connected')
def handle_client_connect_event(json):
    print('received json: {0}'.format(str(json)))


@sock.on('message')
def handle_json_button(json):
    # it will forward the json to all clients.
    send(json, json=True)

@sock.on('control')
def handle_json_button(json):
    # it will forward the json to all clients.
    q.put(json,False)
    sock.sleep(0.1)

@sock.on('alert_button')
def handle_alert_event(json):
    eventlet.spawn(getattr(games, json).run())
