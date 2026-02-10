#!/bin/bash

#LD_LIBRARY_PATH=/home/kkw/CUPDAQ/lib:$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH

###get runnum, subnum @ current subfile

#RAW_DIR="/home/kkw/Muon/DATA/RAW"
#LOG_DIR="/home/kkw/Muon/DATA/LOG"
#OUT_DIR="/home/kkw/Muon/muon_3.1.2/daily_report/"
RAW_DIR="/data/amore2test/RAW"
LOG_DIR="/data/amore2test/LOG"
OUT_DIR="/home/kkw/muon_3.1.2/daily_report/"

cd $RAW_DIR
run="$(find -type d | ls | tail -1)"
#echo $run
runnum=$(echo "${run}" | sed -r 's/^0+//g')
#printf "%s \n" ${run};
#printf "%d \n" ${runnum};

#RAW_DIR="/home/kkw/Muon/DATA/RAW/${run}"
RAW_DIR="/data/amore2test/RAW/${run}"

cd $RAW_DIR

wsub=$(find "$RAW_DIR" -maxdepth 1 -type f -name "FADC_*" | wc -l)
psub=$(find "$RAW_DIR" -maxdepth 1 -type f -name "IADC1_*" | wc -l)

wflag=0
pflag=0
sub=$wsub

if [ ${wsub} -eq 0 ]; then
    wflag=1
    sub=$psub
fi
if [ ${psub} -eq 0 ]; then
    pflag=1
fi
#echo $wsub
#echo $psub
#echo $sub


#sub="$(find -type f -name "FADC*" | wc -l)"
#if [ ${sub} -eq 0 ]; then
#    sub="$(find -type f -name "IADCDAQ1*" | wc -l)"
#fi

let subnum=${sub}-1

#printf "%d \n" ${subnum};

OUTFILE="${OUT_DIR}/t_rate.txt"

echo -e "${runnum} \n\
${subnum}"      > ${OUTFILE}

###trg rate, N of count
FLOGFILE="${LOG_DIR}/FADCDAQ_${run}.log"
A1LOGFILE="${LOG_DIR}/IADCDAQ1_${run}.log"
A2LOGFILE="${LOG_DIR}/IADCDAQ2_${run}.log"

LOGFILE=($FLOGFILE $A1LOGFILE $A2LOGFILE)

cnt=()
rate=()
t_rate=()
runstatus=()
timetxt=()
###########trg info, rate&cnt

##for normal mode 
for ifile in {0..2}
do
#    echo log $ifile
    if [ -e ${LOGFILE[ifile]} ]; then
	for presub in  {5..1}
	do
	    let subcheck=$sub-$presub
	    findline=$(printf "root.%.5d" $subcheck)
#	    echo $findline
#	    echo $subcheck
	    log=$(cat ${LOGFILE[ifile]} | grep -B 2 $findline)
	    #log=$(sed -n '/root.00000/p' ${LOGFILE[ifile]})
	    #log=$(tail -1 ${LOGFILE[ifile]})
#	    echo $log	    
	    check=${log:20:6}
#	    echo $check
	    if [ "$check" = "[STAT]" ] && [ $subcheck -gt 0 ];then
		logline="${log%%Hz*}"
#		echo $logline
		logline=$(echo $logline | sed 's/  */ /g')
		#	    cnt[ifile]+=$(echo $logline | cut -d ' ' -f10)
		timetxt[ifile]=$(echo "$logline" | awk '{print $2}')
		rate[ifile]+=$(echo $logline | cut -d ' ' -f12 | sed 's/(//')
		t_rate[ifile]=$(echo $logline | awk -F'[()]' '{print $2}')
#		echo ${t_rate[ifile]}
#		echo ${timetxt[ifile]}

		#		echo count ${cnt[ifile]}
#		echo rate ${rate[ifile]}
#		echo runstatus ${runstatus[ifile]}
    #		echo -e "${runstatus[ifile]}">>${OUTFILE}	    
    	#	echo -e "${cnt[ifile]}">>${OUTFILE}
		#	echo -e "${rate[ifile]}">>${OUTFILE}
		echo -e "${timetxt[ifile]}">>${OUTFILE}
		echo -e "${t_rate[ifile]}">>${OUTFILE}	
	    else
		#	    echo test
#		cnt[ifile]+=0.
		#		rate[ifile]+=0.
		t_rate[ifile]=0.
		timetxt[ifile]="00:00:00"
		runstatus[ifile]+=1.
#		echo ${cnt[ifile]}
#		echo ${t_rate[ifile]}
#		echo ${timetxt[ifile]}	
	#	echo -e "${cnt[ifile]}">>${OUTFILE}
#		echo -e "${rate[ifile]}">>${OUTFILE}
		echo -e "${timetxt[ifile]}">>${OUTFILE}
		echo -e "${t_rate[ifile]}">>${OUTFILE}
		#		echo -e "${runstatus[ifile]}">>${OUTFILE}
	    fi
	done
    fi
done

#echo ${t_rate[0]}
#echo "test"
cd $OUT_DIR

#a1log=$(tail -$i ${A2LOGFILE})
#a1check=${a1log:20:6}
#a1line="${a1log%%Hz*}"
#a1line=$(echo $a1line | sed 's/  */ /g')
#a1cnt=$(echo $a1line | cut -d ' ' -f9)
#a1rate=$(echo $a1line | cut -d ' ' -f13)


#WORK_DIR="/home/kkw"
#RUN_DIR="${WORK_DIR}/script/monitoring"
WORK_DIR="/home/kkw/muon_3.1.2/daily_report"
RUN_DIR="${WORK_DIR}/"
LOG_DIR1="${RUN_DIR1}/log"
OUTFILE="grafana.txt"
LOGFILE1="FADCDAQ_${run}.log"

## out 1,2 (runnum, subrun)
#printf "%d \n" ${runnum};
#printf "%d \n" ${subnum};

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
#echo $frate

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



