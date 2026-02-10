#############################################################
# example Makefile for linking 
# uses SoftRelTools/standard.mk
#############################################################

BINS = stddaq tcbdaq tcb #daq enddaq usbreset destroyshm

SIMPLEBINS = $(BINS) 

#framework general 

override LOADLIBES += -lOnlObjs
override LOADLIBES += -lRawObjs
override LOADLIBES += -lHDF5Utils
override LOADLIBES += -lNotice
override LOADLIBES += -lDAQConfig
override LOADLIBES += -lDAQSystem
override LOADLIBES += -lDAQUtils
override LOADLIBES += -lDAQTrigger
override LOADLIBES += -lOnlHistogramer
override LOADLIBES += -lDAQ

############################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_hdf5.mk
include SoftRelTools/arch_spec_libusb1.mk
