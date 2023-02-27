
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

SCREEN_WIDTH, SCREEN_HEIGHT = 600, 480
SW2, SH2 = SCREEN_WIDTH/2, SCREEN_HEIGHT/2
SCREENRECT = Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)
FPS=50

class Curve():
	def __init__(self, p1, p2):
		self.point = [p1.Tuple(), p2.Tuple()]
		self.p1, self.p2 = p1, p2
		self.step = 1

	def Gen(self, diff):
		self.diff = diff
		self.a = (diff * (self.p1.x - self.p2.x) - (self.p1.y - self.p2.y))/pow(self.p1.x - self.p2.x, 2)
		self.b = diff - 2*self.a*self.p1.x
		self.c = self.p1.y - self.b*self.p1.x - self.a*pow(self.p1.x, 2)

		self.point = [self.p1.Tuple()]
		if self.p1.x+1 < self.p2.x : 
			for x in xrange(self.p1.x+1, self.p2.x, self.step) :
				y = self.a*pow(x, 2)+self.b*x+self.c
				self.point.append([x, y])
		else : 
			for x in reversed(xrange(self.p2.x, self.p1.x+1, self.step)) :
				y = self.a*pow(x, 2)+self.b*x+self.c
				self.point.append([x, y])
		self.point.append(self.p2.Tuple())

	def GetDiff(self, point):
		return self.a*2*point.x+self.b

	def Draw(self, screen, color = (255, 15, 155), width = 1):
		pygame.draw.lines(screen, color, 0,  self.point, width)
		##pygame.draw.circle(screen, color, self.p1.Tuple(), width/2)
		##pygame.draw.circle(screen, color, self.p2.Tuple(), width/2)

class CurveContainer():
	def __init__(self):
		self.curvs = []
	
	def GenPoints1(self):
		self.curvs = []
		start = Vector2D(10, 200)
		last = start + Vector2D(25, randint(0, 25))
		diff = 0.
		for i in xrange(15):
			curve = Curve(start, last)
			curve.Gen(diff)
			diff = curve.GetDiff(last)
			start = copy(last)
			last += Vector2D(50, randint(0, 20))
			self.curvs.append( curve )
			curve = Curve(start, last)

	def GenPoints2(self):
		self.curvs = []
		start = Vector2D(10, 200+randrange(-100, 100))
		last = start + Vector2D(50, randint(-20, 20))
		diff = 0.
		for i in xrange(10):
			curve = Curve(start, last)
			curve.Gen(diff)
			diff = curve.GetDiff(last)
			start = copy(last)
			last += Vector2D(50, randint(-20, 20))
			self.curvs.append( curve )
			curve = Curve(start, last)

	def GenDiff(self, diff):
		for curve in self.curvs : 
			curve.Gen(diff)
			diff = curve.GetDiff(curve.p2)

	def Draw(self, screen, ii):
		for i, curve in enumerate(self.curvs) : 
			phase = abs(int(10*cos(pi * (ii + 5*i)/100.)))+1
			curve.Draw(screen, (10+20*phase, 250-20*phase,10+20*phase), 2*phase)
			#(randrange(0, 255), 15, 155)

class Point():
	def __init__(self):
		self.friends = []
		

class PointContainer():
	def __init__(self):
		self.points = []
		self.angle = 0.

class Bullet():
	def __init__(self, image, pos = Vector2D(SW2, SH2)):
		self.pos = pos
		self.image = image
		self.time = 0

		self.particles.append(self)

	def Tick(self, screen):
		self.pos += self.speed
		screen.blit(pygame.transform.rotate(self.image, randint(0, 180)), self.pos.Tuple())

		self.time += 1
		if self.time == self.life_time : 
			self.particles.remove(self)

