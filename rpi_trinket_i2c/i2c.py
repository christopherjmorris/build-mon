import smbus
import time
bus = smbus.SMBus(1)
address = 0x3b

while True:
    bus.write_byte_data(address, 0, 1)
    time.sleep(0.5)
    bus.write_byte_data(address, 0, 0)
    time.sleep(0.5)
