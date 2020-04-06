import cv2
import numpy
import math
from PIL import Image
#from matplotlib import pyplot


with open("final_image.txt", "r") as text_file:
    iksevi = text_file.readlines()
    kolone = iksevi[1].split(' ')

    imgArray = numpy.empty([len(iksevi),math.ceil((len(kolone))/3), 3], dtype=numpy.int8)
    xerino = 0
    for x in iksevi:
        brojevi = x.split(" ")
        counter = 0
        y = -1
        for broj in brojevi:
            if (counter % 3 == 0):
                y+=1
                counter = 0
            if (broj != '\n'):
             imgArray[xerino][y][counter] = int(broj)
            counter+=1
        xerino+=1

    img = Image.fromarray(imgArray,'RGB')
    img.show()

