import cv2
import numpy
import sys

img = cv2.imread(str(sys.argv[1]))
width = 256;
height = 256;
dim = (width, height)
# resize image
resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)

im = cv2.cvtColor(resized,cv2.COLOR_BGR2GRAY)

with open("image_small.h", "w") as text_file:
	print('const unsigned char image []= {',file=text_file)
	cut_size_x = 10
	cut_size_y = 10
	x_size = 0
	for x in im:
		y_size = 0
		x_size += 1
		if (x_size>cut_size_x) :
			break
		
		for y in x:	
			y_size += 1    			
			print(y, file=text_file, end=',')
			if (y_size>cut_size_y) :
				break
				        
		print('\n', file=text_file, end='')
	print('};',file=text_file)

