#!/bin/bash

source ~cupsoft/cup_setup.sh 3.1.2
export LD_LIBRARY_PATH=/home/kkw/CUPDAQ/lib:$LD_LIBRARY_PATH

cd /home/kkw/muon_3.1.2/daily_report

python graph.py 
bash rate_value.sh
root -l -b -q report_wcd.C
root -l -b -q report_psd.C
sleep 30
python report.py
sleep 10
rsync -avh /home/kkw/muon_3.1.2/daily_report/pdf/* muon:/monitor/www/ymapp/app/templates/report/

