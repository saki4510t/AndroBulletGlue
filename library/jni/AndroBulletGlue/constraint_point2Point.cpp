/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: constraint_point2Point.cpp
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


#include "constraint_point2Point.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_CONSTRAINT nativeCreate(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_pivotInA,
		ID_VEC id_vec_pivotInB) {

#if LOCAL_DEBUG
	LOGV("Point2PointConstraint#createConstraint:");
#endif
	if (UNLIKELY(isNullField(env, thiz, "mRbA", JTYPE_RIGIDBODY))) {
		LOGE("Point2PointConstraint#createConstraint:rbA not found");
		return (ID_CONSTRAINT)NULL;
	} else {
		jobject rbA_obj = getField_obj(env, thiz, "mRbA", JTYPE_RIGIDBODY);
		btRigidBody *bodyA = (btRigidBody*)get_id(env, rbA_obj);
		btVector3 *pivotInA = reinterpret_cast<btVector3 *>(id_vec_pivotInA);
		if (UNLIKELY(!bodyA || !pivotInA)) {
			LOGE("Point2PointConstraint#createConstraint:invalid ID of rbA or pivotInA is null");
			return (ID_CONSTRAINT)NULL;
		}
		btPoint2PointConstraint *p2p = NULL;
		btVector3 *pivotInB = reinterpret_cast<btVector3 *>(id_vec_pivotInB);
		if (!pivotInB || isNullField(env, thiz, "mRbB", JTYPE_RIGIDBODY)) {
			p2p = new btPoint2PointConstraint(*bodyA, *pivotInA);
		} else {
			jobject rbB_obj = getField_obj(env, thiz, "mRbB", JTYPE_RIGIDBODY);
			btRigidBody *bodyB = (btRigidBody*)get_id(env, rbB_obj);
			if (UNLIKELY(!bodyB)) {
				LOGE("Point2PointConstraint#createConstraint:invalid ID of rbB");
				return (ID_CONSTRAINT)NULL;
			}
			p2p = new btPoint2PointConstraint(*bodyA, *bodyB, *pivotInA, *pivotInB);
		}

		// return pointer value of object as a ID to Java instance
		return reinterpret_cast<ID_CONSTRAINT>(p2p);
	}
}

static jint nativeSetBreakingImpulseThreshold(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jfloat threshold) {

#if LOCAL_DEBUG
	LOGV("Point2PointConstraint_setBreakingImpulseThreshold:");
#endif
	btPoint2PointConstraint *constraint = reinterpret_cast<btPoint2PointConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setBreakingImpulseThreshold(threshold);
		return JNI_OK;
	} else {
		LOGE("Point2PointConstraint#setBreakingImpulseThreshold:invalid id");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(JJ)J", (void *)nativeCreate},
	{"nativeSetBreakingImpulseThreshold", "(JF)I", (void *)nativeSetBreakingImpulseThreshold},
};

int register_Point2PointConstraint(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/constraint/Point2PointConstraint",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
