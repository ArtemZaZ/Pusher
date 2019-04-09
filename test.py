import serial
import time

ser = serial.Serial('/dev/ttyUSB1', baudrate=9600)     # открываем COM порт

for _ in range(4):
    ser.write(b'\xAA')  # отправляем байт, который активирует толкатель
    time.sleep(3)


for _ in range(50):
    print(ord(ser.read(1)))   # читаем 10 байт и выводим их

ser.close()
