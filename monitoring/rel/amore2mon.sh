#!/bin/bash

while :
do
    python3 /home/cupsoft/monitoring/rel/daqmon.py
    python3 /home/cupsoft/monitoring/rel/wcmdmon.py
    rsync -avh /data/amore2test/runcatalog.db muon:/monitor/www/php/
#    python3 /home/gowoon/influxDB/test.py    
#    python3 /etc/supervisord.d/test.py    
#    echo "test" >/home/gowoon/result.txt
    sleep 60
done    
