import pygame
import tkinter as tk
import tkinter.filedialog
import time, random, os

# constants
K_LEFT = pygame.K_LEFT
K_RIGHT = pygame.K_RIGHT
K_DOWN = pygame.K_DOWN
K_UP = pygame.K_UP
K_ESCAPE = pygame.K_ESCAPE
K_o = pygame.K_o
K_w = pygame.K_w
K_h = pygame.K_h

# create some colors
BLACK = (0,0,0)
GREY = (200,200,200)
white = (255,255,255)
# object data
TILE_SIZE = 16+1
WIDTH = 64
HEIGHT = 40
DISPLAY_W = TILE_SIZE*WIDTH+1
DISPLAY_H = TILE_SIZE*HEIGHT
TILE_ROWS = 4
MAX_WIDTH = 300
MAX_HEIGHT = 300
STATUS_Y = DISPLAY_H-TILE_SIZE+2

TITLE = 'Map Editor'

# set up pygame
pygame.init()
pygame.font.init()
# create our surface
screen = pygame.display.set_mode((DISPLAY_W,DISPLAY_H))
# set title
pygame.display.set_caption(TITLE)
clock = pygame.time.Clock()

# load sprites
tiles = []
directory = 'tiles/'
NUMTILES = 0
for filename in sorted(os.listdir(directory)):
	tiles.append(pygame.image.load(directory+filename).convert())
	NUMTILES += 1
tiles.append(pygame.image.load('cursor.bmp').convert())

# width and height boxes
BOX_W = 40
WIDTH_XY = (400,0)
WIDTH_RECT = (WIDTH_XY[0]+56,0,BOX_W,17)
HEIGHT_XY = (WIDTH_RECT[0]+BOX_W+10,0,BOX_W,17)
HEIGHT_RECT = (HEIGHT_XY[0]+66,0,BOX_W,17)

class StatusBar:
	def __init__(self,message='',counter=0):
		self.message = message
		self.counter = counter
	def set(self,message,counter = 60):
		self.message = message
		self.counter = counter

class Mouse:
	def __init__(self,sprite=tiles[-1],x=0,y=0):
		self.x = x
		self.y = y
		self.sprite = sprite
		self.spriteid = -1
		self.w_or_h = ''
	def onMap(self):					# Check if mouse is on map area
		height = level.height
		if height > HEIGHT-TILE_ROWS-1:
			height = HEIGHT-TILE_ROWS-1		# - 1 because the top row is the menu
		width = level.width
		if width > WIDTH:
			width = WIDTH
		return self.y > TILE_SIZE and self.y < (height+1)*TILE_SIZE and self.x < width*TILE_SIZE

class MenuButton:
	def __init__(self,text='button',x=0,y=0,width=10,height=10,action=''):
		self.text = text
		self.x = x
		self.y = y
		self.width = width
		self.height = height
		self.hover = False
		self.action = action
	def __repr__(self):
		return "MenuButton: ['{}',{},{},{},{},{}]".format(self.text,self.x,self.y,self.width,self.height,self.hover)

class Level:
	def __init__(self,width=32,height=32,x=0,y=0,filename=''):
		self.map = []
		self.width = width
		self.height = height
		self.x = x
		self.y = y
		self.saved = False
		self.filename = filename
		self.loadMap()
	def loadMap(self):
		for y in range(MAX_HEIGHT):
			self.map.append([])
			for x in range(MAX_WIDTH):
				self.map[y].append(0)
		print(self.filename)
		if self.filename != '':
			with open(self.filename,"r") as f:
				# first line is width and height
				line = f.readline().rstrip('\n')
				w,h = line.split(' ')
				self.width = int(w)
				self.height = int(h)
				y = 0
				for line in f:
					x = 0
					line = line[:-2]
					for tile in line.split(','):
						self.map[y][x] = int(tile)
						x += 1
					y += 1
			self.saved = True
	def drawMap(self):
		# make sure map doesn't get drawn past screen limits
		height = self.height
		if height > HEIGHT-TILE_ROWS-1:
			height = HEIGHT-TILE_ROWS-1		# - 1 because the top row is the menu
		width = self.width
		if width > WIDTH:
			width = WIDTH
		for y in range(height):
			for x in range(width):
				blitSprite(tiles[self.map[y+self.y][x+self.x]],x*TILE_SIZE+1,y*TILE_SIZE+TILE_SIZE+1)

