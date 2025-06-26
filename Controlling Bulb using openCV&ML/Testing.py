import cv2
from cvzone.HandTrackingModule import HandDetector
from cvzone.ClassificationModule import Classifier
import serial
import numpy as np
import math


arduino = serial.Serial('COM3')
cap = cv2.VideoCapture(0)
detector = HandDetector(maxHands=1)
classifier = Classifier('Model/keras_model.h5','Model/labels.txt')


offset = 20
imgSize = 200


labels =["BulbOFF","BulbON"]
while True:
    success ,img = cap.read()
    imgOutput = img.copy()
    hands, img = detector.findHands(img)
    if hands:
        hand = hands[0]
        x,y,w,h = hand['bbox']
        # Ensure the bounding box coordinates are within the image boundaries
        x = max(0, x - offset)
        y = max(0, y - offset)
        w = min(img.shape[1] - x, w + offset)
        qh = min(img.shape[0] - y, h + offset)
       
        imgCrop = img [y:y + h, x:x + w]
       
        aspectratio = h/w
        if aspectratio > 1:
            k = imgSize/h
            wCal = math.ceil(k*w)
            imgResize = cv2.resize(imgCrop,(wCal,imgSize))
           
            imgResizeShape = imgResize.shape
            wGap = math.ceil((imgSize - wCal)/2)
            imgWhite = np.ones((imgSize,imgSize,3),np.uint8)*255
            imgWhite[:,wGap:wCal+wGap] = imgResize
            prediction,index =classifier.getPrediction(imgWhite)
            print(prediction,index)


        else:
            k = imgSize/w
            hCal = math.ceil(k*h)
            imgResize = cv2.resize(imgCrop, (imgSize,hCal))
            imgResizeShape = imgResize.shape
            hGap = math.ceil((imgSize - hCal)/2)
            imgWhite = np.ones((imgSize,imgSize,3),np.uint8)*255
            imgWhite[hGap:hCal+hGap,:] = imgResize
            prediction,index =classifier.getPrediction(imgWhite)
       
        cv2.putText(imgOutput,labels[index],(x,y-20),cv2.FONT_HERSHEY_COMPLEX,2,(255,0,255),2)
        cv2.rectangle(imgOutput,(x-offset,y-offset),(x+w+offset,y+h+offset),(0,255,0),4)
        cv2.imshow("ImageCrop",imgCrop)
        cv2.imshow("ImageWhite",imgWhite)
       


       
 # Send data to Arduino based on model prediction
        if index == 0:  # BulbOFF
             arduino.write(b'0')
        elif index == 1:  # BulbON
             arduino.write(b'1')
           
    cv2.imshow("Image",imgOutput)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
