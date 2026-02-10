#!/usr/bin/env bash

export MALLOC_CHECK_=0

OPTS=`getopt -o sfgur:c:v:p:d: --long onldaqdir:,rawdatadir:,version:,daqtime:,nevent:,hist -n 'executedaq.sh' -- "$@"`

if [ $? != 0 ]; then
    echo "Terminating..." >&2; exit 1;
fi

eval set -- "$OPTS"

SADC=
FADC=
GADC=
RUNNUM=
CONFIGFILE=
REPORTTIME=
SPLITTIME=
ONLDAQDIR=
RAWDATADIR=
VERSION=
DAQ=
HIST=
DAQTIME=
NEVENT=
TEST=

while true; do
    case "$1" in
    -s) SADC="-s"
        shift ;;
    -f) FADC="-f"
        shift ;;
    -g) GADC="-g"
        shift ;;
    -d) DAQ="$2"
        shift 2;;
    -r) RUNNUM="$2"
        shift 2;;
    -c) CONFIGFILE="-c $2"
        shift 2;;
    -v) REPORTTIME="-p $2"
        shift 2;;
    -p) SPLITTIME="-o $2"
        shift 2;;
    -u) TEST="-u"
        shift ;;
    --onldaqdir) ONLDAQDIR="$2"
        shift 2;;
    --rawdatadir) RAWDATADIR="$2"
        shift 2;;
    --version) VERSION="$2"
        shift 2;;
    --daqtime) DAQTIME="-t $2"
        shift 2;;
    --nevent) NEVENT="-n $2"
        shift 2;;
    --hist) HIST="-h"
        shift ;;
    --) shift;
        break ;;
    *) break ;;
    esac
done

if [ "$ONLDAQDIR" ]
then
    export ONLDAQ_DIR="$ONLDAQDIR"
fi

if [ "$RAWDATADIR" ]
then
    export RAWDATA_DIR="$RAWDATADIR"
fi

if [ "$VERSION" ]
then
    . /home/cupsoft/amore_setup.sh "$VERSION"
    export LD_LIBRARY_PATH=${ONLDAQ_DIR}/shlib/${BFARCH}:$LD_LIBRARY_PATH
fi

RUNNUMSTR=`printf "%06d" $RUNNUM`

case "$DAQ" in
tcb)
    ${ONLDAQ_DIR}/bin/${BFARCH}/tcb $TEST -r $RUNNUM $CONFIGFILE $SADC $FADC $GADC $SPLITTIME $DAQTIME $NEVENT > ${RAWDATA_DIR}/LOG/tcb_${RUNNUMSTR}.log 2>&1 &
    ;;
sadc)
    ${ONLDAQ_DIR}/bin/${BFARCH}/daq -s -r $RUNNUM $REPORTTIME $HIST > ${RAWDATA_DIR}/LOG/sadcdaq_${RUNNUMSTR}.log 2>&1 &
    ;;
fadc)
    ${ONLDAQ_DIR}/bin/${BFARCH}/daq -f -r $RUNNUM $REPORTTIME $HIST > ${RAWDATA_DIR}/LOG/fadcdaq_${RUNNUMSTR}.log 2>&1 &
    ;;
gadc)
    ${ONLDAQ_DIR}/bin/${BFARCH}/daq -g -r $RUNNUM $REPORTTIME $HIST > ${RAWDATA_DIR}/LOG/gadcdaq_${RUNNUMSTR}.log 2>&1 &
    ;;
*)
    exit
esac

exit

