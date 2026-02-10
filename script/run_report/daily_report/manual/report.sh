#!/bin/bash
echo "test"
source ~cupsoft/cup_setup.sh 3.1.2
export LD_LIBRARY_PATH=/home/kkw/CUPDAQ/lib:$LD_LIBRARY_PATH

cd /home/kkw/muon_3.1.2/daily_report/manual

python graph.py
echo "test1"
bash rate_value.sh
echo "test2"
root -l -b -q report_wcd.C
echo "test3"
root -l -b -q report_psd.C
echo "test4"
sleep 30
python report.py
sleep 10
#rsync -avh /home/kkw/muon_3.1.2/daily_report/pdf/* muon:/monitor/www/ymapp/app/templates/report/

