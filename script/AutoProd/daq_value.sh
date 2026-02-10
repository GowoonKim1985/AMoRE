#!/bin/bash

###get runnum, subnum @ current subfile

#RAW_DIR="/home/gowoon/influxDB/dummy/RAW"
#LOG_DIR="/home/gowoon/influxDB/dummy/LOG"
#OUT_DIR="/home/gowoon/influxDB"
RAW_DIR="/amore2/amore2test/RAW"
LOG_DIR="/amore2/amore2test/LOG"
OUT_DIR="/home/cupsoft/monitoring/rel"

cd $RAW_DIR
run="$(find -type d | ls | tail -1)"
#echo $run
runnum=$(echo "${run}" | sed -r 's/^0+//g')
printf "%s \n" ${run};
printf "%d \n" ${runnum};

#RAW_DIR="/home/gowoon/influxDB/dummy/RAW/${run}"
RAW_DIR="/amore2/amore2test/RAW/${run}"

cd $RAW_DIR
sub="$(find -type f -name "FADC*" | wc -l)"
if [ ${sub} -eq 0 ]; then
#    sub="$(find -type f -name "IADCDAQ1*" | wc -l)"
    sub="$(find -type f -name "IADCMERGER*" | wc -l)"
fi

let subnum=${sub}-1

#printf "%d \n" ${subnum};

OUTFILE="${OUT_DIR}/daq_value.txt"

echo -e "${runnum} \n\
${subnum}"      > ${OUTFILE}


: <<'END'
###lsusb result txt output
USBFILE="${OUT_DIR}/lsusb.txt"
#USBOUT=$(lsusb)
USBOUT1=$(ssh 192.168.2.151 lsusb)
#echo $USBOUT1
USBOUT2=$(ssh 192.168.2.161 lsusb)
#echo $USBOUT2
USBOUT3=$(ssh 192.168.2.162 lsusb)
#echo $USBOUT3
#USBOUT1=$(ssh muon ssh amoremon ssh 192.168.2.151 lsusb)
#USBOUT2=$(ssh muon ssh amoremon 192.168.2.161 lsusb)
#USBOUT3=$(ssh muon ssh amoremon 192.168.2.162 lsusb)

echo "$USBOUT1" > ${USBFILE}
echo "$USBOUT2" >> ${USBFILE}
echo "$USBOUT3" >> ${USBFILE}
END

###trg rate, N of count
FLOGFILE="${LOG_DIR}/FADCDAQ_${run}.log"
A1LOGFILE="${LOG_DIR}/IADCDAQ1_${run}.log"
A2LOGFILE="${LOG_DIR}/IADCDAQ2_${run}.log"

LOGFILE=($FLOGFILE $A1LOGFILE $A2LOGFILE)

cnt=()
rate=()
runstatus=()
###########trg info, rate&cnt

##for normal mode 
for ifile in {0..2}
do
    echo log $ifile
    if [ -e ${LOGFILE[ifile]} ]; then
	log=$(tail -1 ${LOGFILE[ifile]})
#	echo $log
	check=${log:20:6}
#	echo $check
	if [ "$check" = "[STAT]" ];then
	    
	    logline="${log%%Hz*}"
	    logline=$(echo $logline | sed 's/  */ /g')
#	    echo ${logline}
	    cntlog="${log#*triggered [}"
	    cntlog="${cntlog%% |*}"	    
#	    echo ${cntlog}
	    ratelog="${log#*/ }"
#	    echo ${ratelog}
	    ratelog="${ratelog%%(*}"
#	    echo ${ratelog}
#	    cnt[ifile]+=$(echo $cntlog | cut -d '[' -f2)
#            rate[ifile]+=$(echo $logline | cut -d ' ' -f12 | sed 's/(//' | sed 's/)//')
	    cnt[ifile]+=$(echo $cntlog)
	    rate[ifile]+=$(echo $ratelog)	    
	    runstatus[ifile]+=0.
	    echo count ${cnt[ifile]}
	    echo rate ${rate[ifile]}
	    echo runstatus ${runstatus[ifile]}
    	    echo -e "${cnt[ifile]}">>${OUTFILE}
	    echo -e "${rate[ifile]}">>${OUTFILE}
    	    echo -e "${runstatus[ifile]}">>${OUTFILE}	    	    
	else
#	    echo test
            cnt[ifile]+=0.
	    rate[ifile]+=0.
	    runstatus[ifile]+=1.
	    echo ${cnt[ifile]}
	    echo ${rate[ifile]}
	    echo ${runstatus[ifile]}	
	    echo -e "${cnt[ifile]}">>${OUTFILE}
	    echo -e "${rate[ifile]}">>${OUTFILE}
	    echo -e "${runstatus[ifile]}">>${OUTFILE}
	fi
    fi
done

cd $OUT_DIR

#a1log=$(tail -$i ${A2LOGFILE})
#a1check=${a1log:20:6}
#a1line="${a1log%%Hz*}"
#a1line=$(echo $a1line | sed 's/  */ /g')
#a1cnt=$(echo $a1line | cut -d ' ' -f9)
#a1rate=$(echo $a1line | cut -d ' ' -f13)


#WORK_DIR="/home/kkw"
#RUN_DIR="${WORK_DIR}/script/monitoring"
WORK_DIR="/home/cupsoft"
RUN_DIR="${WORK_DIR}/monitoring/rel"
LOG_DIR1="${RUN_DIR1}/log"
OUTFILE="grafana.txt"
LOGFILE1="FADCDAQ_${run}.log"

## out 1,2 (runnum, subrun)
printf "%d \n" ${runnum};
printf "%d \n" ${subnum};

cd $LOG_DIR
#tail -1 ${LOGFILE1}
fnum=$(cat ${LOGFILE1} | wc -l)
fline="$(tail -1 $LOGFILE1)"
ftxt="$(echo $fline | cut -c 28)"
#echo $fnum
#echo $ftxt

## out 3 (fadc rate)
if [ "$ftxt" = ":" ]; then
#    echo "running"
    frate=$(echo ${fline} | awk '{ print $14 }')
    frate=$(echo "${frate}" | tr -d ')')
else
#    echo "run finished"
    let fnum=${fnum}-24
    fline="$(head -n "$fnum" ${LOGFILE1} | tail -1)"
    frate=$(echo ${fline} | awk '{ print $4 }')
#    echo ${frate}
fi
echo $frate

cd $RUN_DIR


#rsync -avz --progress $LOG_DIR2/psmd.png muon:/monitor/www/html/psmd
#cd $PROD_DIR
#rm -rf $PROD_DIR/$FINFILE1
#rm -rf $PROD_DIR/$FINFILE2
#hadd $PROD_DIR/$FINFILE1 $PROD_DIR/$FINFILE1.*
#hadd $PROD_DIR/$FINFILE2 $PROD_DIR/$FINFILE2.*

#28
#sed -e s/SETN00/psmd_${runnum}/g -e s/SETN01/${run}/g -e s/SETN02/${sub1}/g -e s/SETN03/${sub2}/g $RUN_DIR/psmd_check.C > ${runmac1}
#sed -e s/SETN00/wcmd_${runnum}/g -e s/SETN01/${run}/g -e s/SETN02/${sub1}/g -e s/SETN03/${sub2}/g $RUN_DIR/wcmd_check.C > ${runmac2}



