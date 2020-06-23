import cv2
import numpy
import math
from PIL import Image
#from matplotlib import pyplot


with open("final_image.txt", "r") as text_file:

	iksevi = text_file.readlines() #iksevi = redovi
	kolone = iksevi[1].split(' ')
	print('iksevi : ',len(iksevi))
	print('kolone : ',len(kolone))
	imgArray = numpy.empty([len(iksevi),len(kolone)], dtype=numpy.int16)
	xerino = 0

	for x in iksevi:
		brojevi = x.split(' ')
		y = 0
		for broj in brojevi:
			if (broj != '\n' and broj != ''):						
				imgArray[xerino][y] = int(broj)
				y+=1
		xerino+=1

	img = Image.fromarray(imgArray)
	img.show()






