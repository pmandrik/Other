
from other import *




class sierpinsky():

  class Point():
    def __init__(self, pos, size, color):
      self.pos = pos
      self.size = size
      self.color = color

    def draw(self):
      pygame.draw.rect(self.screen, self.color, Rect(self.pos.x, self.pos.y, self.size, self.size) )

  def __init__(self, screen):
    self.screen = screen
    self.Point.screen = self.screen

    rect = screen.get_rect()
    self.color = [0,0,0]
    self.points = [ self.Point( Vector2D((rect.width-rect.height)*0.5, 0), rect.height, self.color ) ]

  def get_next(self, p):
    answer = []
    ssize = max(p.size * 0.33333, 2)
    spos  = p.pos
    # print spos
    for i in xrange(3):
      for j in xrange(3):
        p = self.Point( spos + Vector2D(ssize*i, ssize*j), ssize, p.color)
        if i == 1 and j == 1 :
          p.draw()
        else : 
          answer += [ p ]
    return answer

  def get_next_1(self, p):
    answer = []
    ssize = max(p.size * 0.33333, 2)
    spos  = p.pos
    # print spos
    for i in xrange(3):
      for j in xrange(3):
        p = self.Point( spos + Vector2D(ssize*i, ssize*j), ssize, p.color)
        if i == 1 and j == 1 :
          p.draw()
          if p.color[0] < 100 : p.color[0] = 200
          else : p.color[0] = 0
          # answer += [ p ]
        else : 
          p.draw()
          if p.color[1] < 100 : p.color[1] = 200
          else : p.color[1] = 0
          answer += [ p ]
    return answer

  def tick(self):
    p = self.points[0]
    self.points = self.points[1:] + self.get_next( p )



def get_sierp_0(screen):
  screen.fill((255,255,255))
  answer = sierpinsky( screen )
  return answer
    
def get_sierp_1(screen):
  screen.fill((255,255,255))
  answer = sierpinsky( screen )
  answer.get_next = answer.get_next_1
  return answer








