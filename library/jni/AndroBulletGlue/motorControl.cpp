/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: motorControl.cpp
 */
/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "motorControl.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_TYPE nativeCreate(JNIEnv *env, jobject thiz,
	jfloat goal, jfloat maxtorque) {

#if LOCAL_DEBUG
	LOGV("MotorControl#nativeCreate:");
#endif
	MotorControl *motor = new MotorControl();
	motor->goal = goal;
	motor->maxtorque = maxtorque;
	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_TYPE>(motor);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
	ID_TYPE id_motor) {

#if LOCAL_DEBUG
	LOGD("MotorControl#nativeDestroy:");
#endif
	MotorControl *motor = reinterpret_cast<MotorControl *>(id_motor);
	if (LIKELY(motor)) {
		SAFE_DELETE(motor);
	} else {
		LOGE("MotorControl#nativeDestroy:invalid id of motorControl");
	}
}

static jint nativeSet(JNIEnv *env, jobject thiz,
	ID_TYPE id_motor,
	jfloat goal, jfloat maxtorque) {

#if LOCAL_DEBUG
	LOGV("MotorControl#nativeSet:");
#endif
	MotorControl *motor = reinterpret_cast<MotorControl *>(id_motor);
	if (LIKELY(motor)) {
		motor->goal = goal;
		motor->maxtorque = maxtorque;
		return JNI_OK;
	} else {
		LOGE("MotorControl#nativeSet:invalid id of motorControl");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(FF)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeSet", "(JFF)I", (void *)nativeSet},
};

int register_MotorControl(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/softbody/MotorControl",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
