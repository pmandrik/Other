
from math import cos, sin, hypot

#/*%---%*/#

def digsign(x):
	if x < 0 : return -1
	elif x > 0 : return 1
	return 0

def digdir(x):
	if x < 0 : return 0
	return 1
	

def digminmax(x, minn, maxx):
	if x > maxx : return maxx
	if x < minn : return minn
	return x

#/*%---%*/#

class Vector2D():
	def __init__(self, x=0, y=0, z=0):
		self.x, self.y, self.z = x, y, z

	def Var(self):
		return self.x, self.y

	def Tuple(self):
		return (self.x, self.y)

	def List(self):
		return [self.x, self.y]

	def Add(self, x=0, y=0):
		self.x+=x
		self.y+=y

	def Add2(self, t):
		self.x += t[0]
		self.y += t[1]

	def L(self):
		return hypot(self.x, self.y)

	def Max(self):
		return max(self.x, self.y)

	def __add__ (self, other):
		return Vector2D(self.x + other.x, self.y + other.y)

	def __sub__ (self, other):
		return Vector2D(self.x - other.x, self.y - other.y)

	def __min__(self, other):
		return Vector2D(self.x - other.x, self.y - other.y)

	def __complex__ (self):
		pass

	def __str__(self):
		return "Vector2D(%d, %d)"%(self.x, self.y)

	def Rotate(self, angle):
		new_x = self.x * cos(angle) + self.y * sin(angle)
		new_y = -self.x * sin(angle) + self.y * cos(angle)
		#self.x, self.y = new_x, new_y
		return Vector2D(new_x, new_y)

	def RotateInt(self, angle):
		new_x = self.x * cos(angle) + self.y * sin(angle)
		new_y = -self.x * sin(angle) + self.y * cos(angle)
		#self.x, self.y = new_x, new_y
		return Vector2D(int(new_x), int(new_y))

	#def Rotate(self, angle):
	#	new_x = self.x * math.cos(angle) + self.y * math.sin(angle)
	#	new_y = -self.x * math.sin(angle) + self.y * math.cos(angle)
	#	self.x, self.y = new_x, new_y
	#	return self

