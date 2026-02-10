#!/bin/bash

source ~cupsoft/cup_setup.sh 3.1.2
export LD_LIBRARY_PATH=/home/kkw/CUPDAQ/lib:$LD_LIBRARY_PATH
RAWLOG="/data/amore2test/runcatalog.db"
COPYDIR="muon:/monitor/www/php/"
LOGFILE="/home/kkw/muon_3.1.2/AutoProd/runlog_rsync.log"

rsync -avh --progress "$RAWLOG" "$COPYDIR" >> "$LOGFILE" 2>&1
