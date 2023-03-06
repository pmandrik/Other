
from other import *

class gosper_curve():
  def __init__(self, screen):
    self.screen = screen

    self.lsystem = L_System("A")
    self.lsystem.add_rule( "A", "A-B--B+A++AA+B-" )
    self.lsystem.add_rule( "B", "+A-BB--B-A++A+B" )

    self.step = 0
    self.color = [0,0,0]
    self.size = 8
    self.angle = 0
    self.angle_right = 60
    self.angle_left  = 60
    self.position = Vector2D(screen.get_rect().centerx+300, screen.get_rect().centery-100)

  def draw(self):
    s = self.lsystem.state[ self.step ]
    if s == "-" : 
      self.angle -= self.angle_right
    elif s == "+" : 
      self.angle += self.angle_left
    else : 
      position_next = self.position + Vector2D(0, self.size).RotateD( self.angle )
      pygame.draw.line(self.screen, self.color, self.position, position_next, 3)
      self.position = position_next
      if self.position.x < 0 : self.position.x += self.screen.get_rect().width
      elif self.position.x > self.screen.get_rect().width : self.position.x -= self.screen.get_rect().width
      elif self.position.y < 0 : self.position.y += self.screen.get_rect().height
      elif self.position.y > self.screen.get_rect().height : self.position.y -= self.screen.get_rect().height

    self.step += 1
    if self.step >= len(self.lsystem.state):
      self.step = 0
      return False
    return True

  def tick(self):
      if not self.draw() :
        self.color[0] = randint(100, 200)
        self.color[1] = randint(100, 200)
        self.color[2] = randint(100, 200)
        self.lsystem.tick()
      else : 
        return
        self.color[0] = randint(100, 200)
        self.color[1] = randint(100, 200)
        self.color[2] = randint(100, 200)
        

def get_gosper_0(screen):
  screen.fill((25, 35, 45))
  answer = gosper_curve( screen )
  return answer  

def get_gosper_1(screen):
  screen.fill((25, 25, 45))
  answer = gosper_curve( screen )
  answer.angle_right = 50
  answer.angle_left  = 50
  return answer  




