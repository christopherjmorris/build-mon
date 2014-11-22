import smbus
import time

import httplib2
h = httplib2.Http(".cache")

bus = smbus.SMBus(1)
address = 0x3b

h.add_credentials('admin', 'password')

while True:
    (resp_headers, status) = h.request("http://192.168.0.5:91/httpAuth/app/rest/builds/buildType:TestProject_TestBuild/status", "GET")
    if status == "SUCCESS":
        bus.write_byte_data(address, 0, 1)
    else:
        bus.write_byte_data(address, 0, 0)
    time.sleep(5)
