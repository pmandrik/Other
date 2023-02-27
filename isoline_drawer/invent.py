
####fun####-----------------------------------------####fun####
import sys
from os.path import join
from random import randrange, randint, choice, uniform
from math import sin, cos, pi, sqrt

import numpy as np

import pygame
from pygame.locals import *

from copy import copy

from digmath import *

SCREEN_WIDTH, SCREEN_HEIGHT = 800, 700
SW2, SH2 = SCREEN_WIDTH/2, SCREEN_HEIGHT/2
SCREENRECT = Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)
FPS=50

class Gui():
	def __init__(self):
		upsize = 60

		self.back_up = pygame.Surface((SCREEN_WIDTH, upsize))
		self.back_up.fill((125,0,0))
		self.back_up.set_colorkey((0,0,0))

		slots = 10
		slot_size = 40
		shift = (upsize - slot_size)/2
		slot_w = SCREEN_WIDTH / 10
		for i in xrange(slots):
			top = Vector2D(i * slot_w, 0)
			color = (255, 255, 255)

			l1 = Vector2D(shift, shift)
			l2 = Vector2D(shift, shift + 10)
			pygame.draw.line(self.back_up, color, (top + l1).Tuple(), (top + l2).Tuple(), 3)

			l1 = Vector2D(shift, shift)
			l2 = Vector2D(shift + 10, shift)
			pygame.draw.line(self.back_up, color, (top + l1).Tuple(), (top + l2).Tuple(), 3)
			#----------
			l1 = Vector2D(shift, upsize-shift)
			l2 = Vector2D(shift, upsize-shift-10)
			pygame.draw.line(self.back_up, color, (top + l1).Tuple(), (top + l2).Tuple(), 3)

			l1 = Vector2D(shift, upsize-shift)
			l2 = Vector2D(shift + 10, upsize-shift)
			pygame.draw.line(self.back_up, color, (top + l1).Tuple(), (top + l2).Tuple(), 3)
			#----------
			l1 = Vector2D(slot_w - shift, shift)
			l2 = Vector2D(slot_w - shift, shift + 10)
			pygame.draw.line(self.back_up, color, (top + l1).Tuple(), (top + l2).Tuple(), 3)
			
			l1 = Vector2D(slot_w - shift, shift)
			l2 = Vector2D(slot_w - shift - 10, shift)
			pygame.draw.line(self.back_up, color, (top + l1).Tuple(), (top + l2).Tuple(), 3)
			#----------
			l1 = Vector2D(slot_w - shift, upsize-shift)
			l2 = Vector2D(slot_w - shift, upsize-shift-10)
			pygame.draw.line(self.back_up, color, (top + l1).Tuple(), (top + l2).Tuple(), 3)

			l1 = Vector2D(slot_w - shift, upsize-shift)
			l2 = Vector2D(slot_w - shift - 10, upsize-shift)
			pygame.draw.line(self.back_up, color, (top + l1).Tuple(), (top + l2).Tuple(), 3)

		self.back_down = pygame.Surface((SCREEN_WIDTH, 125))
		self.back_down.fill((125,0,0))
		self.back_down.set_colorkey((0,0,0))

	def Tick(self, screen):
		screen.fill((0, 0, 0))
		screen.blit(self.back_up, (0, 0))
		screen.blit(self.back_down, (0, 700-125))

####fun####-----------------------------------------####fun####
def main():
	#>----<#
	#print "init pygame ... "
	pygame.init()
	#>----<#
	winstyle = 0#FULLSCREEN | HWSURFACE | DOUBLEBUF
	bestdepth = pygame.display.mode_ok(SCREENRECT.size, winstyle, 32)
	screen = pygame.display.set_mode(SCREENRECT.size, winstyle, bestdepth)
	background = pygame.Surface(SCREENRECT.size)
	background.set_alpha(255)

	#>----<#
	scr_number = 0
	clock = pygame.time.Clock()
	gc = Gui()
	game = True
	while game :
		#>----<#
		keystate = pygame.key.get_pressed()
		events = pygame.event.get()
		#>----<#
		#gc.Tick(screen)

		background.fill((255,255,255))
		gc.Tick(background)
		screen.blit(background, (0,0))	

		pygame.display.update(SCREENRECT)
		#>----<#
		for event in events : 
			if event.type == QUIT : game = False
			elif event.type == KEYDOWN and event.key == K_ESCAPE: game = False
			elif event.type == KEYDOWN and event.key == K_z : 
				pygame.image.save(screen, "scr"+str(scr_number)+".png")
				scr_number += 1
		#>----<#
		clock.tick(FPS)

	#>----<#
	
	pygame.quit()

####the####-----------------------------------------####the####
if __name__ == '__main__': main()
