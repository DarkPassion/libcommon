APP_CFLAGS += -fexceptions 
APP_STL := gnustl_static  
APP_CPPFLAGS += -frtti 
APP_PLATFORM := android-19
APP_ABI := armeabi-v7a
#armeabi
#APP_OPTIM := debug
APP_OPTIM := release

ifeq ($(APP_OPTIM), debug)
    APP_CFLAGS := -O0 -g $(APP_CFLAGS)
else
    APP_CFLAGS := -O2 -DNDEBUG -g $(APP_CFLAGS)
endif

