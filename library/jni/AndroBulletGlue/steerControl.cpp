/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: steerControl.cpp
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

#include "steerControl.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_TYPE nativeCreate(JNIEnv *env, jobject thiz,
	jfloat goal, jfloat maxtorque, jfloat sign) {

#if LOCAL_DEBUG
	LOGV("SteerControl#nativeCreate:");
#endif
	SteerControl *steer = new SteerControl(sign);
	steer->goal = goal;
	steer->maxtorque = maxtorque;
	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_TYPE>(steer);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
	ID_TYPE id_steer) {

#if LOCAL_DEBUG
	LOGD("SteerControl#nativeDestroy:");
#endif
	SteerControl *steer = reinterpret_cast<SteerControl *>(id_steer);
	if (LIKELY(steer)) {
		SAFE_DELETE(steer);
	} else {
		LOGE("SteerControl#nativeDestroy:invalid id of steerControl");
	}
}

static jint nativeSet(JNIEnv *env, jobject thiz,
	ID_TYPE id_steer,
	jfloat goal, jfloat maxtorque) {

#if LOCAL_DEBUG
	LOGV("SteerControl#nativeSet:");
#endif
	SteerControl *steer = reinterpret_cast<SteerControl *>(id_steer);
	if (LIKELY(steer)) {
		steer->goal = goal;
		steer->maxtorque = maxtorque;
		return JNI_OK;
	} else {
		LOGE("SteerControl#nativeSet:invalid id of steerControl");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(FFF)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeSet", "(JFF)I", (void *)nativeSet},
};

int register_SteerControl(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/softbody/SteerControl",
		methods, NUM_ARRAY_ELEMENTS(methods));
}

