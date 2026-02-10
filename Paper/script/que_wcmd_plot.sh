#!/bin/bash

. ~cupsoft/cup_setup.sh 3.1.2
cd /home/kkw/muon_3.1.2/script

root -q -l pbs_wcmd_plot.C"("$1")"
	






