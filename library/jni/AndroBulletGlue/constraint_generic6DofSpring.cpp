/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: constraint_generic6DofSpring.cpp
 */
/*
This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#include "constraint_generic6DofSpring.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static jint nativeEnableSpring(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint index,
		jboolean onOff) {

#if LOCAL_DEBUG
	LOGV("Generic6DofSpringConstraint#enableSpring:");
#endif
	btGeneric6DofSpringConstraint *constraint = reinterpret_cast<btGeneric6DofSpringConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->enableSpring(index, onOff);
		return JNI_OK;
	} else {
		LOGE("Generic6DofSpringConstraint#enableSpring: invalid id");
		return -1000;
	}
}

static jint nativeSetStiffness(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint index,
		jfloat stiffness) {

#if LOCAL_DEBUG
	LOGV("Generic6DofSpringConstraint#setStiffness:");
#endif
	btGeneric6DofSpringConstraint *constraint = reinterpret_cast<btGeneric6DofSpringConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setStiffness(index, stiffness);
		return JNI_OK;
	} else {
		LOGE("Generic6DofSpringConstraint#setStiffness: invalid id");
		return -1000;
	}
}

static jint nativeSetDamping(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint index,
		jfloat damping) {

#if LOCAL_DEBUG
	LOGV("Generic6DofSpringConstraint#setDamping:");
#endif
	btGeneric6DofSpringConstraint *constraint = reinterpret_cast<btGeneric6DofSpringConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setDamping(index, damping);
		return JNI_OK;
	} else {
		LOGE("Generic6DofSpringConstraint#setDamping: invalid id");
		return -1000;
	}
}

static jint nativeSetEquilibriumPoint(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint) {

#if LOCAL_DEBUG
	LOGV("Generic6DofSpringConstraint#setEquilibriumPoint:");
#endif
	btGeneric6DofSpringConstraint *constraint = reinterpret_cast<btGeneric6DofSpringConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setEquilibriumPoint();
		return JNI_OK;
	} else {
		LOGE("Generic6DofSpringConstraint#setEquilibriumPoint: invalid id");
		return -1000;
	}
}

static jint nativeSetEquilibriumPointIndex(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint index) {

#if LOCAL_DEBUG
	LOGV("Generic6DofSpringConstraint#setEquilibriumPoint:");
#endif
	btGeneric6DofSpringConstraint *constraint = reinterpret_cast<btGeneric6DofSpringConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setEquilibriumPoint(index);
		return JNI_OK;
	} else {
		LOGE("Generic6DofSpringConstraint#setEquilibriumPoint: invalid id");
		return -1000;
	}
}

static jint nativeSetEquilibriumPointIndexVal(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint index,
		jfloat val) {

#if LOCAL_DEBUG
	LOGV("Generic6DofSpringConstraint#setEquilibriumPoint:");
#endif
	btGeneric6DofSpringConstraint *constraint = reinterpret_cast<btGeneric6DofSpringConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setEquilibriumPoint(index, val);
		return JNI_OK;
	} else {
		LOGE("Generic6DofSpringConstraint#setEquilibriumPoint: invalid id");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeEnableSpring", "(JIZ)I", (void *)nativeEnableSpring},
	{"nativeSetStiffness", "(JIF)I", (void *)nativeSetStiffness},
	{"nativeSetDamping", "(JIF)I", (void *)nativeSetDamping},
	{"nativeSetEquilibriumPoint", "(J)I", (void *)nativeSetEquilibriumPoint},
	{"nativeSetEquilibriumPointIndex", "(JI)I", (void *)nativeSetEquilibriumPointIndex},
	{"nativeSetEquilibriumPointIndexVal", "(JIF)I", (void *)nativeSetEquilibriumPointIndexVal},
};

int register_Generic6DofSpringConstraint(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/constraint/Generic6DofSpringConstraint",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
