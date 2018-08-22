import sys, math, pygame
import flask,eventlet,pygame, random, pygame.freetype, io,numpy
from pygame.locals import *
from flask_socketio import SocketIO, send, emit
import webapp
class Point3D:
    def __init__(self, x = 0, y = 0, z = 0):
        self.x, self.y, self.z = float(x), float(y), float(z)
    def rotateX(self, angle):
        """ Rotates the point around the X axis by the given angle in degrees. """
        rad = angle * math.pi / 180
        cosa = math.cos(rad)
        sina = math.sin(rad)
        y = self.y * cosa - self.z * sina
        z = self.y * sina + self.z * cosa
        return Point3D(self.x, y, z)
    def rotateY(self, angle):
        """ Rotates the point around the Y axis by the given angle in degrees. """
        rad = angle * math.pi / 180
        cosa = math.cos(rad)
        sina = math.sin(rad)
        z = self.z * cosa - self.x * sina
        x = self.z * sina + self.x * cosa
        return Point3D(x, self.y, z)
    def rotateZ(self, angle):
        """ Rotates the point around the Z axis by the given angle in degrees. """
        rad = angle * math.pi / 180
        cosa = math.cos(rad)
        sina = math.sin(rad)
        x = self.x * cosa - self.y * sina
        y = self.x * sina + self.y * cosa
        return Point3D(x, y, self.z)
    def project(self, win_width, win_height, fov, viewer_distance):
        """ Transforms this 3D point to 2D using a perspective projection. """
        factor = fov / (viewer_distance + self.z)
        x = self.x * factor + win_width / 2
        y = -self.y * factor + win_height / 2
        return Point3D(x, y, 1)


def run():

    win_width = 96
    win_height = 34
    pygame.init()

    screen = pygame.Surface((win_width, win_height))
    clock = pygame.time.Clock()

    vertices = [
        Point3D(-1,1,-1),
        Point3D(1,1,-1),
        Point3D(1,-1,-1),
        Point3D(-1,-1,-1),
        Point3D(-1,1,1),
        Point3D(1,1,1),
        Point3D(1,-1,1),
        Point3D(-1,-1,1)
    ]

    # Define the vertices that compose each of the 6 faces. These numbers are
    # indices to the vertices list defined above.
    faces = [(0,1,2,3),(1,5,6,2),(5,4,7,6),(4,0,3,7),(0,4,5,1),(3,2,6,7)]
    angleX, angleY, angleZ = 0, 0, 0
    
    
    webapp.cnc.put("killall",False)
    webapp.sock.sleep(0.3)
    try:
        kill = webapp.cnc.get(False)
    except:
        pass
    kill = "no"
    while 1:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()

        pygame.event.pump()
        webapp.sock.sleep(0.02)
        screen.fill((0,0,0))

        # Will hold transformed vertices.
        t = []
        for v in vertices:
            # Rotate the point around X axis, then around Y axis, and finally around Z axis.
            r = v.rotateX(angleX).rotateY(angleY).rotateZ(angleZ)
            # Transform the point from 3D to 2D
            p = r.project(screen.get_width(), screen.get_height(), 256, 22)
            # Put the point in the list of transformed vertices
            t.append(p)

        for f in faces:
            pygame.draw.line(screen, (255,255,255), (t[f[0]].x, t[f[0]].y), (t[f[1]].x, t[f[1]].y))
            pygame.draw.line(screen, (255,255,255), (t[f[1]].x, t[f[1]].y), (t[f[2]].x, t[f[2]].y))
            pygame.draw.line(screen, (255,255,255), (t[f[2]].x, t[f[2]].y), (t[f[3]].x, t[f[3]].y))
            pygame.draw.line(screen, (255,255,255), (t[f[3]].x, t[f[3]].y), (t[f[0]].x, t[f[0]].y))
        angleX += 1
        angleY += 1
        angleZ += 1

        pixels = pygame.surfarray.array2d(pygame.transform.flip(pygame.transform.rotate(screen.convert(8),90),False,True))
        pix = pixels.astype(bool).tobytes()
        webapp.sock.emit('pixels',pix)
        try:
            kill = webapp.cnc.get(False)
        except:
            pass
        if(kill == "killall"):
            return
