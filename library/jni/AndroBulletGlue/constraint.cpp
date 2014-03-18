/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: constraint.cpp
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


#include "constraint.h"
#include "variables.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_CONSTRAINT nativeCreate(JNIEnv *env, jobject thiz,
		ID_TRANS id_transformA,
		ID_TRANS id_transformB,
		jboolean useReferenceFrameA) {

#if LOCAL_DEBUG
	LOGV("Constraint#nativeCreate:");
#endif
	int constraintType = getField_int(env, thiz, "mType");

	if (UNLIKELY(isNullField(env, thiz, "mRbA", JTYPE_RIGIDBODY))) {
		LOGE("Constraint#nativeCreate:rbA not found");
		return (ID_CONSTRAINT)NULL;
	} else {
		jobject rbA_obj = getField_obj(env, thiz, "mRbA", JTYPE_RIGIDBODY);
		btRigidBody *bodyA = (btRigidBody*)get_id(env, rbA_obj);
		if (UNLIKELY(!bodyA)) {
			LOGE("Constraint#nativeCreate:invalid ID of rbA");
			return (ID_CONSTRAINT)NULL;
		}
		btTransform *transformA = reinterpret_cast<btTransform *>(id_transformA);
		btTypedConstraint *constraint;
		if (isNullField(env, thiz, "mRbB", JTYPE_RIGIDBODY)) {
			switch (constraintType) {
			case HINGE_CONSTRAINT_TYPE:
				constraint = new btHingeConstraint(*bodyA, *transformA, useReferenceFrameA);
				break;
			case CONETWIST_CONSTRAINT_TYPE:
				constraint = new btConeTwistConstraint(*bodyA, *transformA);
				break;
			case SLIDER_CONSTRAINT_TYPE:
				constraint = new btSliderConstraint(*bodyA, *transformA, useReferenceFrameA);
				break;
			case D6_CONSTRAINT_TYPE:
				// mRbBが無い時はUniversalConstraintのチェックは無し
				constraint = new btGeneric6DofConstraint(*bodyA, *transformA, useReferenceFrameA);
				break;
			case D6_SPRING_CONSTRAINT_TYPE:
				// mRbBが無い時はHinge2Constraintのチェックは無し
				constraint = new btGeneric6DofSpringConstraint(*bodyA, *transformA, useReferenceFrameA);
				break;
			default:
				LOGE("Constraint#nativeCreate:constraint type %d not supported", constraintType);
				return (ID_CONSTRAINT)NULL;
			}
		} else {
			jobject rbB_obj = getField_obj(env, thiz, "mRbB", JTYPE_RIGIDBODY);
			btRigidBody *bodyB = (btRigidBody*)get_id(env, rbB_obj);
			if (UNLIKELY(!bodyB)) {
				LOGE("Constraint#nativeCreate:invalid ID of rbB");
				return (ID_CONSTRAINT)NULL;
			}
			btTransform *transformB = reinterpret_cast<btTransform *>(id_transformB);
			switch (constraintType) {
			case HINGE_CONSTRAINT_TYPE:
				constraint = new btHingeConstraint(*bodyA, *bodyB, *transformA, *transformB, useReferenceFrameA);
				break;
			case CONETWIST_CONSTRAINT_TYPE:
				constraint = new btConeTwistConstraint(*bodyA, *bodyB, *transformA, *transformB);
				break;
			case SLIDER_CONSTRAINT_TYPE:
				constraint = new btSliderConstraint(*bodyA, *bodyB, *transformA, *transformB, useReferenceFrameA);
				break;
			case D6_CONSTRAINT_TYPE:
				// UniversalConstraintのチェック
				// mAnchor/mAxis1/mAxis2フィールドがあればUniversalConstraint
				if (!isNullField(env, thiz, "mAnchor", JTYPE_VECTOR3)
					&& !isNullField(env, thiz, "mAxis1", JTYPE_VECTOR3)
					&& !isNullField(env, thiz, "mAxis2", JTYPE_VECTOR3)) {

#if LOCAL_DEBUG
					LOGV("Constraint#nativeCreate:btUniversalConstraint");
#endif
					btVector3 *anchor = getField_vec(env, thiz, "mAnchor");
					btVector3 *axis1 = getField_vec(env, thiz, "mAxis1");
					btVector3 *axis2 = getField_vec(env, thiz, "mAxis2");
					constraint = new btUniversalConstraint(*bodyA, *bodyB, *anchor, *axis1, *axis2);
				} else {
#if LOCAL_DEBUG
					LOGV("Constraint#nativeCreate:btGeneric6DofConstraint");
#endif
					constraint = new btGeneric6DofConstraint(*bodyA, *bodyB, *transformA, *transformB/*localFrameA, localFrameB*/, useReferenceFrameA);
				}
				break;
			case D6_SPRING_CONSTRAINT_TYPE:
				// Hinge2Constraintのチェック
				// mAnchorフィールドがあればHinge2Constraint
				if (!isNullField(env, thiz, "mAnchor", JTYPE_VECTOR3)
					&& !isNullField(env, thiz, "mAxis1", JTYPE_VECTOR3)
					&& !isNullField(env, thiz, "mAxis2", JTYPE_VECTOR3)) {

#if LOCAL_DEBUG
					LOGV("Constraint#nativeCreate:btHinge2Constraint");
#endif
					btVector3 *anchor = getField_vec(env, thiz, "mAnchor");
					btVector3 *axis1 = getField_vec(env, thiz, "mAxis1");
					btVector3 *axis2 = getField_vec(env, thiz, "mAxis2");
					constraint = new btHinge2Constraint(*bodyA, *bodyB, *anchor, *axis1, *axis2);
				} else {
#if LOCAL_DEBUG
					LOGV("Constraint#nativeCreate:btGeneric6DofSpringConstraint");
#endif
					constraint = new btGeneric6DofSpringConstraint(*bodyA, *bodyB, *transformA, *transformB/*localFrameA, localFrameB*/, useReferenceFrameA);
				}
				break;
			default:
				LOGE("Constraint#nativeCreate:constraint type %d not supported", constraintType);
				return (ID_CONSTRAINT)NULL;
			}
		}

		// return pointer value of object as a ID to Java instance
		return reinterpret_cast<ID_CONSTRAINT>(constraint);
	}
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint) {

#if LOCAL_DEBUG
	LOGD("Constraint#nativeDestroy:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(
			getField_nativePtr(env, thiz, "mPhysicsWorldID"));
	if (UNLIKELY(!gPhysicsWorldCtrlBlocks.get((PhysicsWorldCtrlBlock*)ctrlBlock))) {
		LOGE("Constraint#removeConstraint:bullet control block not found");
	}
	btTypedConstraint *constraint = reinterpret_cast<btTypedConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		ctrlBlock->dynamicsWorld->removeConstraint(constraint);
		SAFE_DELETE(constraint);
	}
}

