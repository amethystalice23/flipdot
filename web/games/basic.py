import flask,eventlet,pygame,webapp
from pygame.locals import *
from flask_socketio import *

def run():
    pygame.init()
    surface = pygame.Surface((96, 34))
    surface.fill((0, 0, 0, 255))
    sendbitmap = webapp.sendbitmap()
    pos = 3264
    while True:
        pygame.event.pump()
        webapp.sock.sleep(1/200)
        c=int(float(pos%6528)/3264)*255
        surface.set_at((pos%96,int(float(pos%3264)/96)),(c,c,c))
        pos+=1
        sendbitmap.pixels(surface)
