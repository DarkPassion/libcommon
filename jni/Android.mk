LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE    := libcomon_jni

# 遍历目录及子目录的函数
define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# source code
ALLFILES = $(call walk, $(LOCAL_PATH)/../src/)
ALLFILES += $(call walk, $(LOCAL_PATH)/../3rd/jsoncpp/src/)

# test libcommon.a
#ALLFILES += $(LOCAL_PATH)/../test/block_queue.cpp

# 从所有文件中提取出所有.cpp和.c文件
FILE_LIST += $(filter %.cpp, $(ALLFILES))
FILE_LIST += $(filter %.c, $(ALLFILES))
FILE_LIST += $(filter %.cc, $(ALLFILES))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := ../include  \
	../3rd/jsoncpp/include 


LOCAL_CFLAGS := -O2 -g -pipe -W -Wall -std=c++11 -DWEBRTC_POSIX  -DPLATFORM_ANDROID -D_SIGSLOT_HAS_POSIX_THREADS
	
LOCAL_LDLIBS := -landroid -llog

ifeq ($(APP_OPTIM), debug)
    LOCAL_CFLAGS += -DANDROID_DEV
else
    LOCAL_CFLAGS += -DANDROID_PROD
endif


include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)

