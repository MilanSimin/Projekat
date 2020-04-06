import cv2
import numpy
import sys

img = cv2.imread(str(sys.argv[1]))
im = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)

with open("image.txt", "w") as text_file:
    cut_size_x = 512
    cut_size_y = 512
    x_size = 0
    for x in im:
        y_size = 0
        x_size += 1
        if (x_size>cut_size_x) :
            break
        for y in x:
            y_size += 1
            if (y_size>cut_size_y) :
                break
            count = 0
            for z in y:
                count+=1
                print(z, file=text_file, end='')
                if count != 3:
                    print(' ', file=text_file, end='')
            print(' ', file=text_file, end='')
        print('\n', file=text_file, end='')


