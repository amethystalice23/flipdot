from flask import *
from flask_socketio import *
from eventlet import *
from games.snake import *
from games.cube import *
from games.lunarlander import *
from games.sintext import *
pool = eventlet.GreenPool()
eventlet.monkey_patch()
app = Flask(__name__)
sock = SocketIO(app,async_handlers=True)
q = eventlet.queue.Queue()
cnc = eventlet.queue.Queue()

@app.route('/')
def index():
    return render_template('index.html')

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
    if (json == "snake"):
        eventlet.spawn(snake())
    if (json == "lunar"):
        eventlet.spawn(lunar())
    if (json == "sintext"):
        eventlet.spawn(sintext())
    if (json == "cube"):
        eventlet.spawn(Simulation().run())
