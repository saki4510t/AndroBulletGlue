#/*
# * AndroBullet
# * Bullet Continuous Collision Detection and Physics Library for Android
# * Copyright(c) 2013,2014 t_saki@serenegiant
# * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
#
#This software is provided 'as-is', without any express or implied warranty.
#In no event will the authors be held liable for any damages arising from the use of this software.
#Permission is granted to anyone to use this software for any purpose,
#including commercial applications, and to alter it and redistribute it freely,
#subject to the following restrictions:
#
#1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
#2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
#3. This notice may not be removed or altered from any source distribution.
#*/

#MY_LOCAL_PATH	:= $(call my-dir)
######################################################################
#libglues.a
#glues
######################################################################
include $(CLEAR_VARS)
LOCAL_PATH		:= $(MY_LOCAL_PATH)/glues

CFLAGS := -Werror

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
#compile with arm mode if the target is armeabi-v7a (Default is thumb mode)
#the file size of thumb mode is little bit smaller than arm mode
#but the performance with arm mode is better
#LOCAL_ARM_MODE := arm
#when enable NEON (but it does not work well)
#LOCAL_CFLAGS += -DBT_USE_NEON
#LOCAL_ARM_NEON := true
else
#comment out next line if complie with arm mode. (Default is thumb mode)
#LOCAL_ARM_MODE := arm
endif # TARGET_ARCH_ABI == armeabi-v7a

#libbullet.a
LOCAL_MODULE    := glues
LOCAL_C_INCLUDES := $(LOCAL_PATH)/

LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%)
LOCAL_CFLAGS += -DANDROID_NDK 
#LOCAL_CFLAGS += -DDISABLE_IMPORTGL			# when static link OpenGL|ES library

# if you want to use exception support of C++
# don't forget to use runtime library with exception support(like GNU STL)
# (you should set APP_STL in Application.mk gnustl_static or gnustl_shared)
#LOCAL_CPP_FEATURES += exceptions 

#LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl	# to avoid NDK issue(no need for static library)
#LOCAL_LDLIBS += -lm						# math...automaticalliy linked by Android NDK
#LOCAL_LDLIBS += -lpthread					# pthread...automaticalliy linked by Android NDK
#LOCAL_LDLIBS += -lz						# zlib of Android NDK
#LOCAL_LDLIBS += -llog						# log output library
#LOCAL_LDLIBS += -lGLESv1_CM				# OpenGL|ES 1.1 library
#LOCAL_LDLIBS += -lGLESv2					# OpenGL|ES 2.0 library
#LOCAL_LDLIBS += -landroid					# Android native related library(when you use nativeActivity)
#LOCAL_LDLIBS += -OpenMAXAL					# OpenMAXAL for Android

#external modules that link dynamically
#LOCAL_SHARED_LIBRARIES :=
#external modules that link statically
#LOCAL_STATIC_LIBRARIES :=

# source files that we want to compile and link into current module
LOCAL_SRC_FILES := \
	    glues_error.c \
    	glues_mipmap.c \
    	glues_project.c \
    	glues_quad.c \
    	glues_registry.c

#link as a static library
include $(BUILD_STATIC_LIBRARY)
