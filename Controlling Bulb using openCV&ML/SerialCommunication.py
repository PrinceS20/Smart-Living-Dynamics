from cvzone.SerialModule import SerialObject
from time import sleep
arduino = SerialObject("COM3")
while True:
    data = arduino.getData()
    if data!= "":
        if data == "BulbON":
            arduino.sendData("1")  # Send signal to turn on the bulb
            print("Bulb is ON")
        elif data == "BulbOFF":
            arduino.sendData("0")  # Send signal to turn off the bulb
            print("Bulb is OFF")
    sleep(0.1)