def newFile():
	level.map = []
	level.width = 32
	level.height = 32
	level.x = 0
	level.y = 0
	level.filename = ''
	level.saved = False
	level.loadMap()
	statusbar.set("New map")

def openFile():
	root = tk.Tk()
	root.withdraw()
	level.filename = tk.filedialog.askopenfilename(title = "Load a map",filetypes = (("map files","*.map"),("all files","*.*")),defaultextension=".map")
	if not level.filename:
		level.filename = ''
	root.destroy()
	level.loadMap()
	statusbar.set("Map loaded")

def saveFile():
	if level.filename == '':
		root = tk.Tk()
		root.withdraw()
		level.filename = tk.filedialog.asksaveasfilename(title = "Save your map",filetypes = (("map files","*.map"),("all files","*.*")),defaultextension=".map")
		root.destroy()
	if level.filename:
		with open(level.filename,"wt") as f:
			f.write("{} {}\n".format(level.width,level.height))
			for y in range(level.height):
				for x in range(level.width):
					f.write(str(level.map[y][x])+',')
				f.write('\n')
		level.saved = True
		statusbar.set("File saved")
	else:
		level.filename = ''

def exportFile():
	if level.filename == '':
		saveFile()
	
	if level.filename != '':
		filename,ext = os.path.basename(level.filename).split('.')
		with open(filename+'.h',"wt") as header:
			header.write("//Width: {}\t Height: {}\nglobal_variable u16 {}[] = {{".format(level.width,level.height,filename))
			for y in range(level.height):
				header.write('\n\t')
				for x in range(level.width):
					header.write(str(level.map[y][x])+',')
			header.write('\n}')
		statusbar.set("Map exported")

def drawMenu(buttons, width, height):
	fontobject = pygame.font.Font(None,25)

	# menu buttons
	for b in buttons:
		border = 1
		if b.hover:
			border = 0
		pygame.draw.rect(screen, (154,125,254),(b.x,b.y,b.width,b.height), border)
		screen.blit(fontobject.render(b.text, 0, BLACK),(b.x+2,b.y))

	# width and height boxes
	screen.blit(fontobject.render("Width: ", 0, BLACK),WIDTH_XY)
	screen.blit(fontobject.render("Height: ", 0, BLACK),HEIGHT_XY)
	border = 1
	if mouse.w_or_h == 'w':
		border = 0
	pygame.draw.rect(screen, (154,125,254),WIDTH_RECT, border)
	border = 1
	if mouse.w_or_h == 'h':
		border = 0
	pygame.draw.rect(screen, (154,125,254),HEIGHT_RECT, border)

	screen.blit(fontobject.render(width, 0, BLACK),(WIDTH_RECT[0]+2,2))
	screen.blit(fontobject.render(height, 0, BLACK),(HEIGHT_RECT[0]+2,2))

