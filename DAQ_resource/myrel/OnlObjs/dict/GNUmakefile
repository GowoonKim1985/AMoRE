ifdef LINK_SHARED
  LINK_SHARED_MODULES = yes
  SHAREDLIB = libOnlObjs.so
else
  sharedlib_o_dir = $(staticlib_o_dir)
  LIB       = libOnlObjs.a
endif

LIBCXXFILES = $(filter-out $(skip_files), $(wildcard $(sharedlib_o_dir)*_dict.cxx))

include SoftRelTools/standard.mk
include SoftRelTools/refresh.mk
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_rootcint.mk
