# VxWorks Makefile for RTI DDS Micro
CPU = ARMARCH7
TOOL = gnu
VXBUILD = SMP
SPACE = user

# RTI DDS Micro配置
RTIMEARCH = armvxWorks6.9gcc4.3.3_rtp
RTIMEHOME = ../../../

# 目录配置
BUILD_DIR = build
SRC_DIR = src

# VxWorks标准变量
TGT_DIR = $(WIND_BASE)/target/usr
OBJ_DIR = $(BUILD_DIR)

# 可执行文件输出到build目录
EXE = $(BUILD_DIR)/pub.vxe

# 设置源文件搜索路径
VPATH = $(SRC_DIR)

# 对象文件列表（VxWorks会自动处理路径）
OBJS = DataType.o DataTypeSupport.o DataTypePlugin.o DataTypeApplication.o Base.o pub.o

# 编译定义
EXTRA_DEFINE = -DRTI_VXWORKS -fno-builtin -DTOOL=gnu -DCPU=ARMARCH7 -DRTI_RTP \
               -DHAVE_MMAP -DHAVE_UNISTD_H -D__C99__ -D__vxworks__ -D__noudev__ \
               -DWLR_USE_UNSTABLE -std=c99 -fasm -Wall -Wsystem-headers -MD -MP

# 包含路径
EXTRA_INCLUDE = -I. -I$(SRC_DIR) -I$(RTIMEHOME)/include -I$(RTIMEHOME)/include/rti_me \
                -I$(WIND_USR)/h -I$(WIND_USR)/h/wrn/coreip

# 库配置
LIBS = -mrtp \
       -L$(RTIMEHOME)/lib/$(RTIMEARCH) -L$(RTIMEHOME)/lib/arm-wrs-vxworks \
       -lrti_me_rhsmz$(LIBSUFFIX) -lrti_me_whsmz$(LIBSUFFIX) \
       -lrti_me_discdpdez$(LIBSUFFIX) -lrti_me_discdpsez$(LIBSUFFIX) \
       -lrti_mez$(LIBSUFFIX) \
       -L$(WIND_BASE)/vxworks-6.9/target/lib_smp/usr/lib/arm/ARMARCH7/common \
       -lpthread

ADDED_LIBS = $(LIBS)

# 在包含rules.rtp之前创建build目录
$(shell mkdir -p $(BUILD_DIR))

# 包含VxWorks规则
include $(TGT_DIR)/make/rules.rtp

# 自定义清理规则（如果需要额外的清理操作）
distclean: clean
	rm -rf $(BUILD_DIR)

.PHONY: distclean
