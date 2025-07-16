RTIMEARCH ?= armvxWorks6.9gcc4.3.3_rtp
RTIMEHOME ?= ../../../

BUILD_DIR = build
SRC_DIR = src

TGT_DIR=$(WIND_BASE)/target/usr
EXE = sub.vxe

OBJ_DIR=$(shell pwd)

OBJS = DataType.o DataTypeSupport.o DataTypePlugin.o DataTypeApplication.o Base.o sub.o

EXTRA_DEFINE += -DRTI_VXWORKS -DRTI_RTP -DCPU=ARMARCH7 -D__vxworks__ -D__noudev__ \
	-DHAVE_MMAP -DWLR_USE_UNSTABLE \
	-fno-builtin -std=c99 -fasm -Wall -Wsystem-headers -MD -MP
EXTRA_INCLUDE += -I. -I$(RTIMEHOME)/include -I$(RTIMEHOME)/include/rti_me -I$(WIND_USR)/h -I$(WIND_USR)/h/wrn/coreip

LIBS = -mrtp \
	-L$(RTIMEHOME)/lib/$(RTIMEARCH) -L$(RTIMEHOME)/lib/arm-wrs-vxworks \
	-lrti_me_rhsmz$(LIBSUFFIX) -lrti_me_whsmz$(LIBSUFFIX) -lrti_me_discdpdez$(LIBSUFFIX) -lrti_me_discdpdez$(LIBSUFFIX) -lrti_mez$(LIBSUFFIX) \
	-L$(WIND_BASE)/target/lib_smp/usr/lib/arm/ARMARCH7/common

ADDED_LIBS += ${LIBS}

# 添加源文件路径
vpath %.c $(SRC_DIR)

include $(WIND_USR)/make/rules.rtp