static jint nativeAddConstraint(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		ID_CTRLBLOCK id_ctrlblock,
		jboolean disableCollisionsBetweenLinkedBodies) {

	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock || !ctrlBlock->dynamicsWorld)) {
		LOGE("addConstraint:bullet control block not found");
		return -1000;
	}
	btTypedConstraint *constraint = reinterpret_cast<btTypedConstraint *>(id_constraint);
	if (UNLIKELY(!constraint)) {
		LOGE("addConstraint:constraint is null");
		return -1000;
	}
	ctrlBlock->dynamicsWorld->addConstraint(constraint, disableCollisionsBetweenLinkedBodies);
	return JNI_OK;
}

static jint nativeSetDbgDrawSize(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jfloat dbgDrawSize) {

#if LOCAL_DEBUG
	LOGV("Constraint#setDbgDrawSize");
#endif
	btTypedConstraint *constraint = reinterpret_cast<btTypedConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setDbgDrawSize(dbgDrawSize);
		return JNI_OK;
	} else {
		LOGE("setDbgDrawSize:invalid ID of constraint");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(JJZ)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeSetDbgDrawSize", "(JF)I", (void *)nativeSetDbgDrawSize},
	{"nativeAddConstraint", "(JJZ)I", (void *)nativeAddConstraint},
};

int register_Constraint(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/interfaces/Constraint",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
