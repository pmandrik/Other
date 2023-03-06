
import pygame
from pygame.locals import *

import math
import random
from random import randint
from math import acos, pi

class L_System():
        def __init__(self, state):
                self.rules = {}
                self.state = state

        def add_rule(self, symbol, replacement):
                self.rules[symbol] = replacement

        def tick(self):
                new_state = ""
                for symbol in self.state:
                        if not self.rules.has_key( symbol ):
                                new_state += symbol
                                continue
                        new_state += self.rules[ symbol ]
                self.state = new_state

###VECTOR2D###
class Vector2D():
	def __init__(self, x_=0, y_=0):
		self.x, self.y = x_, y_

	def __add__ (self, other):
		return Vector2D(self.x + other.x, self.y + other.y)

	def Cos(self):
		return self.x/self.L()

	def __mul__(self, val):
		return Vector2D(self.x*val, self.y*val)

	def __str__(self):
		return "Vector2D" + str((self.x, self.y))


	def __neg__(self) :
		return Vector2D(-self.x, -self.y)

	def __len__(self):
		return 2

	def __sub__ (self, other):
		return Vector2D(self.x - other.x, self.y - other.y)


	def Rotate(self, angle):
		new_x = self.x * math.cos(angle) + self.y * math.sin(angle)
		new_y = -self.x * math.sin(angle) + self.y * math.cos(angle)
		self.x, self.y = new_x, new_y
		return self

	def RotateD(self, angle):
                angle = angle / 180. * math.pi
		new_x = self.x * math.cos(angle) + self.y * math.sin(angle)
		new_y = -self.x * math.sin(angle) + self.y * math.cos(angle)
		self.x, self.y = new_x, new_y
		return self

	def Angle(self):
		angle = acos(self.Cos())
		if self.y < 0 : angle = 2*pi - angle 
		return angle

	def GetTuple(self):
		return (self.x, self.y)

	def Lenght(self):
		return math.hypot(self.x, self.y)

	def __getitem__(self, key):
		if key == 0 : return self.x
		if key == 1 : return self.y
		if key == 2 : return self.z


        def L(self):
		return math.hypot(self.x, self.y)

	def Max(self):
		if abs(self.x) > abs(self.y) : return abs(self.x)
		return abs(self.y)

	def Accept(self, x_, y_):
		self.x, self.y = x_, y_

###LINE2D###
class Line2D():
	def __init__(self, point1, point2):
		self.p1, self.p2 = point1, point2
