#!/bin/bash

source ~cupsoft/cup_setup.sh 3.1.2
export LD_LIBRARY_PATH=/home/kkw/CUPDAQ/lib:$LD_LIBRARY_PATH

#export PATH=$PATH:$/home/kkw/CUPDAQ/bin
#export LD_LIBRARY_PATH
#source /home/kkw/muon_3.1.2/AutoProd/env.sh
#include 
###get runnum, subnum @ current subfile

#RAW_DIR="/home/gowoon/influxDB/dummy/RAW"
#LOG_DIR="/home/gowoon/influxDB/dummy/LOG"
#OUT_DIR="/home/gowoon/influxDB"
WORK_DIR="/home/kkw/muon_3.1.2/AutoProd"
RAW_DIR="/data/amore2test/RAW"
LOG_DIR="/data/amore2test/LOG"
OUT_DIR1="/home/kkw/PROD/WCMD"
OUT_DIR2="/home/kkw/PROD/PSMD"
MAC_DIR="/home/kkw/muon_3.1.2/AutoProd/MAC"

cd $RAW_DIR
run="$(find -type d | ls | tail -1)"
#echo $run
runnum=$(echo "${run}" | sed -r 's/^0+//g')
#printf "%s \n" ${run};
#printf "%d \n" ${runnum};

#RAW_DIR="/home/gowoon/influxDB/dummy/RAW/${run}"
RAW_DIR="/data/amore2test/RAW/${run}"

cd $RAW_DIR
#sub="$(find -type f -name "FADC*" | wc -l)"
#if [ ${sub} -eq 0 ]; then
#    sub="$(find -type f -name "IADCDAQ1*" | wc -l)"
#fi

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

let wsub=${wsub}-1
let psub=${psub}-1
let sub=${sub}-1


echo $wsub
echo $psub
echo $sub

#### WCMD prod

cd $OUT_DIR1

#LD_LIBRARY_PATH=/home/kkw/CUPDAQ/lib:$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH


if [ ! -d $run ]; then
    mkdir $run
fi

cd $OUT_DIR1/$run
WMACFILE="${WORK_DIR}/prod_wcd_sub.C"

#for ((iprod=0;iprod<$sub;iprod++));
for ((iprod=0;iprod<$wsub;iprod++));
#for iprod in {0..$sub}
do
    isub="prd_wcd_${run}_$(printf "%05d" ${iprod}).root"
    echo $isub
    if [ ! -e $isub ]; then
	wrunmac="${MAC_DIR}/prd_wcd_${iprod}.C"
#	echo $wrunmac
	sed -e s/SET01/prd_wcd_${iprod}/g -e s/SET02/${runnum}/g -e s/SET03/${iprod}/g ${WMACFILE} > ${wrunmac}
	cd $MAC_DIR
	root -q -l -b ${wrunmac}
	echo "file producted"
	chmod 644 $OUT_DIR1/$run/$isub
    else
	echo "file already producted"
    fi
done


#### PSMD prod

cd $OUT_DIR2
if [ ! -d $run ]; then
    mkdir $run
fi

cd $OUT_DIR2/$run
PMACFILE="${WORK_DIR}/prod_psd_sub.C"

#for ((iprod=0;iprod<$sub;iprod++));
for ((iprod=0;iprod<$psub;iprod++));
do
    isub="prd_psd_daq1_${run}_$(printf "%05d" ${iprod}).root"
    isub2="prd_psd_daq2_${run}_$(printf "%05d" ${iprod}).root"    
    echo $isub
    if [ ! -e $isub ]; then
	p1runmac="${MAC_DIR}/prd_psd1_${iprod}.C"
	p2runmac="${MAC_DIR}/prd_psd2_${iprod}.C"	
#	echo $wrunmac
	sed -e s/SET01/prd_psd1_${iprod}/g -e s/SET02/${runnum}/g -e s/SET03/${iprod}/g -e s/SET04/1/g ${PMACFILE} > ${p1runmac}
	sed -e s/SET01/prd_psd2_${iprod}/g -e s/SET02/${runnum}/g -e s/SET03/${iprod}/g -e s/SET04/2/g ${PMACFILE} > ${p2runmac}	
	cd $MAC_DIR
	root -q -l -b ${p1runmac}
	root -q -l -b ${p2runmac}	
	echo "file producted"
	chmod 644 $OUT_DIR2/$run/$isub
	chmod 644 $OUT_DIR2/$run/$isub2	
    else
	echo "file already producted"
    fi
done


#cd $WORK_DIR

#printf "%d \n" ${subnum};
