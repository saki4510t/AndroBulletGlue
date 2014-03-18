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

MY_LOCAL_PATH	:= $(call my-dir)

######################################################################
# Make libbullet.a and libglues.a at the inner Android.mk
######################################################################
include $(CLEAR_VARS)
LOCAL_PATH		:= $(MY_LOCAL_PATH)
# Include all libs (built and prebuilt)
#include $(call all-subdir-makefiles)
include $(MY_LOCAL_PATH)/Bullet/Android.mk
include $(MY_LOCAL_PATH)/glues/Android.mk

######################################################################
# Make shared library libbulletglue.so
######################################################################
include $(CLEAR_VARS)
LOCAL_PATH		:= $(MY_LOCAL_PATH)

CFLAGS := -Werror

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
#compile with arm mode if the target is armeabi-v7a (Default is thumb mode)
#the file size of thumb mode is smaller than arm mode
#but the performance with arm mode is better
#LOCAL_ARM_MODE := arm
#when you want to enable NEON (but it does not work well)
#LOCAL_CFLAGS += -DBT_USE_NEON
#LOCAL_ARM_NEON := true
else
#comment out next line if complie with arm mode. (Default is thumb mode)
#LOCAL_ARM_MODE := arm
endif # TARGET_ARCH_ABI == armeabi-v7a

#libbulletglue.so
LOCAL_MODULE    := androbulletglue
LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/ \
        $(LOCAL_PATH)/Bullet \
        $(LOCAL_PATH)/Bullet/BulletCollision/BroadphaseCollision \
        $(LOCAL_PATH)/Bullet/BulletCollision/CollisionDispatch \
        $(LOCAL_PATH)/Bullet/BulletCollision/CollisionShapes \
        $(LOCAL_PATH)/Bullet/BulletCollision/NarrowPhaseCollision \
        $(LOCAL_PATH)/Bullet/BulletDynamics/ConstraintSolver \
        $(LOCAL_PATH)/Bullet/BulletDynamics/Dynamics \
        $(LOCAL_PATH)/Bullet/BulletDynamics/Vehicle \
        $(LOCAL_PATH)/Bullet/BulletDynamics/Character \
        $(LOCAL_PATH)/Bullet/BulletSoftBody \
        $(LOCAL_PATH)/Bullet/LinearMath

LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%)
LOCAL_CFLAGS += -DANDROID_NDK
LOCAL_CFLAGS += -DDISABLE_IMPORTGL			# when static link OpenGL|ES library

# if you want to use exception support of C++
# don't forget to use runtime library with exception support(like GNU STL)
# (you should set APP_STL in Application.mk gnustl_static or gnustl_shared)
#LOCAL_CPP_FEATURES += exceptions 

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl	# to avoid NDK issue(no need for static library)
#LOCAL_LDLIBS += -lm						# math...automaticalliy linked by Android NDK
#LOCAL_LDLIBS += -lpthread					# pthread...automaticalliy linked by Android NDK
#LOCAL_LDLIBS += -lz						# zlib of Android NDK
LOCAL_LDLIBS += -llog						# log output library
LOCAL_LDLIBS += -lGLESv1_CM					# OpenGL|ES 1.1 library
#LOCAL_LDLIBS += -lGLESv2					# OpenGL|ES 2.0 library
# LOCAL_LDLIBS += -landroid					# Android native related library(when you use nativeActivity)
# LOCAL_LDLIBS += -OpenMAXAL				# OpenMAXAL for Android

#external modules that link dynamically
#LOCAL_SHARED_LIBRARIES :=

#external modules that link statically
#LOCAL_STATIC_LIBRARIES := android_native_app_glue	# NativeActivityGlue
LOCAL_STATIC_LIBRARIES += bullet			# original Bullet library itself
LOCAL_STATIC_LIBRARIES += glues				# GLU|ES library

# source files that we want to compile and link into current module
LOCAL_SRC_FILES := \
        AndroBulletGlue/_onload.cpp \
        AndroBulletGlue/utils.cpp \
        AndroBulletGlue/mygl.cpp \
        AndroBulletGlue/vector3.cpp \
        AndroBulletGlue/quaternion.cpp \
        AndroBulletGlue/matrix3x3.cpp \
        AndroBulletGlue/transform.cpp \
        AndroBulletGlue/GLShapeDrawer.cpp \
        AndroBulletGlue/GLDebugDrawer.cpp \
        AndroBulletGlue/GLDebugFont.cpp \
        AndroBulletGlue/GLBulletWorldSurfaceView.cpp \
        AndroBulletGlue/bulletmanager.cpp \
        AndroBulletGlue/physicsWorld.cpp \
        AndroBulletGlue/bulletWorldDraw.cpp \
        AndroBulletGlue/shape.cpp \
        AndroBulletGlue/shape_compound.cpp \
        AndroBulletGlue/shape_convexHull.cpp \
        AndroBulletGlue/shape_multiMaterialTriangleMesh.cpp \
        AndroBulletGlue/rigidBody.cpp \
        AndroBulletGlue/softBody.cpp \
        AndroBulletGlue/motorControl.cpp \
        AndroBulletGlue/steerControl.cpp \
        AndroBulletGlue/softbodyMaterial.cpp \
        AndroBulletGlue/constraint.cpp \
        AndroBulletGlue/constraint_coneTwist.cpp \
        AndroBulletGlue/constraint_gear.cpp \
        AndroBulletGlue/constraint_generic6Dof.cpp \
        AndroBulletGlue/constraint_generic6DofSpring.cpp \
        AndroBulletGlue/constraint_hinge.cpp \
        AndroBulletGlue/constraint_point2Point.cpp \
        AndroBulletGlue/vehicle.cpp

#link as a shared library
include $(BUILD_SHARED_LIBRARY)

#path for external modules
#$(call import-module,android/native_app_glue)
