LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_FMGEN_SRC = ./m_s98/device/fmgen/file.cpp \
./m_s98/device/fmgen/fmgen.cpp \
./m_s98/device/fmgen/fmtimer.cpp \
./m_s98/device/fmgen/opm.cpp \
./m_s98/device/fmgen/opna.cpp \
./m_s98/device/fmgen/psg.cpp

MY_M_S98_SRC = ./m_s98/device/s98fmgen.cpp \
./m_s98/device/s98mame.cpp \
./m_s98/device/s98opll.cpp \
./m_s98/device/s98sng.cpp \
./m_s98/device/emu2413/emu2413.c \
./m_s98/device/mame/fmopl.c \
./m_s98/device/mame/ymf262.c \
./m_s98/device/s_logtbl.c \
./m_s98/device/s_sng.c \
./m_s98/m_s98.cpp

LOCAL_MODULE    := m_s98
LOCAL_SRC_FILES := jni.cpp $(MY_M_S98_SRC) $(MY_FMGEN_SRC)
LOCAL_CFLAGS    += -DUSE_ZLIB -I$(LOCAL_PATH)/m_s98 -I.. 
LOCAL_LDLIBS	+= -lz
LOCAL_ARM_MODE	:= arm

include $(BUILD_SHARED_LIBRARY)
