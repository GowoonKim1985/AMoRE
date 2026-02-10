#!/usr/bin/python3

import sqlite3
import sys
import socket
import struct
import datetime
import re
import time
import os
#import influxdb_client
from influxdb_client import InfluxDBClient
from sqlite3 import Error

import subprocess

###time
current_time = datetime.datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ')
print(current_time)

###txt output
#######################
#os.system("/home/cupsoft/monitoring/rel/daq_value.sh")#txtfile make(runinfo, usb connection)
#os.system("/home/gowoon/influxDB/daq_value.sh")#txtfile make(runinfo, usb connection)
process0 = subprocess.call(['/home/cupsoft/monitoring/rel/daq_value.sh'])

#######################
time.sleep(10)

###runnum, subrunnum
#####################################
tempfile = open("/home/cupsoft/monitoring/rel/daq_value.txt")
#tempfile = open("/home/gowoon/influxDB/daq_value.txt")
####################################
lines=tempfile.readlines()
tempfile.close()

print(lines) # runnum, subnum, run, wcmd cnt-rate-status, iadc1 cnt-rate-status, iadc2 cnt-rate-status
value = []

for i in lines:
    print(float(i))
    value.append(float(i))

#print ("test")

###module connection
moduleid = ['0547:1501', '0547:1502', '0547:2010'] # dummyid... will be changed -> tcb, fadc, iadc
#moduleid = ['0547:1501', '0547:1502', '0547:2010'] # dummyid... will be changed -> tcb, fadc, iadc1
modulenum = [1, 12, 12]

process1 = subprocess.run(["ssh","172.16.2.50","lsusb"], capture_output=True, text=True)
process2 = subprocess.run(["ssh","172.16.2.71","lsusb"], capture_output=True, text=True)
process3 = subprocess.run(["ssh","172.16.2.72","lsusb"], capture_output=True, text=True)
result1=process1.stdout
result2=process2.stdout
result3=process2.stdout
usbtxt = open("/home/cupsoft/monitoring/rel/lsusb.txt",'w')
usbtxt.write(result1)
usbtxt.write(result2)
usbtxt.write(result3)
usbtxt.close()

tempfile = open("/home/cupsoft/monitoring/rel/lsusb.txt")
#tempfile = open("/home/gowoon/influxDB/lsusb_dummy.txt")
lines=tempfile.readlines()
tempfile.close()

nusb=len(lines)
usbid = []

for i in range(nusb):
#    print(iusb)
#    print(lines[iusb])
    usbid.append(lines[i][23:32])
    
monum = [0.,0.,0.]# number of modules
mocheck = [1.,1.,1.] # 0 connected, 1 disconnecteed
for i in range(3):
    for j in range(nusb):
#        print(i,":",j)
#       print(moduleid[i])
#        print(usbid[j])
        if(usbid[j]==moduleid[i]):
            monum[i]=monum[i]+1

if(monum[0]==modulenum[0]):
    mocheck[0]=0.
#    print('tcb connect')
if(monum[1]==modulenum[1]):
    mocheck[1]=0.
#    print('wcmd 12 connect')
if(monum[2]==modulenum[2]):
    mocheck[2]=0.    
#    print('psmd 12 connect')

### shift, runtype read from db
#con = sqlite3.connect('/home/gowoon/influxDB/dummy/runcatalog.db')
con = sqlite3.connect('/amore2/amore2test/runcatalog.db')
cursor = con.cursor()
cursor.execute("SELECT * FROM 'runcatalog' ORDER BY ROWID DESC LIMIT 1")
for row in cursor:
#    print(row[1])#runtype
#    print(row[3])#shift
    shift=row[3]
    runtype=row[1]

    
### values summary     

daq_status = {}
print("error test")        

#0: on, 1: down, 2: finished, 3: no info

#daq_status['TCBUSB'] = 0.
#daq_status['FADCUSB'] = 0.
#daq_status['IADC1USB'] = 0.
#daq_status['IADC2USB'] = 0.
daq_status['TCBUSB'] = mocheck[0]
daq_status['FADCUSB'] = mocheck[1]
daq_status['IADC1USB'] = mocheck[2]
#daq_status['IADC2USB'] = mocheck[3]

#daq_status['WCMDDAQ'] = 1.
#daq_status['PSMD1DAQ'] = 0.
#daq_status['PSMD2DAQ'] = 0.
daq_status['WCMDDAQ'] = value[4]
daq_status['PSMD1DAQ'] = value[7]
daq_status['PSMD2DAQ'] = value[10]

#daq_status['RunNumber'] = 100.
#daq_status['RunNumber'] = 50.
daq_status['RunNumber'] = value[0]
daq_status['SubNumber'] = value[1]

daq_info = {}
#daq_info['RunType'] = 'Physics'
#daq_info['Shift'] = "GWKim"
daq_info['Shift'] = shift
daq_info['RunType'] = runtype


daq_trg = {}
#daq_trg['IADC1TRG'] = 100.
#daq_trg['IADC2TRG'] = 100.
#daq_trg['FADCTRG'] = 100.
#daq_trg['IADC1CNT'] = 10000.
#daq_trg['IADC2CNT'] = 10000.
#daq_trg['FADCCNT'] = 10000.
daq_trg['FADCCNT'] = value[2]
daq_trg['FADCTRG'] = value[3]
daq_trg['IADC1CNT'] = value[5]
daq_trg['IADC1TRG'] = value[6]
daq_trg['IADC2CNT'] = value[8]
daq_trg['IADC2TRG'] = value[9]

point1 = [
    {
        "measurement": "daqstate",
        "time": current_time,
        "fields": daq_status
    }
]

point2 = [
    {
        "measurement": "daqinfo",
        "time": current_time,
        "fields": daq_info
    }
]

point3 = [
    {
        "measurement": "daqtrg",
        "time": current_time,
        "fields": daq_trg
    }
]

#print(point1)
#print(point2)
#print(point3)


with InfluxDBClient(url="https://influxdb.amore2.yemilab.kr", token="C_tGUnSWL13PJkeR7oufi49Viw9wXAjQjKQV_81Lo0kab-ZxYQTLLi9g89PypQSHeyDItDzBYzM1lP_StsZC8g==", org="AMoRE2") as client:
    with client.write_api() as write_client:
        write_client.write("AMoRE2_Muon", "AMoRE2", point1)
        write_client.write("AMoRE2_Muon", "AMoRE2", point2)
        write_client.write("AMoRE2_Muon", "AMoRE2", point3)

print("error test")        
        


