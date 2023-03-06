
from other import *

class MenuLine():
	def __init__(self, start_, end_, color_, grow_time):
		self.start, self.end = start_, end_
		self.color, self.gr_time = color_, float(grow_time)
		self.delta = math.hypot(start_.x-end_.x, start_.y-end_.y)/100.
	def grow(self, part, width):
		mult =  part / self.gr_time
		color = (self.color[0] * mult, self.color[1] * mult, self.color[2] * mult)
		pygame.draw.line(self.surface, color, self.start.GetTuple(), self.end.GetTuple(), 1)

class MenuBG():
	MAXPOINTNUMBER = 8192
	MAXPOINTPERCYCLE = 15
	GROWTIME = 25
	DRAWLINE1 = 64
	DRAWLINE2 = 1024
	DRAWLINE3 = 4096

	class Point:
		vector = Vector2D()
		angle = 0
		size = 95

	def __init__(self, surf, angle_=0):
		self.live_lines = []
		self.dead_lines = []
		start_point = self.Point()
		start_point.vector =  Vector2D(surf.get_rect().centerx, surf.get_rect().centery + 5)
		start_point.angle = angle_
		self.angle = math.pi * random.randrange(20, 30, 1) / 100.
		self.cos = math.cos(self.angle)
		self.sin = math.sin(self.angle)
		self.grow = self.GROWTIME
		self.work = True
		self.points = []
		self.point_number = 1
		self.nextPoint(start_point)
		self.line_srez_1 = float(self.DRAWLINE1)
		self.line_srez_2 = float(self.DRAWLINE2)
		self.line_srez_3 = float(self.DRAWLINE3)
		self.line_srez_4 = float(self.MAXPOINTNUMBER)
		self.color = (0, 0, 0)

	def nextPoint(self, start_point):
		a_point, b_point = self.Point(), self.Point()
		a = start_point.size * self.cos
		b = start_point.size * self.sin
		point_x = -start_point.size * 0.5 + a * self.cos
		point_y = start_point.size * 0.5 + a * self.sin
		vec = Vector2D(point_x, point_y)
		a_point.vector =  Vector2D(-a*0.5, a*0.5)
		b_point.vector =  Vector2D(b*0.5, b*0.5)
		a_point.vector.Rotate(self.angle)
		b_point.vector.Rotate(self.angle - math.pi * 0.5)
		a_point.vector += vec
		b_point.vector += vec
		a_point.vector.Rotate(start_point.angle)
		b_point.vector.Rotate(start_point.angle)
		a_point.vector += start_point.vector	
		b_point.vector += start_point.vector
		if self.work:
			a_point.size, b_point.size = a, b
			a_point.angle, b_point.angle = self.angle + start_point.angle, self.angle - math.pi * 0.5 + start_point.angle
			self.points.append(a_point)
			self.points.append(b_point)
			self.point_number += 2
			if self.point_number > self.MAXPOINTNUMBER : self.work = False
		return a_point.vector, b_point.vector

        def update_simple(self):
                if self.grow : 
			self.grow -= 1
			width = int( 5 - 4 * self.point_number / float(self.MAXPOINTNUMBER))
			for line in self.live_lines: line.grow(self.GROWTIME - self.grow, width)
		else:
			self.dead_lines += self.live_lines
			self.live_lines[:] = []
			self.grow = self.GROWTIME
			self.color = (0, 0, 0)
			for point in self.points[:self.MAXPOINTPERCYCLE]:
				start_point = point.vector
				end_point_a, end_point_b = self.nextPoint(point)
				self.live_lines.append(MenuLine(start_point, end_point_a, self.color, self.GROWTIME))
				self.live_lines.append(MenuLine(start_point, end_point_b, self.color, self.GROWTIME))
				self.points.remove(point) 

	def update(self):
		if self.grow : 
			self.grow -= 1
			width = int( 5 - 4 * self.point_number / float(self.MAXPOINTNUMBER))
			for line in self.live_lines: line.grow(self.GROWTIME - self.grow, width)
		else:
			self.dead_lines += self.live_lines
			self.live_lines[:] = []
			self.grow = self.GROWTIME
			if self.point_number > self.DRAWLINE3:
				mult = self.point_number/self.line_srez_4
				red = 180 + (39 - 180) * mult
				green = 38 + (64 - 38) * mult
				blue = 38 + (139 - 38) * mult
				self.color = (red, green, blue)
			elif self.point_number > self.DRAWLINE2:
				mult = self.point_number/self.line_srez_3
				red = 93 + (180 - 93) * mult
				green = 71 + (38 - 71) * mult
				blue = 140 + (38 - 140) * mult
				self.color = (red, green, blue)
			elif self.point_number > self.DRAWLINE1:
				mult = self.point_number/self.line_srez_2
				self.color = (93*mult, 71*mult, 140*mult)
			else : 
				mult = self.point_number/self.line_srez_1
				self.color = (40*mult, 40*mult, 60*mult)
			for point in self.points[:self.MAXPOINTPERCYCLE]:
				start_point = point.vector
				end_point_a, end_point_b = self.nextPoint(point)
				self.live_lines.append(MenuLine(start_point, end_point_a, self.color, self.GROWTIME))
				self.live_lines.append(MenuLine(start_point, end_point_b, self.color, self.GROWTIME))
				self.points.remove(point) 

def get_setup_0( screen ):
  screen.fill( (255 , 255 ,255) )
  MenuLine.surface = screen
  answer = MenuBG( screen, math.pi )
  answer.tick = answer.update_simple
  answer.angle = math.pi * 0.25
  answer.cos = math.cos(answer.angle)
  answer.sin = math.sin(answer.angle)
  start_point = MenuBG.Point()
  start_point.angle  = math.pi
  start_point.vector = Vector2D(screen.get_rect().centerx, screen.get_rect().height)
  answer.points = []
  answer.point_number = 1
  answer.nextPoint( start_point )
  return answer

def get_setup_1( screen ):
  MenuLine.surface = screen
  answer = MenuBG( screen )
  answer.tick = answer.update
  answer.angle = math.pi * 0.25 + 0.05
  answer.cos = math.cos(answer.angle)
  answer.sin = math.sin(answer.angle)
  start_point = MenuBG.Point()
  start_point.angle  = math.pi
  start_point.vector = Vector2D(screen.get_rect().centerx, screen.get_rect().height)
  answer.points = []
  answer.point_number = 1
  answer.nextPoint( start_point )
  return answer

