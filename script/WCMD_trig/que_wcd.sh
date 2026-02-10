#!/bin/bash

. ~cupsoft/setup_prod.sh 1.0
cd /home/kkw/muon_3.1.2/analysis/WCMD_trig

root -q -l trg_calculation_sub_pbs.C"("$1")"
	






