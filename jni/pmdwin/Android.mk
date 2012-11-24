LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_FMGEN_SRC = ./fmgen/file.cpp \
./fmgen/fmgen.cpp \
./fmgen/fmtimer.cpp \
./fmgen/opm.cpp \
./fmgen/opna.cpp \
./fmgen/psg.cpp

MY_PMDWIN_SRC = ./opnaw.cpp \
./p86drv.cpp \
./pmdwin.cpp \
./ppsdrv.cpp \
./ppz8l.cpp \
./table.cpp \
./util.cpp

LOCAL_MODULE    := pmdwin
LOCAL_SRC_FILES := jni.cpp $(MY_FMGEN_SRC) $(MY_PMDWIN_SRC)
LOCAL_CFLAGS    += -I.. -I$(LOCAL_PATH) -I$(LOCAL_PATH)/fmgen
LOCAL_LDLIBS	+= -lm -lz -llog
LOCAL_ARM_MODE	:= arm

include $(BUILD_SHARED_LIBRARY)
