#!/bin/bash


source ~cupsoft/cup_setup.sh 3.1.2
export LD_LIBRARY_PATH=/home/kkw/CUPDAQ/lib:$LD_LIBRARY_PATH
WORK_DIR="/home/kkw/muon_3.1.2/AutoProd"

cd $WORK_DIR
root -l prod_wcd_test.C
