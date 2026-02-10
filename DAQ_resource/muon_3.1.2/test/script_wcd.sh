#!/bin/bash

#echo $1

totrun=$(seq 0 $1)
cd /home/kmseo/Works/muon_3.1.2/test

for i in $totrun
do
    if [ `expr $i % 5` = 0 ] && [ $i != 0 ]
    then
	#echo $i
	root -q -l prod_wcd.C"("$i")"
    else
	if [ $i = $1 ]
	then
	    #echo $i
	    root -q -l prod_wcd.C"("$i")"
	else
	    #echo $i "&"
	    root -q -l prod_wcd.C"("$i")" &
	fi
    fi
done







