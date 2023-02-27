
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

SCREEN_WIDTH, SCREEN_HEIGHT = 600, 600
SW2, SH2 = SCREEN_WIDTH/2, SCREEN_HEIGHT/2
SCREENRECT = Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)
FPS=50

import ROOT
from ROOT import *

####fun####-----------------------------------------####fun####

# -3, 2.5
# -3.14 + 3.14 / 2., 3.14/2.

pL = -3
pR = 2.5
pB = -3.14 - 3.14 /2.
pT = 3.14/2.

center = Vector2D(SCREEN_WIDTH, SCREEN_HEIGHT)
center.x *= 1. / (pR - pL) * abs(pL)
center.y *= 1. / (pT - pB) * abs(pT)

center2 = Vector2D(abs(pL), abs(pB))
	
class Core():
	def __init__(self):
		self.jet_state = 0

	def Tick(self, screen):
		self.jet = TVector3(0, 0., 1);
		self.jet.RotateY( (15 + 5 * self.jet_state) * pi / 180. )

		self.jet_state += 1
		
		self.tracks = []

		self.step = 0.01
		self.step2 = 0.075
		self.N = int(1.0 * pi / self.step2)

		for i in xrange(self.N):
			v = TVector3(self.jet)
			v.RotateY(0.01 + self.step2 * i )
			self.tracks += [ v ]

		for i in xrange( int(2 * pi / self.step) ): self.Draw(screen)

		pygame.draw.line(screen, [0,0,0], (0,0), (0,SCREEN_HEIGHT), 5)
		pygame.draw.line(screen, [0,0,0], (0,SCREEN_HEIGHT), (SCREEN_WIDTH,SCREEN_HEIGHT), 5)

		font = pygame.font.Font(None, 30)

		nny = 40
		for i in xrange(nny):
			py = int(float(SCREEN_HEIGHT) / nny * (nny - i))
			if not i % 5 and i:
				pygame.draw.line(screen, [0,0,0], (0,py), (15,py), 3)

				text = font.render("{:1.2f}".format(pB + (pT-pB)/nny*i), 1, (0, 0, 0), (255, 255, 255))
				textpos = [20, py - 10]
				screen.blit(text, textpos)
			else :
				pygame.draw.line(screen, [0,0,0], (0,py), (10,py), 3)

		nnx = 55
		for i in xrange(nnx):
			px = int(float(SCREEN_WIDTH) / nnx * i)
			if not i % 5 and i:
				pygame.draw.line(screen, [0,0,0], (px,SCREEN_HEIGHT), (px,SCREEN_HEIGHT-15), 3)

				text = font.render("{:1.2f}".format(pL + (pR-pL)/nnx*i), 1, (0, 0, 0), (255, 255, 255))
				textpos = [px-text.get_width()/2, SCREEN_HEIGHT-20-17]
				screen.blit(text, textpos)
			else:
				pygame.draw.line(screen, [0,0,0], (px,SCREEN_HEIGHT), (px,SCREEN_HEIGHT-10), 3)

		text = font.render("{:1.2f}".format(self.jet.PseudoRapidity()), 1, (0, 0, 0), (255, 100, 100))
		textpos = [SCREEN_WIDTH-50-text.get_width()/2, 20]
		screen.blit(text, textpos)

	def Draw(self, screen):

		pygame.draw.rect(screen, [0,0,0], Rect(center.Tuple(), [2,2]))

		for i, track in enumerate(self.tracks):
			switch = False
			phase = float(i) / float(self.N); 
			color = [255 * phase, 50,  50]

			deta = track.PseudoRapidity() - self.jet.PseudoRapidity()
			if self.jet.PseudoRapidity() < 0 : deta *= -1
			dphi = track.Phi() - self.jet.Phi()
			if dphi < -pT : 
				dphi += 2 * pi
				switch = True

			pos_old = center + Vector2D(deta*SCREEN_WIDTH/(pR - pL), dphi*SCREEN_HEIGHT/(pT - pB))
			
			track.Rotate(self.step, self.jet)

			deta = track.PseudoRapidity() - self.jet.PseudoRapidity()
			if self.jet.PseudoRapidity() < 0 : deta *= -1
			dphi = track.Phi() - self.jet.Phi()
			if dphi < -pT : dphi += 2 * pi
			elif switch : continue

			pos_new = center + Vector2D(deta*SCREEN_WIDTH/(pR - pL), dphi*SCREEN_HEIGHT/(pT - pB))

			pygame.draw.line(screen, color, pos_old.Tuple(), pos_new.Tuple(), 2)

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
	gc = Core()
	game = True
	background.fill((255,255,255))

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

		pygame.image.save(screen, "lines"+str(scr_number)+".png")
		scr_number += 1
		#>----<#
		for event in events : 
			if event.type == QUIT : game = False
			elif event.type == KEYDOWN and event.key == K_ESCAPE: game = False
			elif event.type == KEYDOWN and event.key == K_z : 
				pygame.image.save(screen, "lines"+str(scr_number)+".png")
				scr_number += 1
		#>----<#
		clock.tick(FPS)
		T = sys.stdin.readline()

	#>----<#
	
	pygame.quit()

####the####-----------------------------------------####the####
if __name__ == '__main__': main()
