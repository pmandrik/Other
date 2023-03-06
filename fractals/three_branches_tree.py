
from other import *

class three_branches_tree():
  def __init__(self, screen):
    self.screen = screen

    self.size = 150
    self.angle = 90 # . * 3.14 / 180.

    self.color = [0,0,0]
    self.width = 50
    self.iters = 7

    self.lines_to_draw  = [ [Vector2D(screen.get_rect().centerx, screen.get_rect().height), Vector2D(screen.get_rect().centerx, screen.get_rect().height - self.size)] ]
    self.next_lines_to_draw = []

  def tick(self):
    if len( self.lines_to_draw ) :
      line = self.lines_to_draw.pop()
      pygame.draw.line(self.screen, self.color, line[0], line[1], int(self.width))

      dv = line[1] - line[0]
      # angle = dv.Angle()
      dv0 = Vector2D( dv.x / dv.L() * self.size, dv.y / dv.L() * self.size )
      dv1 = Vector2D( dv.x / dv.L() * self.size, dv.y / dv.L() * self.size )
      dv2 = Vector2D( dv.x / dv.L() * self.size, dv.y / dv.L() * self.size )

      #print dv, angle

      if False : 
        for i in xrange( 360 / self.angle ) : 
          self.next_lines_to_draw += [ [line[1], line[1] + dv0.RotateD( self.angle ) ] ]   
        return

      self.next_lines_to_draw += [ [line[1], line[1] + dv0.RotateD( self.angle ) ] ]    
      self.next_lines_to_draw += [ [line[1], line[1] + dv1 ] ]    
      self.next_lines_to_draw += [ [line[1], line[1] + dv2.RotateD( -self.angle ) ] ]    
    else : 
      self.lines_to_draw, self.next_lines_to_draw = self.next_lines_to_draw, self.lines_to_draw
      self.size = max(2, self.size / 2) 
      self.width = max(1, self.width * 0.65)
      self.color[0] = randint(100, 200)
      self.color[1] = randint(100, 200)
      self.color[2] = randint(100, 200)
    pass

  def tick_windy(self):
    self.screen.fill((255,255,255))

    self.angle += 1
    self.angle_1 = 50  + 20 * math.cos(3.14 * 2 * self.angle / 360.)
    self.angle_2 = 0   + 30 * math.cos(3.14 * 2 * self.angle / 360.)
    self.angle_3 = -50 + 40 * math.cos(3.14 * 2 * self.angle / 360.)

    for i in xrange( self.iters ):
      for line in self.lines_to_draw :
        pygame.draw.line(self.screen, self.color, line[0], line[1], int(self.width))

        dv = line[1] - line[0]
        dv0 = Vector2D( dv.x / dv.L() * self.size, dv.y / dv.L() * self.size )
        dv1 = Vector2D( dv.x / dv.L() * self.size, dv.y / dv.L() * self.size )
        dv2 = Vector2D( dv.x / dv.L() * self.size, dv.y / dv.L() * self.size )

        self.next_lines_to_draw += [ [line[1], line[1] + dv0.RotateD( self.angle_1 ) ] ]    
        self.next_lines_to_draw += [ [line[1], line[1] + dv1.RotateD( self.angle_2 ) ] ]    
        self.next_lines_to_draw += [ [line[1], line[1] + dv2.RotateD( self.angle_3 ) ] ]    
      else : 
        self.lines_to_draw, self.next_lines_to_draw = self.next_lines_to_draw, []
        self.size = max(2, self.size * 0.75) 
        self.width = max(1, self.width * 0.65)
        #self.color[0] = randint(100, 200)
        #self.color[1] = randint(100, 200)
        #self.color[2] = randint(100, 200)
    self.size = 100
    self.color = [0,0,0]
    self.width = 50

    self.lines_to_draw  = [ [Vector2D(self.screen.get_rect().centerx, self.screen.get_rect().height), Vector2D(self.screen.get_rect().centerx, self.screen.get_rect().height - self.size)] ]
    self.next_lines_to_draw = []

def get_three_branches_tree_0(screen):
  screen.fill( (255,255,255) )
  answer = three_branches_tree(screen)
  return answer

def get_three_branches_tree_1(screen):
  screen.fill( (255,255,255) )
  answer = three_branches_tree(screen)
  answer.angle = 30
  return answer

def get_three_branches_tree_2(screen):
  screen.fill( (255,255,255) )
  answer = three_branches_tree(screen)
  answer.tick = answer.tick_windy
  answer.angle = 30
  return answer
