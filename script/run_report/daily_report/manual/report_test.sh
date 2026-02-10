#!/bin/bash

source ~cupsoft/cup_setup.sh 3.1.2
export LD_LIBRARY_PATH=/home/kkw/CUPDAQ/lib:$LD_LIBRARY_PATH

cd /home/kkw/muon_3.1.2/daily_report
root -l report_wcd.C
