import pygame,sys,array
from random import*
from pygame import*
init()
from math import*
import flask,eventlet,pygame,random,pygame.freetype,io,numpy
from pygame.locals import *
from flask_socketio import *
import webapp
def lunar():
    webapp.cnc.put("killall",False)
    webapp.sock.sleep(0.3)
    ctrl = "no"
    try:
        kill = webapp.cnc.get(False)
    except:
        pass
    kill = "no"
    c=''
    d='LANDING'
    e='CRASH'
    b=(0,0,0)
    dn=False
    s=96
    n=255
    z=3
    Z=s-z
    sc=display.set_mode((96,96))
    mh=16
    w=(255,255,255)
    lp=14
    pl=5
    fuel=400
    ph=randint(0,s)
    am=s/5
    fs=s/32
    cl=cr=b
    ss=c
    y=z
    T=11025
    x=randint(z,Z)
    u=v=0
    r=5
    cg=w
    wi=n
    q=127
    p=-q
    gs=c
    P=0.01
    pygame.font.init()
    ft=pygame.font.SysFont('courier',10)
    N=n*4
    mn=32
    st=array.array('b',(max(p,min(q,int(T*sin(i*P))))for i in range(N))).tostring()
    se=array.array('b',(randint(p,q)for i in range(N))).tostring()
    mx=[]
    my=[]
    a=2
    for i in range(mn+1):
        mx.append(z*i)
        my.append(int(randint(-mh,0)+am*(4-sin((i+ph)/5.)))-fs)
    mx.append(s)
    my.append(randint(s-mh,s))
    mx[pl]=mx[pl-1]+lp
    my[pl]=my[pl-1]
    while dn == False:
        pygame.event.pump()
        webapp.sock.sleep(0.08)
        try:
            ctrl = webapp.q.get(False)
        except:
            pass

        if ctrl=="up":
            x=randint(z,Z)
            y=z
            u=v=0
            r=5
            cg=w
            wi=n
            fuel=s
            gs=c
        if ctrl=="down" and fuel>0:
            v=v-a
            fuel=fuel-5
            cl=w
            cr=w
            ss=se
        if ctrl=="left" and fuel>0:
            u=u+a
            fuel=fuel-5
            cl=w
            ss=se
        if ctrl=="right" and fuel>0:
            u=u-a
            fuel=fuel-5
            cr=w
            ss=se
        if gs==c and (x<0 or x>s):
            x=x-(abs(x)/x)*s
        if gs==c:
            v=v+1
            x=(10*x+u)/10
            y=(10*y+v)/10
        if (y+8)>=my[pl] and x>mx[pl-1] and x<mx[pl] and v<30:
            gs=d
        for i in range(mn):
            if gs==c and mx[i]<=x and mx[i+1]>=x and (my[i]<=y or my[i+1]<=y):
                cr=1
                cg=b
                gs=e
        sc.fill(b)
        draw.line(sc,w,(mx[pl-1],my[pl-1]),(mx[pl],my[pl]),3)
        if wi>10 and gs==e:
            r=r+z
            wi=wi-z
            ss=st
        for i in range(50):
            ax=sin(i/8.)
            ay=cos(i/8.)
            draw.line(sc,(wi,wi,wi),(x+r*ax,y+r*ay),(x+r*ax,y+r*ay))
        draw.line(sc,cg,(x+3,y+3),(x+4,y+6))
        draw.line(sc,cg,(x-3,y+3),(x-4,y+6))
        draw.line(sc,cl,(x+2,y+5),(x,y+9))
        draw.line(sc,cr,(x-2,y+5),(x,y+9))
        txt='FUEL %3d     ALT %3d     VERT SPD %3d     HORZ SPD %3d'%(fuel,s-y,v,u)
        sp=ft.render(txt,0,w)
        sc.blit(sp,(0,s-12))
        cl=b
        cr=b
        for i in range(mn):
            draw.line(sc,w,(mx[i],my[i]),(mx[i+1],my[i+1]))
        sp=ft.render(gs,0,w)
        sc.blit(sp,(s/3,s/2))
        display.flip()
        ss=c

        pixels = pygame.surfarray.array2d(pygame.transform.flip(pygame.transform.rotate(sc.convert(8),90),False,True))
        pix = pixels.astype(bool).tobytes()
        webapp.sock.emit('pixels',pix)
        try:
            kill = webapp.cnc.get(False)
        except:
            pass
        if (kill == "killall"):
            return
    pygame.quit()
