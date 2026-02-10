#!/usr/bin/python3

import sys
import socket
import struct
import datetime
from influxdb_client import InfluxDBClient

import subprocess

#
# Constants
#
kPORT = 7820
#kNCH = 48
kNCH = 47
kUNITSIZE = 4
kMESSLEN = 100*kUNITSIZE
kHVMON = 2

current_time = datetime.datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ')

#hvtable = "/home/cupsoft/monitoring/rel/6e6_230914.table"
hvtable = "/home/cupsoft/HVControl/6e6_231103.table"

process = subprocess.run(["/home/cupsoft/monitoring/rel/HVreader/build/hvreader","-t",hvtable], capture_output=True, text=True)
tempfile = open("/home/cupsoft/monitoring/rel/hv_value.txt")

lines=tempfile.readlines()
tempfile.close()
#print(lines)

value = []
for i in lines:
#    print(float(i))
    value.append(float(i))


'''
connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    connection.connect(('localhost', kPORT))
except socket.error as e:
    sys.exit(1)


cmd = struct.pack('2i', kHVMON, 0)
connection.send(cmd)
buf = connection.recv(2*kNCH*kUNITSIZE)
values = struct.unpack('%df' % (2*kNCH), buf)
'''

wcmd_status = {}

#for i in range(kNCH):
for i in range(kNCH):    
    id = i+1
    #vdict['V_%d' % id] = values[i]
    #vdict['I_%d' % id] = values[i+kNCH]
#    wcmd_status['V_%d' % id] = 1590.
#    wcmd_status['I_%d' % id] = 980.
    wcmd_status['V_%d' % id] = value[2*i]
    wcmd_status['I_%d' % id] = value[2*i+1]

'''
for i in range(kNCH):
    id = i+1
    #vdict['V_%d' % id] = values[i]
    #vdict['I_%d' % id] = values[i+kNCH]
    vdict['V_%d' % id] = 1590.
    vdict['I_%d' % id] = 980.
'''
#wcmd_status['I_1'] = 500.
point = [
    {
        "measurement": "wcmdstate",
        "time": current_time,
        "fields": wcmd_status
    }
]

print(point)

with InfluxDBClient(url="https://influxdb.amore2.yemilab.kr", token="C_tGUnSWL13PJkeR7oufi49Viw9wXAjQjKQV_81Lo0kab-ZxYQTLLi9g89PypQSHeyDItDzBYzM1lP_StsZC8g==", org="AMoRE2") as client:
    with client.write_api() as write_client:
        write_client.write("AMoRE2_Muon", "AMoRE2", point)