def drawStatusBar():
	fontobject = pygame.font.Font(None,21)
	# check if there's a status message to display
	if statusbar.counter > 0:
		screen.blit(fontobject.render(statusbar.message, 0, BLACK),(10,STATUS_Y))
		statusbar.counter -= 1
	# draw map coords
	string = "Map - {},{}".format(str(level.x),str(level.y))
	screen.blit(fontobject.render(string, 0, BLACK),(DISPLAY_W//2-100,STATUS_Y))
	# draw the mouse coords if the mouse is on top of the map
	if mouse.onMap():
		string = "Mouse - {},{}".format(str(mouse.x//TILE_SIZE+level.x),str(mouse.y//TILE_SIZE-1+level.y))
		screen.blit(fontobject.render(string, 0, BLACK),(DISPLAY_W//2,STATUS_Y))

def blitSprite(sprite,x,y):
	screen.blit(sprite,(x,y))

def drawMouse():
	sprite = mouse.sprite
	if mouse.y > DISPLAY_H-TILE_ROWS*TILE_SIZE:
		sprite = tiles[-1]						# if cursor is over tiles, switch to main cursor sprite
	surface = pygame.Surface((16,16), depth=24)
	surface.set_alpha(255)
	surface.set_colorkey((255,255,255))
	surface.blit(sprite,(0,0))
	screen.blit(surface,(mouse.x,mouse.y))

def drawTileList():
	x = 1
	y = (HEIGHT-TILE_ROWS)*TILE_SIZE+1
	for tile in tiles[:-1]:
		blitSprite(tile,x,y)
		x += TILE_SIZE

def drawGrid():
	width = level.width
	height = level.height
	if level.width > WIDTH:
		width = WIDTH
	if level.height > HEIGHT-TILE_ROWS-1:
		height = HEIGHT-TILE_ROWS-1
	for x in range(width+1):
		pygame.draw.line(screen,BLACK,(x*TILE_SIZE,TILE_SIZE),(x*TILE_SIZE,(height+1)*TILE_SIZE))
	for y in range(1,height+2):
		pygame.draw.line(screen,BLACK,(0,y*TILE_SIZE),(width*TILE_SIZE,y*TILE_SIZE))
	pygame.draw.line(screen,BLACK,(0,DISPLAY_H-TILE_SIZE),(DISPLAY_W,DISPLAY_H-TILE_SIZE))


def checkMouse(buttons):
	for b in buttons:
		if mouse.y < TILE_SIZE and b.x < mouse.x < b.x+b.width:
			b.hover = True
		else:
			b.hover = False
	mouseClick = pygame.mouse.get_pressed()
	if mouseClick[0] == 1:
		# buttons up top
		if mouse.y < TILE_SIZE:
			if WIDTH_RECT[0]+BOX_W > mouse.x > WIDTH_RECT[0]:
				mouse.w_or_h = 'w'
			if HEIGHT_RECT[0]+BOX_W > mouse.x > HEIGHT_RECT[0]:
				mouse.w_or_h = 'h'
			
		# list of tiles at bottom
		if mouse.y > DISPLAY_H-TILE_ROWS*TILE_SIZE:
			y = (mouse.y - (DISPLAY_H-TILE_ROWS*TILE_SIZE))//TILE_SIZE 
			x = mouse.x//TILE_SIZE
			if len(tiles)-1 > y*WIDTH + x:
				mouse.spriteid = y*WIDTH + x
				mouse.sprite = tiles[mouse.spriteid]
		# tilemap
		if mouse.onMap() and mouse.spriteid != -1:
			y = (mouse.y-TILE_SIZE) // TILE_SIZE			# don't forget top row is the menu
			x = mouse.x // TILE_SIZE
			level.map[y+level.y][x+level.x] = mouse.spriteid
			level.saved = False

BUTTONS = [	('New',0,0,50,16,newFile),
			('Load',50,0,50,16,openFile),
			('Save',100,0,50,16,saveFile),
			('Export',150,0,60,16,exportFile)]

def main():
#	level.loadMap()
	buttons = []
	for text,x,y,width,height,action in BUTTONS:
		buttons.append(MenuButton(text,x,y,width,height,action))

	running = True
	width_str = str(level.width)
	height_str = str(level.height)
	while running:
		filename = os.path.basename(level.filename)
		if not level.saved:
			pygame.display.set_caption(TITLE+' '+filename+' [not saved]')
		else:
			pygame.display.set_caption(TITLE+' '+filename)
		key = 0
		keys = pygame.key.get_pressed()
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				running = False
			# check keyboard commands
			if event.type == pygame.KEYDOWN:
				if keys[pygame.K_LCTRL] or keys[pygame.K_RCTRL]:
					if event.key == pygame.K_s:
						saveFile()
					if event.key == K_o:
						openFile()
					if event.key == pygame.K_n:
						newFile()
					if event.key == pygame.K_e:
						exportFile()
				if event.key == K_ESCAPE:
					if mouse.w_or_h == '':
						running = False
					else:
						width_str = str(level.width)
						height_str = str(level.height)
						mouse.w_or_h = ''
				if event.key == K_w:
					if mouse.w_or_h == '':
						mouse.w_or_h = 'w'			# update width
					else:
						width_str = str(level.width)
						mouse.w_or_h = ''
				if event.key == K_h:
					if mouse.w_or_h == '':
						mouse.w_or_h = 'h'			# update height
					else:
						height_str = str(level.height)
						mouse.w_or_h = ''
				if event.key == pygame.K_BACKSPACE:
					key = 'backspace'
				if event.key == pygame.K_RETURN:
					key = 'enter'
				if 47 < event.key < 59:
					key = event.key
			# update mouse x/y
			if event.type == pygame.MOUSEMOTION:
				mousePos = pygame.mouse.get_pos()
				mouse.x = mousePos[0]
				mouse.y = (mousePos[1]//TILE_SIZE)*TILE_SIZE+1
				if mouse.y > TILE_SIZE:
					mouse.x = (mousePos[0]//TILE_SIZE)*TILE_SIZE+1
			# check for mouseclick on menu buttons
			if event.type == pygame.MOUSEBUTTONDOWN:
				if event.button < 4:
					for b in buttons:
						if mouse.y < TILE_SIZE and b.x < mouse.x < b.x+b.width:
							b.action()
				# check for scroll wheel
				if event.button == 4:				# middle mouse down
					if mouse.spriteid > -1:
						mouse.spriteid -= 1
						mouse.sprite = tiles[mouse.spriteid]
				if event.button == 5:				# middle mouse down
					if -1 <= mouse.spriteid < NUMTILES -1:
						mouse.spriteid += 1
						mouse.sprite = tiles[mouse.spriteid]
		if mouse.w_or_h != '' and key != 0:
			# fill out width/height box
			if mouse.w_or_h == 'w':
				if key == 'backspace':
					width_str = width_str[:-1]
				elif key == 'enter':
					if not width_str:
						width_str = '8'
					if int(width_str) > MAX_WIDTH:
						width_str = str(MAX_WIDTH)
					level.width = int('0'+width_str)
					mouse.w_or_h = ''
				elif len(width_str) < 3:
					width_str += chr(key)
			elif mouse.w_or_h == 'h':
				if key == 'backspace':
					height_str = height_str[:-1]
				elif key == 'enter':
					if not height_str:
						height_str = '8'
					if int(height_str) > MAX_HEIGHT:
						height_str = str(MAX_HEIGHT)
					level.height = int('0'+height_str)
					mouse.w_or_h = ''
				elif len(height_str) < 3:
					height_str += chr(key)
		elif mouse.w_or_h == '':
			width_str = str(level.width)
			height_str = str(level.height)
		if keys[K_LEFT]:
			if level.x > 0:
				level.x -= 1
		if keys[K_UP]:
			if level.y > 0:
				level.y -= 1
		if keys[K_RIGHT]:
			if level.x < level.width-WIDTH:
				level.x += 1
		if keys[K_DOWN]:
			if level.y < level.height-(HEIGHT-1-TILE_ROWS):
				level.y += 1

		# erase screen
		screen.fill(white)

		# draw grid
		drawGrid()

		# draw tilemap
		level.drawMap()

		# draw tile list
		drawTileList()

		# draw menu
		drawMenu(buttons, width_str, height_str)

		# handle mouse
		if mouse.onMap() or mouse.y > DISPLAY_H-TILE_ROWS*TILE_SIZE:
			drawMouse()
		checkMouse(buttons)	# check for mouse clicks

		drawStatusBar()

		# update screen
		pygame.display.update()

		# 60 Mhz
		clock.tick(60)
	print('closing')
	pygame.quit()
	print('closed')

mouse = Mouse()
level = Level()
statusbar = StatusBar()
main()
print('end of main')
