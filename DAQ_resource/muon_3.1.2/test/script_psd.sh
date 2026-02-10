#!/bin/bash

totrun=$(seq 0 $1)
cd /home/kmseo/Works/muon_3.1.2/test

for i in $totrun
do
    if [ `expr $i % 5` = 0 ] && [ $i != 0 ]
    then
	#echo $i
	root -q -l prod_psd.C"("$i", "$2")"
    else
	if [ $i = $1 ]
	then
	    #echo $i
	    root -q -l prod_psd.C"("$i", "$2")"
	else
	    #echo $i "&"
	    root -q -l prod_psd.C"("$i", "$2")" &
	fi
    fi
done
