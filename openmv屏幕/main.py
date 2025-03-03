import sensor, image, time,screen
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
clock = time.clock()
screen.init()
img_drawing_board=sensor.alloc_extra_fb(320,240,sensor.RGB565)
img_drawing_board.draw_rectangle(0,0,320,240,fill=True,color=(255,255,255))
fps=0
last_x=0
last_y=0
first_time_press=True
while True:
	clock.tick()
	img = sensor.snapshot()
	if screen.press:
		if first_time_press:
			img_drawing_board.draw_line(screen.x,screen.y,screen.x,screen.y,color=(0,0,0),thickness=3)
			last_x=screen.x
			last_y=screen.y
			first_time_press=False
		else:
			img_drawing_board.draw_line(screen.x,screen.y,last_x,last_y,color=(0,0,0),thickness=3)
			last_x=screen.x
			last_y=screen.y
	else:
		first_time_press=True
	img.b_nor(img_drawing_board)
	img.draw_string(10,10,'FPS:'+str(fps),color=(255,0,0),mono_space=False)
	screen.display(img)
	fps=clock.fps()