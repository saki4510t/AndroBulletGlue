/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: constraint_generic6Dof.cpp
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


#include "constraint_generic6Dof.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static jint nativeSetLimit(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint axis,
		jfloat low, jfloat high) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#nativeSetLimit");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setLimit(axis, low, high);
	} else {
		LOGE("Generic6DofConstraint#setLinearLowerLimit: invalid id");
	}
}

static jint nativeSetLinearLowerLimitVec(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		ID_VEC id_vec_lowerLimit) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#nativeSetLinearLowerLimitVec");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	btVector3 *lowerLimit = reinterpret_cast<btVector3 *>(id_vec_lowerLimit);
	if (LIKELY(constraint && lowerLimit)) {
		constraint->setLinearLowerLimit(*lowerLimit);
	} else {
		LOGE("Generic6DofConstraint#nativeSetLinearLowerLimitVec: invalid id");
	}
}

static jint nativeSetLinearLowerLimit(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jfloat lower_x, jfloat lower_y, jfloat lower_z) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#setLinearLowerLimit");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setLinearLowerLimit(btVector3(lower_x, lower_y, lower_z));
	} else {
		LOGE("Generic6DofConstraint#setLinearLowerLimit: invalid id");
	}
}

static jint nativeSetLinearUpperLimitVec(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		ID_VEC id_vec_upperLimit) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#nativeSetLinearUpperLimitVec");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	btVector3 *upperLimit = reinterpret_cast<btVector3 *>(id_vec_upperLimit);
	if (LIKELY(constraint && upperLimit)) {
		constraint->setLinearUpperLimit(*upperLimit);
	} else {
		LOGE("Generic6DofConstraint#nativeSetLinearUpperLimitVec: invalid id");
	}
}

static jint nativeSetLinearUpperLimit(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jfloat upper_x, jfloat upper_y, jfloat upper_z) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#setLinearUpperLimit");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setLinearUpperLimit(btVector3(upper_x, upper_y, upper_z));
	} else {
		LOGE("Generic6DofConstraint#setLinearUpperLimit: invalid id");
	}
}

static jint nativeSetAngularLowerLimitVec(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		ID_VEC id_vec_lowerLimit) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#nativeSetAngularLowerLimitVec");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	btVector3 *lowerLimit = reinterpret_cast<btVector3 *>(id_vec_lowerLimit);
	if (LIKELY(constraint && lowerLimit)) {
		constraint->setAngularLowerLimit(*lowerLimit);
	} else {
		LOGE("Generic6DofConstraint#nativeSetAngularLowerLimitVec: invalid id");
	}
}

static jint nativeSetAngularLowerLimit(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jfloat lower_x, jfloat lower_y, jfloat lower_z) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#setAngularLowerLimit");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setAngularLowerLimit(btVector3(lower_x, lower_y, lower_z));
	} else {
		LOGE("Generic6DofConstraint#setAngularLowerLimit: invalid id");
	}
}

static jint nativeSetAngularUpperLimitVec(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		ID_VEC id_vec_upperLimit) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#nativeSetAngularUpperLimitVec");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	btVector3 *upperLimit = reinterpret_cast<btVector3 *>(id_vec_upperLimit);
	if (LIKELY(constraint && upperLimit)) {
		constraint->setAngularUpperLimit(*upperLimit);
	} else {
		LOGE("Generic6DofConstraint#nativeSetAngularUpperLimitVec: invalid id");
	}
}

static jint nativeSetAngularUpperLimit(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jfloat upper_x, jfloat upper_y, jfloat upper_z) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#setAngularUpperLimit");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setAngularUpperLimit(btVector3(upper_x, upper_y, upper_z));
	} else {
		LOGE("Generic6DofConstraint#setAngularUpperLimit: invalid id");
	}
}

static jint nativeSetTranslationalLimitMotorEnableMotor(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint ix,
		jboolean enable) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#setTranslationalLimitMotorEenableMotor:");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->getTranslationalLimitMotor()->m_enableMotor[ix] = enable;
	} else {
		LOGE("Generic6DofConstraint#setTranslationalLimitMotorEenableMotor: invalid id");
	}
}

static jint nativeSetTranslationalLimitMotorTargetVelocity(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint ix,
		jfloat velocity) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#setTranslationalLimitMotorTargetVelocity:");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->getTranslationalLimitMotor()->m_targetVelocity[ix] = velocity;
	} else {
		LOGE("Generic6DofConstraint#setTranslationalLimitMotorTargetVelocity: invalid id");
	}
}

static jint nativeSetTranslationalLimitMotorMaxMotorForce(
		JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint ix,
		jfloat force) {

#if LOCAL_DEBUG
	LOGV("Generic6DofConstraint#setTranslationalLimitMotorMaxMotorForce:");
#endif
	btGeneric6DofConstraint *constraint = reinterpret_cast<btGeneric6DofConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->getTranslationalLimitMotor()->m_maxMotorForce[ix] = force;
	} else {
		LOGE("Generic6DofConstraint#setTranslationalLimitMotorMaxMotorForce: invalid id");
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeSetLimit", "(JIFF)I", (void *)nativeSetLimit},
	{"nativeSetLinearLowerLimitVec", "(JJ)I", (void *)nativeSetLinearLowerLimitVec},
	{"nativeSetLinearLowerLimit", "(JFFF)I", (void *)nativeSetLinearLowerLimit},
	{"nativeSetLinearUpperLimitVec", "(JJ)I", (void *)nativeSetLinearUpperLimitVec},
	{"nativeSetLinearUpperLimit", "(JFFF)I", (void *)nativeSetLinearUpperLimit},
	{"nativeSetAngularLowerLimitVec", "(JJ)I", (void *)nativeSetAngularLowerLimitVec},
	{"nativeSetAngularLowerLimit", "(JFFF)I", (void *)nativeSetAngularLowerLimit},
	{"nativeSetAngularUpperLimitVec", "(JJ)I", (void *)nativeSetAngularUpperLimitVec},
	{"nativeSetAngularUpperLimit", "(JFFF)I", (void *)nativeSetAngularUpperLimit},
	{"nativeSetTranslationalLimitMotorEnableMotor", "(JIZ)I", (void *)nativeSetTranslationalLimitMotorEnableMotor},
	{"nativeSetTranslationalLimitMotorTargetVelocity", "(JIF)I", (void *)nativeSetTranslationalLimitMotorTargetVelocity},
	{"nativeSetTranslationalLimitMotorMaxMotorForce", "(JIF)I", (void *)nativeSetTranslationalLimitMotorMaxMotorForce},
};

int register_Generic6DofConstraint(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/constraint/Generic6DofConstraint",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
