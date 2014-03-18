/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: constraint_hinge.cpp
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

#include "constraint_hinge.h"
#include "physicsworld.h"
#include "variables.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_CONSTRAINT nativeCreate(
		JNIEnv *env, jobject thiz,
		ID_CTRLBLOCK id_ctrlblock,
		ID_VEC id_vec_pivotInA,
		ID_VEC id_vec_pivotInB,
		ID_VEC id_vec_axisInA,
		ID_VEC id_vec_axisInB) {

#if LOCAL_DEBUG
	LOGV("HingeConstraint#nativeCreate:");
#endif
	btVector3 *pivotInA = reinterpret_cast<btVector3 *>(id_vec_pivotInA);
	btVector3 *axisInA = reinterpret_cast<btVector3 *>(id_vec_axisInA);
	if (UNLIKELY(isNullField(env, thiz, "mRbA", JTYPE_RIGIDBODY) || !pivotInA || !axisInA)) {
		LOGE("HingeConstraint#nativeCreate:rbA not found / pivotInA or axisInA is null");
		return (ID_CONSTRAINT)NULL;
	} else {
		jobject rbA_obj = getField_obj(env, thiz, "mRbA", JTYPE_RIGIDBODY);
		PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
		if (UNLIKELY(!gPhysicsWorldCtrlBlocks.get(ctrlBlock))) {
			LOGE("HingeConstraint#nativeCreate:invalid ID of physicsWorld");
			return (ID_CONSTRAINT)NULL;
		}
		btRigidBody *bodyA = (btRigidBody*)get_id(env, rbA_obj);
		if (UNLIKELY(!bodyA)) {
			LOGE("HingeConstraint#nativeCreate:invalid ID of rbA");
			return (ID_CONSTRAINT)NULL;
		}
		btHingeConstraint *hinge;
		bool useReferenceFrameA = getField_bool(env, thiz, "mUseReferenceFrameA");
		if (isNullField(env, thiz, "mRbB", JTYPE_RIGIDBODY)) {
#if LOCAL_DEBUG
			LOGV("HingeConstraint#nativeCreate:create1");
#endif
			hinge = new btHingeConstraint(*bodyA, *pivotInA, *axisInA, useReferenceFrameA);
		} else {
			jobject rbB_obj = getField_obj(env, thiz, "mRbB", JTYPE_RIGIDBODY);
			btRigidBody *bodyB = (btRigidBody*)get_id(env, rbB_obj);
			btVector3 *pivotInB = reinterpret_cast<btVector3 *>(id_vec_pivotInB);
			btVector3 *axisInB = reinterpret_cast<btVector3 *>(id_vec_axisInB);
			if (UNLIKELY(!bodyB || !pivotInB || !axisInB)) {
				LOGE("HingeConstraint#nativeCreate:invalid ID of rbB / pivotInB or axisInB is null");
				return (ID_CONSTRAINT)NULL;
			}
#if LOCAL_DEBUG
			LOGV("HingeConstraint#nativeCreate:create2");
#endif
			hinge = new btHingeConstraint(*bodyA, *bodyB, *pivotInA, *pivotInB, *axisInA, *axisInB, useReferenceFrameA);
		}

		if (LIKELY(hinge)) {
#if LOCAL_DEBUG
			LOGV("HingeConstraint#nativeCreate:created:set");
#endif
			btTransform *ta = getField_trans(env, thiz, "mFrameA");
			*ta = hinge->getAFrame();
			btTransform *tb = getField_trans(env, thiz, "mFrameB");
			*tb = hinge->getBFrame();
		}
		// return pointer value of object as a ID to Java instance
		return reinterpret_cast<ID_CONSTRAINT>(hinge);
	}
}

static jint nativeSetLimit(
		JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jfloat low,
		jfloat high) {

#if LOCAL_DEBUG
	LOGV("HingeConstraint#setLimit:");
#endif
	btHingeConstraint *constraint = reinterpret_cast<btHingeConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setLimit(low, high);
		return JNI_OK;
	} else {
		LOGE("HingeConstraint#setLimit:invalid ID of HingeConstraint");
		return -1000;
	}
}

static jint nativeEnableAngularMotor(
		JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jboolean enable,
		jfloat targetVelocity,
		jfloat maxMotorImpulse) {

#if LOCAL_DEBUG
	LOGV("HingeConstraint#enableAngularMotor:");
#endif
	btHingeConstraint *constraint = reinterpret_cast<btHingeConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->enableAngularMotor(enable, targetVelocity, maxMotorImpulse);
		return JNI_OK;
	} else {
		LOGE("HingeConstraint#enableAngularMotor:invalid ID of HingeConstraint");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(JJJJJ)J", (void *)nativeCreate},
	{"nativeSetLimit", "(JFF)I", (void *)nativeSetLimit},
	{"nativeEnableAngularMotor", "(JZFF)I", (void *)nativeEnableAngularMotor},
};

int register_HingeConstraint(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/constraint/HingeConstraint",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