class Weapont():
	SIZEX = 30
	SIZEY = 15
	
	BSIZEX = 3
	BSIZEY = 3

	def __init__(self):
		self.image = pygame.Surface((self.SIZEX, self.SIZEY))
		self.image.fill((0,0,0))
		self.image.set_colorkey((0,0,0))

		#self.image.fill((255,0,0))

		self.color = [randint(175, 255) for i in xrange(3)]

		def digcolor(c):
			c = max(c, 0)
			c = min(c, 255)
			return c

		def MutColor(color, f):
			R = digcolor(color[0]+randint(-f, f))
			G = digcolor(color[1]+randint(-f, f))
			B = digcolor(color[2]+randint(-f, f))
			return (R, G, B)

		def Max(color):
			R, G, B = color[0], color[1], color[2]
			if R > G : G *= 0.9
			else : R *= 0.9
		
			if G > B : B *= 0.9
			else : G *= 0.9

			if R > B : B *= 0.9
			else : R *= 0.9

			return (R, G, B)
	
		points = [(randint(0, self.SIZEX), randint(0, self.SIZEY)) for i in xrange(3)]
		for i in xrange(8):
			col1 = MutColor(self.color, 25)
			points = points[1:]+[(randint(0, self.SIZEX), randint(0, self.SIZEY))]
			pygame.draw.polygon(self.image, col1, points)
			col2 = Max(col1)
			pygame.draw.polygon(self.image, col2, points, 1)

		self.BSIZEX = randint(4,6)
		self.BSIZEY = randint(4,6)
		self.bullet_image = pygame.Surface((self.BSIZEX, self.BSIZEY))
		self.bullet_image.fill((0,0,0))
		self.bullet_image.set_colorkey((0,0,0))

		for i in xrange(5):
			bullet_color = Max(self.color)
			bullet_points = [(randint(0, self.BSIZEX), randint(0, self.BSIZEY)) for i in xrange(3)]
			pygame.draw.polygon(self.bullet_image, bullet_color, bullet_points)

		self.bullet_pos = Vector2D(SW2, SH2) + Vector2D(self.SIZEX/2, self.SIZEY/2)

		self.reload = 0
		self.fire = 0
		self.turn = 0

		#$%^&*-----*&^%$#

		self.type = choice(["bursts", "single"])

		#$%^&*-----*&^%$#

		self.range = randint(25, 100)
		self.reload_time = randint(5, 100)
		self.bullet_per_shoot = randint(1, 4)
		self.rate = randint(5, 25)
		self.bullet_per_turn = randint(1, max(1, self.rate/self.bullet_per_shoot))
		self.interval = self.rate / self.bullet_per_turn - 1
		self.speed = Vector2D(randint(5, 10+randint(0, 5 + randint(0, 5))), 0)

		if self.type == "single" : 
			self.bullet_per_shoot = 1
			self.bullet_per_turn = 1
			self.reload_time = max(5, self.reload_time/2)
			self.rate = 1
			self.interval = 0
			print "SINGLE"

		self.accuracy = randint(0, 4) + 2 * (self.bullet_per_shoot - 1) + randint(0, self.bullet_per_turn)
		danage_coff = float(self.bullet_per_shoot * self.bullet_per_turn) /  float(self.reload_time)
		danage_coff = 1./danage_coff + randint(-5, int(max(5, 25 - 1./danage_coff)))
		danage_coff = max(1, danage_coff)
		print danage_coff

		#$%^&*-----*&^%$#

		print "self.reload_time = ", self.reload_time
		print "self.range = ", self.range
		print "self.bullet_per_shoot = ", self.bullet_per_shoot
		print "self.rate = ", self.rate
		print "self.bullet_per_turn = ", self.bullet_per_turn

	def Fire(self):
		if self.fire < self.rate :
			self.fire += 1
			self.turn += 1
			if self.turn > self.interval :
				self.turn = 0 
				for i in xrange(self.bullet_per_shoot):
					b = Bullet(self.bullet_image, self.bullet_pos)
					b.speed = self.speed.Rotate( pi / 180. * randint(-self.accuracy, self.accuracy) )
					b.life_time = self.range
		else : 
			self.reload += 1
			if self.reload > self.reload_time : 
				self.reload = 0
				self.fire = 0
				self.turn = 0

def digcolor(c):
	if c > 255 : c = 255
	elif c < 0 : c = 0
	return c

