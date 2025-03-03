import sensor, image, lcd
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA2)
lcd.init()
while(True):
	lcd.display(sensor.snapshot())