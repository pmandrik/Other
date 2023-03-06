 
from other import *

class koch():
  def __init__(self, screen):
    self.screen = screen
    self.center = Vector2D(screen.get_rect().centerx, screen.get_rect().centery)

    self.size = screen.get_rect().height * 0.5
    self.color = (0,0,0)

    self.points = []
    self.new_points = []
    self.angle = 0
    self.add_points_start( self.center, self.size, self.angle )
  
  def add_points_start(self, pos, size, angle):
    p1 = pos + Vector2D(size, 0).RotateD( angle )
    p2 = pos + Vector2D(size, 0).RotateD( angle + 120 )
    p3 = pos + Vector2D(size, 0).RotateD( angle + 240 )
    self.new_points += [ [p1, p2, p3] ]
    pygame.draw.line(self.screen, self.color, p1.GetTuple(), p2.GetTuple(), 1)
    pygame.draw.line(self.screen, self.color, p2.GetTuple(), p3.GetTuple(), 1)
    pygame.draw.line(self.screen, self.color, p3.GetTuple(), p1.GetTuple(), 1)

  def add_point_from_line(self, p01, p02):
    p1 = p01 + (p02 - p01)*0.33
    p3 = p01 + (p02 - p01)*0.66
    perp = (p01 - p02).RotateD( 270 )
    p2 = (p01 + p02)*0.5 - perp * 0.33

    perp = (p01 - p02).RotateD( 90 )
    p2_alt = (p01 + p02)*0.5 - perp * 0.33

    if (p2 - self.center).L() < (p2_alt - self.center).L():
      p2 = p2_alt

    self.new_points += [ [p01, p1, p2],  [p1, p2, p3], [p2, p3, p02] ]

    pygame.draw.line(self.screen, self.color, p1.GetTuple(), p2.GetTuple(), 1)
    pygame.draw.line(self.screen, self.color, p2.GetTuple(), p3.GetTuple(), 1)
    pygame.draw.line(self.screen, self.color, p3.GetTuple(), p1.GetTuple(), 1)

    # pygame.draw.polygon( self.screen, self.color, [p1.GetTuple(), p2.GetTuple(), p3.GetTuple()] )

  def tick(self):
    # self.screen.fill((255, 255, 255))
    points = self.new_points[0]
    self.new_points = self.new_points[1:]

    p1 = points[0]
    p2 = points[1]
    p3 = points[2]
    self.points += [ p1, p2, p3 ]

    self.add_point_from_line( p1, p2 )
    self.add_point_from_line( p2, p3 )
    self.add_point_from_line( p3, p1 )

class koch_alt( koch ):  
  def add_points_start(self, pos, size, angle):
    p1 = pos + Vector2D(size, 0).RotateD( angle )
    p2 = pos + Vector2D(size, 0).RotateD( angle + 120 )
    p3 = pos + Vector2D(size, 0).RotateD( angle + 240 )
    self.new_points += [ [p1, p2], [p2, p3], [p3, p1] ]
    pygame.draw.line(self.screen, self.color, p1.GetTuple(), p2.GetTuple(), 1)
    pygame.draw.line(self.screen, self.color, p2.GetTuple(), p3.GetTuple(), 1)
    pygame.draw.line(self.screen, self.color, p3.GetTuple(), p1.GetTuple(), 1)

  def add_point_from_line(self, p01, p02):
    p1 = p01 + (p02 - p01)*0.33
    p3 = p01 + (p02 - p01)*0.66
    perp = (p01 - p02).RotateD( 270 ) # + 45 * math.cos(p01.x/640. * 3.14 *2) * 4 )
    p2 = (p01 + p02)*0.5 - perp * (0.333333 ) # + 0.2*math.sin(p01.y/640. * 3.14 *2) * 1)

    perp = (p01 - p02).RotateD( 90 )
    p2_alt = (p01 + p02)*0.5 - perp * 0.333333

    #if (p2 - center).L() < (p2_alt - center).L():
    #p2 = p2_alt

    self.new_points += [ [p01, p1],  [p1, p2], [p2, p3], [p3, p02] ]

    pygame.draw.line(self.screen, (255,255,255), p3.GetTuple(), p1.GetTuple(), 3)
    pygame.draw.line(self.screen, self.color, p1.GetTuple(), p2.GetTuple(), 1)
    pygame.draw.line(self.screen, self.color, p2.GetTuple(), p3.GetTuple(), 1)

  def draw(self):
    for i in xrange(len(self.points)):
      p1 = self.points[i]
      if i != len(self.points)-1: p2 = self.points[i+1]
      else : p2 = self.points[0]

      pygame.draw.line(self.screen, self.color, p1, p2, 1)

  def tick(self):
    # self.screen.fill((255, 255, 255))
    points = self.new_points[0]
    self.new_points = self.new_points[1:]

    p1 = points[0]
    p2 = points[1]
    self.points += [ p2 ]
    # self.draw()

    self.add_point_from_line( p1, p2 )

def get_koch_0(screen):
  screen.fill((255, 255, 255))
  answer = koch( screen )
  return answer  

def get_koch_1(screen):
  screen.fill((255, 255, 255))
  answer = koch_alt( screen )
  return answer  



