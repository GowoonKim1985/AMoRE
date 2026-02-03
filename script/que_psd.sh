#!/bin/bash

. ~cupsoft/cup_setup.sh 3.1.2
cd /home/kkw/muon_3.1.2/test

root -q -l prod_psd.C"("$1", 1)"
root -q -l prod_psd.C"("$1", 2)"
