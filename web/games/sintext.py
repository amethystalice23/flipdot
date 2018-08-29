import pygame
import sys
import math
import flask,eventlet,pygame,random,pygame.freetype,io,numpy
from pygame.locals import *
from flask_socketio import *
import webapp
from time import localtime, strftime

import pyowm
owm = pyowm.OWM('080ffb507f0bd6b7c77c5f445f1d0db8')
# Lat-Lon for EMF Camp, do not chnage!
try:
    observation = owm.weather_around_coords(52.029827,-2.387467)[0]
    w = observation.get_weather()
    temp = w.get_temperature('celsius')['temp']
except:
    temp = 0
class SinusTextPy(object):
    """Sinus wave scroll text"""

    def __init__(self, surface, text, hpos, amplitude, frequency, color, size=30):
        """
        (pygame.Surface) surface to draw on
        (string) text - text to draw
        (int) hpos - horizontal position on y axis
        (int) amplitude - amplitude of sinus wave
        (int) frequency - frequency of sinus wave
        (pygame.Color) color - color of font
        (int) size - size of font
ww
`"""
        self.surface = surface
        # prepend an append some spaces
        appendix = " " * int(self.surface.get_width() / size)
        self.text = appendix + text + appendix
        self.hpos = hpos
        self.amplitude = amplitude
        self.frequency = frequency
        self.color = color
        self.size = size
        self.font = None
        f = pygame.freetype.Font("games/fonts/outline_pixel-7.ttf",20)
        t = f.render(self.text, fgcolor=(255,255,255))
        self.text_surface = pygame.Surface((t[1][2],t[1][3]))
        self.text_surface.blit(t[0], (0,0))
        # position in rendered string
        self.position = 0
        # radian to degree
        self.factor = 2 * math.pi / self.surface.get_width()



    def update(self, hpos=None):
        """
        update every frame
        (int)hpos y axis offset
        """
        if hpos is not None:
            self.hpos = hpos
        for offset in range(self.text_surface.get_width()):
            self.surface.blit( \
                self.text_surface, \
                (0 + offset, self.hpos + math.sin(offset * self.frequency * self.factor) * self.amplitude), \
                (self.position + offset, 0, 1, self.size) \
            )
        if self.position < self.text_surface.get_width():
            self.position += 1
        else:
            self.position = 0

def run():
    surface = pygame.Surface((96, 34))
    pygame.init()
    spheres = (
        SinusTextPy(surface, "    * *  SWANSEA HACKSPACE  * *  A new life awaits you in the Off-World Colonies! ...", 4, 15, 0.3, pygame.Color(255,255,255)),
        )
    sendbitmap = webapp.sendbitmap()
    while True:
        pygame.event.pump()
        webapp.sock.sleep(0.03)
        surface.fill((0, 0, 0, 255))
        for thing in spheres:
            thing.update(hpos=None)
        f=pygame.freetype.Font("games/fonts/boxxy.bdf", 14)
        g=pygame.freetype.Font("games/fonts/uushi.bdf", 11)
        t=f.render(strftime("%H:%M:%S", localtime()),fgcolor=(255,255,255))
        surface.blit(t[0], (47, 0))
        t=g.render("{0:.1f}".format(temp)+"°",fgcolor=(255,255,255))
        surface.blit(t[0], (1, 25))
        sendbitmap.pixels(surface)
    return