class Core():
	def __init__(self):
		# Tick 1
		self.curve = Curve(Vector2D(100,100), Vector2D(150,100))
		self.curve.Gen(0.5)
		self.counter = 0

		# Tick 2
		self.back = pygame.Surface(SCREENRECT.size)
		#self.back.fill((0,0,0))
		#self.back.set_colorkey((0,0,0))
		self.back.set_alpha(250)
		self.container = CurveContainer()
		self.container.GenPoints2()

		# Tick 3
		self.particles = []
		Bullet.particles = self.particles

		self.containers = []
		for i in xrange(1) : 
			cont=CurveContainer()
			cont.GenPoints2()
			self.containers.append( cont  )

		# Tick 4
		self.weapont = Weapont()
		# print self.weapont

		# Tick 5
		self.flag = True
		#self.point_pos = [[randint(0, SCREEN_WIDTH), choice([0, SCREEN_HEIGHT])] for i in xrange(1000)]
		#self.point_pos = [[choice([0, SCREEN_WIDTH]), randint(0, SCREEN_HEIGHT)] for i in xrange(5000)]
		self.point_pos = [[SW2, SH2] for i in xrange(5000)]
		#self.point_pos = [[SW2, SH2] for i in xrange(1000)]
		shift = 50
		#self.point_pos = [[randint(-shift, SCREEN_WIDTH+shift), randint(-shift, SCREEN_HEIGHT+shift)] for i in xrange(100)]
		self.lines = [[randint(0, len(self.point_pos)), randint(0, len(self.point_pos))] for i in xrange(1000)]
		

		#self.Tick = self.Tick3
		self.Tick = self.Tick4
		self.color = [0,0,0]

	def Tick5(self, screen):
		if self.flag : 
			self.flag = False
			screen.fill((255, 255, 255))
			#screen.fill((0, 0, 0))
		#screen.fill((255, 255, 255))
		for line in self.lines : 
			continue
			p1 = self.point_pos[line[0]]
			p2 = self.point_pos[line[1]]
			#color = choice([(0,0,0), (255,255,255), (128, 0, 0)])
			#color = choice([(0,0,0), (255,255,255), (171, 130, 255)])
			color = [digcolor(255 - abs(p1[0] - p2[0]) / 10.), digcolor(255 - abs(p1[0] - p2[0]) / 10.), digcolor(255 - abs(p1[0] - p2[0]) / 10.)]
			pygame.draw.line(screen, color, p1, p2)
		for i in xrange(100):
			continue
			p1 = choice(self.point_pos)
			p2 = choice(self.point_pos)
			color = choice([(0,0,0), (255,255,255), (128, 0, 0)])
			color = choice([(0,0,0), (255,255,255), (171, 130, 255)])
			pygame.draw.line(screen, color, p1, p2)
		self.color[0] = digminmax(self.color[0] + randint(-3, 10), 0, 255)
		self.color[1] = digminmax(self.color[1] + randint(-3, 10), 0, 255)
		self.color[2] = digminmax(self.color[2] + randint(-3, 10), 0, 255)
		if sum(self.color) > 100*3 :
			self.color = [int(self.color[0]*0.1),int(self.color[1]*0.1),int(self.color[2]*0.1)]
		self.point_pos = self.point_pos[5:] + [[choice([0, SCREEN_WIDTH]), randint(0, SCREEN_HEIGHT)] for i in xrange(5)]
		for p in self.point_pos:
			#if p[0] < SW2 : 
			#	p[0] = digminmax(p[0] + randint(0, 1), 0, SCREEN_WIDTH)
			#else : 
			#	p[0] = digminmax(p[0] + randint(-1, 0), 0, SCREEN_WIDTH)
			p[0] = digminmax(p[0] + randint(-1, 1), 0, SCREEN_WIDTH)
			p[1] = digminmax(p[1] + randint(-1, 1), 0, SCREEN_HEIGHT)
			#color = choice([(0,0,0), (255,255,255), (255,0,255), (0,255,255)])
			#color = choice([(0,0,0), (255,255,255), (168, 0, 0)])
			#color = choice([(0,0,0), (255,255,255), (randint(0,255),randint(0,255),randint(0,255))])
			pygame.draw.rect(screen, self.color, Rect(p, (2,2)))

	def Tick4(self, screen):
		screen.fill((0,0,0))
		screen.blit(self.weapont.image, (SCREEN_WIDTH/2, SCREEN_HEIGHT/2) )

		self.weapont.Fire()
		for particle in self.particles :
			particle.Tick(screen)

	def Tick3(self, screen):
		#screen.fill((0,0,0))
		self.counter += 1
		if self.counter > 1000 : self.counter = 0
		#screen.fill((randrange(55, 60),0,randrange(55, 60)))
		for i, cont in enumerate(self.containers) :
			diff = cos(2*pi*float(self.counter+10*i)/100.)
			cont.GenDiff(diff)
			cont.Draw(screen, self.counter+10*i)
		
		#screen.blit(self.back, (0,0))

	def Tick2(self, screen):
		#screen.fill((0,0,0))
		self.counter += 10
		if self.counter > 1000 : self.counter = 0
		screen.fill((0,0,0))
		#diff = cos(2*pi*float(self.counter)/1000.)
		for i in xrange(25):
			self.container.GenPoints2()
			self.container.GenDiff(0.)
			self.container.Draw(screen)
		
		#screen.blit(self.back, (0,0))

	def Tick1(self, screen):
		self.counter += 10
		if self.counter > 1000 : self.counter = 0
		diff = 5*cos(2*pi*float(self.counter)/1000.)

		screen.fill((0,0,0))
		self.curve.p2 = Vector2D(100,100) + Vector2D(50,0)#.RotateInt(2*pi*float(self.counter)/1000.)
		print self.curve.p2
		try : 
			self.curve.Gen(-5.)
			self.curve.Draw(screen)
		except : print "error = ", self.curve.p2

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
