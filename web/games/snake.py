import flask,eventlet,pygame,random,pygame.freetype,io,numpy
from pygame.locals import *
from flask_socketio import *
import webapp
def snake():
    webapp.cnc.put("killall",False)
    webapp.sock.sleep(0.3)
    ctrl = "right"
    try:
        kill = webapp.cnc.get(False)
    except:
        pass
    kill = "no"
    # pixels = io.StringIO("Test!")
    def collide(x1, x2, y1, y2):
        if x1==x2 and y1==y2:
            return True
        else:
            return False
    def die(screen, score):
        f=pygame.freetype.Font('games/fonts/blackout.fon')
        t=f.render('You Suck!',fgcolor=(255,255,255))
        s.blit(t[0], (13, 13))
        pixels = pygame.surfarray.array2d(pygame.transform.flip(pygame.transform.rotate(s.convert(8),90),False,True))
        pix = pixels.astype(bool).tobytes()
        webapp.sock.emit('pixels',pix)
        webapp.sock.sleep(2)
        eventlet.spawn(snake())
        pygame.quit()
        return

    xs = [5,4,3,2,1]
    ys = [16,16,16,16,16]
    dirs = 0
    score = 0
    applepos = (random.randint(4, 92), random.randint(4, 30))
    pygame.init()
    #scr=pygame.display.set_mode((384, 264))
    s=pygame.Surface((96, 34))
    pygame.display.set_caption('Snake')
    appleimage = pygame.Surface((1, 1))
    appleimage.fill((255,255,255))
    img = pygame.Surface((1, 1))
    img.fill((255,255,255))
    f=pygame.freetype.Font('games/fonts/blackout.fon')
    #clock = pygame.time.Clock()


    while True:
        pygame.event.pump()
        webapp.sock.sleep(0.08)
        try:
            ctrl = webapp.q.get(False)
        except:
            pass
        if ctrl=="up" and dirs != 0:dirs = 2
        elif ctrl=="down" and dirs != 2:dirs = 0
        elif ctrl=="left" and dirs != 1:dirs = 3
        elif ctrl=="right" and dirs != 3:dirs = 1
        i = len(xs)-1
        while i >= 2:
            if collide(xs[0], xs[i], ys[0], ys[i]):
                die(s, score)
                return
            i-= 1
        if collide(xs[0], applepos[0], ys[0], applepos[1]):
            score+=1
            xs.append(700)
            ys.append(700)
            applepos=(random.randint(4,92),random.randint(4,30))
        if xs[0] < 0 or xs[0] > 96 or ys[0] < 0 or ys[0] > 34:
            die(s, score)
            return
        i = len(xs)-1
        while i >= 1:
            xs[i] = xs[i-1];ys[i] = ys[i-1];i -= 1
        if dirs==0:ys[0] += 1
        elif dirs==1:xs[0] += 1
        elif dirs==2:ys[0] -= 1
        elif dirs==3:xs[0] -= 1
        s.fill((0,0,0))
        for i in range(0, len(xs)):
            s.blit(img, (xs[i], ys[i]))
        s.blit(appleimage, applepos)
        t=f.render(str(score),fgcolor=(255,255,255))
        s.blit(t[0], (2, 2))
        pixels = pygame.surfarray.array2d(pygame.transform.flip(pygame.transform.rotate(s.convert(8),90),False,True))
        pix = pixels.astype(bool).tobytes()
        webapp.sock.emit('pixels',pix)
        try:
            kill = webapp.cnc.get(False)
        except:
            pass
        if (kill == "killall"):
            return
    return
