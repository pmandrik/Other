
import pygame
from pygame.locals import *

from pythagoras_tree import *
from koch_curve   import *
from sierpinski   import *
from dragon_curve import *
from gosper_curve import *
from three_branches_tree import * 

SCREENRECT = Rect(0,0,640,480)
FPS = 60

def main():
  pygame.init()
	##>-------<##
  winstyle = 0  # |FULLSCREEN
  bestdepth = pygame.display.mode_ok(SCREENRECT.size, winstyle, 32)
  screen = pygame.display.set_mode(SCREENRECT.size, winstyle, bestdepth)
	##>-------<##
  clock = pygame.time.Clock()

  if   True : core = get_setup_0( screen )
  elif True : core = get_setup_1( screen )
  elif True : core = get_koch_1( screen )
  elif True : core = get_koch_0( screen )
  elif True : core = get_sierp_0( screen )
  elif True : core = get_sierp_1( screen )
  elif True : core = get_dragon_0( screen )
  elif True : core = get_gosper_0( screen )
  elif True  : core = get_three_branches_tree_0( screen )
  elif True  : core = get_three_branches_tree_1( screen )
  elif True  : core = get_three_branches_tree_2( screen )

  game = True
  while game :
    core.tick()
    pygame.display.update()
    clock.tick(FPS)

    for event in pygame.event.get():
      if event.type == QUIT: return False
      if event.type == KEYDOWN and event.key == K_ESCAPE: return False
  pygame.quit()

##---------------------//spure//---------------------##
if __name__ == "__main__":
	main()
