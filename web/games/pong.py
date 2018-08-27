import flask, pygame, random, pygame.freetype, io, numpy, webapp, games, eventlet, time
from pygame.locals import *
from flask_socketio import *
BLACK = (0 ,0, 0)
WHITE = (255, 255, 255)

class Ball(pygame.sprite.Sprite):
    # Constructor. Pass in the color of the block, and its x and y position
    def __init__(self):
        # Call the parent class (Sprite) constructor
        super().__init__()

        # Create the image of the ball
        self.image = pygame.Surface([2,2])

        # Color the ball
        self.image.fill(WHITE)

        # Get a rectangle object that shows where our image is
        self.rect = self.image.get_rect()

        # Get attributes for the height/width of the screen
        self.screenheight = 34 #pygame.display.get_surface().get_height()
        self.screenwidth = 96 #pygame.display.get_surface().get_width()

        # Speed in pixels per cycle
        self.speed = 0

        # Floating point representation of where the ball is
        self.x = 0
        self.y = 0

        # Direction of ball in degrees
        self.direction = 0

        # Height and width of the ball
        self.width = 2
        self.height = 2

        # Set the initial ball speed and position
        self.reset()

    def reset(self):
        self.x = random.randrange(0,96)
        self.y = 31
        self.speed=1.0

        # Direction of ball (in degrees)
        self.direction = random.randrange(-45,45)

        # Flip a 'coin'
        if random.randrange(2) == 0 :
            # Reverse ball direction, let the other guy get it first
            self.direction += 180
            self.y = 3

    # This function will bounce the ball off a horizontal surface (not a vertical one)
    def bounce(self,diff):
        self.direction = (180-self.direction)%360
        self.direction -= diff

        # Speed the ball up
        self.speed *= 1.1

    # Update the position of the ball
    def update(self):
        # Sine and Cosine work in degrees, so we have to convert them
        direction_radians = numpy.radians(self.direction)

        # Change the position (x and y) according to the speed and direction
        self.x += self.speed * numpy.sin(direction_radians)
        self.y -= self.speed * numpy.cos(direction_radians)

        if self.y < 0:
            self.reset()

        if self.y > 34:
            self.reset()

        # Move the image to where our x and y are
        self.rect.x = self.x
        self.rect.y = self.y

        # Do we bounce off the left of the screen?
        if self.x <= 0:
            self.direction = (360-self.direction)%360
            print(self.direction)
            #self.x=1

        # Do we bounce of the right side of the screen?
        if self.x > self.screenwidth-self.width:
            self.direction = (360-self.direction)%360

# This class represents the bar at the bottom that the player controls
class Player(pygame.sprite.Sprite):
    # Constructor function
    def __init__(self, y_pos):
        # Call the parent's constructor
        super().__init__()
        self.ctrl = 50
        self.width=10
        self.height=2
        self.image = pygame.Surface([self.width, self.height])
        self.image.fill(WHITE)

        # Make our top-left corner the passed-in location.
        self.rect = self.image.get_rect()
        self.screenheight = 34 #pygame.display.get_surface().get_height()
        self.screenwidth = 96 #pygame.display.get_surface().get_width()

        self.rect.x = 0
        self.rect.y = y_pos

    # Update the player
    def update(self):

        # This gets the position of the axis on the game controller
        # It returns a number between -1.0 and +1.0

        try:
            self.ctrl = float(webapp.padinput)
        except:

            pass

        
        # Move x according to the axis. We multiply by 15 to speed up the movement.
        OldMin = float(60)
        OldMax = float(10)
        NewMin = float(10)
        NewMax = float(96)
        OldRange = (OldMax - OldMin)  
        NewRange = (NewMax - NewMin)  
        NewValue = int((((self.ctrl - OldMin) * NewRange) / OldRange) + NewMin)
        self.rect.x=int(NewValue)

        # Make sure we don't push the player paddle off the right side of the screen
        if self.rect.x > self.screenwidth - self.width:
            self.rect.x = self.screenwidth - self.width


def run():
    score1 = 0
    score2 = 0
    pygame.init()
    screen = pygame.Surface((96,34))
    font = pygame.font.Font(None, 10)
    ball = Ball()
    # Create a group of 1 ball (used in checking collisions)
    balls = pygame.sprite.Group()
    balls.add(ball)

    player1 = Player(0)
    player2 = Player(32)

    movingsprites = pygame.sprite.Group()
    movingsprites.add(player1)
    movingsprites.add(player2)
    movingsprites.add(ball)

    sendbitmap = webapp.sendbitmap()
    ctrl = 50
    while True:
        pygame.event.pump()
        webapp.sock.sleep(1/36)
        screen.fill(BLACK)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                exit_program = True

        # Stop the game if there is an imbalance of 3 points
        #if abs(score1 - score2) > 3:
        #    done = True

 
        player1.update()
        player2.update()
        ball.update()

        # If we are done, print game over
        if False:
            text = font.render("Game Over", 1, WHITE)
            textpos = text.get_rect(centerx=screen.get_width()/2)
            textpos.top = 50
            screen.blit(text, textpos)

        # See if the ball hits the player paddle
        if pygame.sprite.spritecollide(player1, balls, False):
            # The 'diff' lets you try to bounce the ball left or right depending where on the paddle you hit it
            diff = (player1.rect.x + player1.width/2) - (ball.rect.x+ball.width/2)

            # Set the ball's y position in case we hit the ball on the edge of the paddle
            ball.y = 3
            ball.bounce(diff)
            score1 += 1

        # See if the ball hits the player paddle
        if pygame.sprite.spritecollide(player2, balls, False):
            # The 'diff' lets you try to bounce the ball left or right depending where on the paddle you hit it
            diff = (player2.rect.x + player2.width/2) - (ball.rect.x+ball.width/2)

            # Set the ball's y position in case we hit the ball on the edge of the paddle
            ball.y = 31
            ball.bounce(diff)
            score2 += 1

        # Print the score
        scoreprint = "Player 1: "+str(score1)
        text = font.render(scoreprint, 1, WHITE)
        textpos = (12, 0)
        #screen.blit(text, textpos)

        scoreprint = "Player 2: "+str(score2)
        text = font.render(scoreprint, 1, WHITE)
        textpos = (12, 18)
        #screen.blit(text, textpos)

        movingsprites.draw(screen)
        sendbitmap.pixels(screen)
