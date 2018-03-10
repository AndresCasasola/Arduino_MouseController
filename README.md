# Arduino MouseController

- Arduino mousecontroller is an application to controls the linux mouse with sensors diferent from the typical optic mouse or touchpad.

- These examples uses arduino-serial-library developed by todbot, https://github.com/todbot/arduino-serial

- This is a little first vesion which uses two fotoresistors sensibles to visible light to detect when something is covering the surface above every sensor. When the sensor detects something over him that is sent to the program running on the Linux PC.
The data for comunication between arduino and linux is a buffer of 50 bytes, where only 2 first bytes (char) are used to represent the on/off state of every sensor.

In linux to control the mouse is nedeed to open and make write/read operations over the "uinput" file. This file is which hands the mouse.

# Typical Issues:

- uInput file without W/R privileges enoughs. uInput path: /dev/uinput



