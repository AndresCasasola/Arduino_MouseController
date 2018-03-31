# Arduino MouseController

- Arduino mousecontroller is an application to control the linux mouse with sensors diferent from the typical optic mouse or touchpad.

- These examples uses arduino-serial-library developed by todbot, https://github.com/todbot/arduino-serial

- This is a little first vesion which uses two fotoresistors sensibles to visible light to detect when something is covering the surface above every sensor. When the sensor detect something over him, that is sent to the program running on the Linux PC.
Intuitively the sensor placed to the right moves the mouse to the right and vice versa with the left.
The data for comunication between arduino and linux is a buffer of 50 bytes, where only 2 first bytes (char) are used to represent the on/off state of every sensor.

- In linux to control the mouse is nedeed to open and make write/read operations to the "uinput" file. This file is which hands the mouse.

# Typical Issues:

- uInput file without W/R privileges enoughs. uInput path: /dev/uinput. Solve: type on bash: sudo chmod 777 /dev/uinput



