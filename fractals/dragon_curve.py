
from other import *

class dragon():
  def __init__(self, screen):
    self.screen = screen

    self.center = Vector2D(screen.get_rect().centerx, screen.get_rect().centery+100)
    
    self.line        = []
    self.line_to_add = []    
    self.angle = 90

    self.color = [0,0,0]
    self.size = 5

    self.add_line(Vector2D(), Vector2D(self.size, 0))
    self.add_line(Vector2D(), Vector2D(0, self.size))

  def add_line(self, p1, p2):
    #print self.line
    plast = self.center
    if self.line : 
      line = self.line[-1]
      plast = line[1]
      #print plast
    pnext = plast + (p1-p2).RotateD(self.angle)
    pygame.draw.line(self.screen, self.color, pnext, plast, 1)
    self.line += [ [plast, pnext ] ]

  def tick(self):
    if len( self.line_to_add ):
      line = self.line_to_add[ -1 ]
      self.line_to_add = self.line_to_add[:-1]

      p1 = line[0]
      p2 = line[1]
      self.add_line( p1, p2 )
    else : 
      self.color[0] = randint(100, 200)
      self.color[1] = randint(100, 200)
      self.color[2] = randint(100, 200)
      self.line_to_add = self.line[:]


def get_dragon_0(screen):
  screen.fill( (5, 55, 55) )
  answer = dragon(screen)
  return answer
