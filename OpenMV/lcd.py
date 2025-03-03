import sensor
import display
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA2)
lcd = display.SPIDisplay()
while True:
	lcd.write(sensor.snapshot())