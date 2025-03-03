# Template Matching Example - Normalized Cross Correlation (NCC)
#
# This example shows off how to use the NCC feature of your OpenMV Cam to match
# image patches to parts of an image... expect for extremely controlled enviorments
# NCC is not all to useful.
#
# WARNING: NCC supports needs to be reworked! As of right now this feature needs
# a lot of work to be made into somethin useful. This script will reamin to show
# that the functionality exists, but, in its current state is inadequate.

import time, sensor, image,os,tf
from image import SEARCH_EX, SEARCH_DS
from pyb import UART
#从imgae模块引入SEARCH_EX和SEARCH_DS。使用from import仅仅引入SEARCH_EX,
#SEARCH_DS两个需要的部分，而不把image模块全部引入。
uart = UART(3, 9600)
# Reset sensor
sensor.reset()

# Set sensor settings
# 与 SEARCH_EX 匹配的最大分辨率是 QQVGA
sensor.set_framesize(sensor.QQVGA)
# 可以设置窗口减少搜索图像
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_lens_correction(True, 5,5)
#sensor.skip_frames()

net = "trained.tflite"
labels = [line.rstrip('\n') for line in open("labels.txt")]

# 加载模板
# 模板应该是小的 (如 32x32 像素) 灰度
template1 = ["/1.pgm"]
template2 = ["/2.pgm"]
template3 = ["/3.pgm","/3a.pgm","/3b.pgm"]
template4 = ["/4.pgm","/4a.pgm","/4b.pgm"]
template5 = ["/5.pgm","/5a.pgm","/5b.pgm"]
template6 = ["/6.pgm","/6a.pgm","/6b.pgm"]
template7 = ["/7.pgm","/7a.pgm","/7b.pgm"]
template8 = ["/8.pgm","/8a.pgm","/8b.pgm"]

A0=1
A9=1
B0=1
A1=0
A2=0
A3=0
A4=0
A5=0
A6=0
A7=0
A8=0
B0=1
C0=1
C1=0
C2=1
D0=1
D1=1


#加载模板图片

clock = time.clock()

# Run template matching
while (True):
        clock.tick()
        img = sensor.snapshot()
        while(A0):
            clock.tick()
            img = sensor.snapshot()
            t1 = image.Image(template1[0])
            r1 = img.find_template(t1, 0.80, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r1:
                img.draw_rectangle(r1)
                A1=1
                A0=0
            t2 = image.Image(template2[0])
            r2 = img.find_template(t2, 0.80, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r2:
                img.draw_rectangle(r2)
                A2=1
                A0=0
            t3 = image.Image(template3[0])
            r3 = img.find_template(t3, 0.85, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r3:
                img.draw_rectangle(r3)
                print('3') #打印模板名字
                A3=1
                A0=0
            t4 = image.Image(template4[0])
            r4 = img.find_template(t4, 0.80, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r4:
                img.draw_rectangle(r4)
                print('4') #打印模板名字
                A4=1
                A0=0
            t5 = image.Image(template5[0])
            r5 = img.find_template(t5, 0.80, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r5:
                img.draw_rectangle(r5)
                print('5') #打印模板名字
                A5=1
                A0=0
            t6 = image.Image(template6[0])
            r6 = img.find_template(t6, 0.80, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r6:
                img.draw_rectangle(r6)
                print('6') #打印模板名字
                A6=1
                A0=0
            t7 = image.Image(template7[0])
            r7 = img.find_template(t7, 0.80, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r7:
                img.draw_rectangle(r7)
                print('7') #打印模板名字
                A7=1
                A0=0
            t8 = image.Image(template8[0])
            r8 = img.find_template(t8, 0.85, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r8:
                img.draw_rectangle(r8)
                print('8') #打印模板名字
                A8=1
                A0=0

        while(A9):
            if A1==1:
                uart.write('1')
                print('1') #打印模板名字
                num=1
                A9=0
            if A2==1:
                uart.write('2')
                print('2') #打印模板名字
                num=2
                A9=0
            if A3==1:
                uart.write('3')
                print('3')
                num=3
                A9=0
            if A4==1:
                uart.write('4')
                print('4')
                num=4
                A9=0
            if A5==1:
                uart.write('5')
                print('5')
                num=5
                A9=0
            if A6==1:
                uart.write('6')
                print('6')
                num=6
                A9=0
            if A7==1:
                uart.write('7')
                print('7')
                num=7
                A9=0
            if A8==1:
                uart.write('8')
                print('8')
                num=8
                A9=0

        while(True):
            while(B0):
                    if (uart.any()):
                        B = uart.read()
                        print(B)
                        if B==b'1':
                            print("1")
                            B0=0
                            C0=1
            while(C0):
                img = sensor.snapshot()
                roiL=(10,10,60,100)
                for obj in tf.classify(net, img,roi=roiL,min_scale=1, scale_mul=0.8, x_overlap=0.5, y_overlap=0.5):
                    print("**********\nPredictions at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
                    img.draw_rectangle(obj.rect())
                    predictions_list = list(zip(labels, obj.output()))
                for i in range(len(predictions_list)):
                    print("%s = %f" % (predictions_list[i][0], predictions_list[i][1]))
                    num1=ord(predictions_list[i][0])-48
                    if predictions_list[i][1]>0.7 and num1==num:
                        uart.write('1')
                        print('11')
                        B0=1
                        C1=1
                        C0=0
                roiR=(100,10,60,100)
                for obj in tf.classify(net,img ,roi=roiR,min_scale=1, scale_mul=0.8, x_overlap=0.5, y_overlap=0.5):
                    print("**********\nPredictions2 at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
                    img.draw_rectangle(obj.rect())
                    predictions_list2 = list(zip(labels, obj.output()))
                for p in range(len(predictions_list2)):
                    print("%s = %f" % (predictions_list2[p][0], predictions_list2[p][1]))
                    num2=ord(predictions_list2[p][0])-48
                    if predictions_list2[p][1]>0.7 and num2 == num:
                        uart.write('2')
                        print("22")
                        B0=1
                        C1=1
                        C0=0
                if C1==0:
                    uart.write('0')
                    print("00")
                    B0=1
            print(clock.fps(), "fps")

