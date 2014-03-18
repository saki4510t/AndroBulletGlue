/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: rigidBody.cpp
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


#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "utils.h"
#include "variables.h"
#include "rigidBody.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_RIGIDBODY nativeCreate(JNIEnv *env, jobject thiz,
		ID_CTRLBLOCK id_ctrlblock,
		ID_SHAPE id_shape,
		jfloat mass,
		jobject motionState_obj,
		jfloat contactProcessingThreshold) {

	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!gPhysicsWorldCtrlBlocks.get(ctrlBlock))) {
		LOGE("nativeCreate:bullet control block not found");
		return (ID_RIGIDBODY)NULL;
	}

	btCollisionShape *colShape = reinterpret_cast<btCollisionShape *>(id_shape);
	if (UNLIKELY(!colShape)) {
		LOGE("nativeCreate:invalid id of CollisionShape");
		return (ID_RIGIDBODY)NULL;
	}

	if (UNLIKELY(!motionState_obj)) {
		LOGE("nativeCreate:motionState is NULL.");
		return (ID_RIGIDBODY)NULL;
	}

	btTransform *startTransform = getField_trans(env, motionState_obj, "worldTransform");
	btDefaultMotionState *motionState = new btDefaultMotionState(*startTransform);

	btVector3 localInertia = btVector3(btScalar(0.), btScalar(0.), btScalar(0.));
	if (mass != 0.f) {	// mass 0 means static object
		// calculate inertia from shape and mass
		colShape->calculateLocalInertia(mass, localInertia);
#if LOCAL_DEBUG
		LOGV("nativeCreate:calculateLocalInertia");
#endif
	}

	btRigidBody::btRigidBodyConstructionInfo rbInfo(
		mass, motionState, colShape, localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	body->setContactProcessingThreshold(contactProcessingThreshold);
	// add
	ctrlBlock->dynamicsWorld->addRigidBody(body);
#if LOCAL_DEBUG
	LOGV("nativeCreate:create btRigidBody");
#endif
	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_RIGIDBODY>(body);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body) {

#if LOCAL_DEBUG
	LOGD("RigidBody#nativeDestroy:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(
			getField_nativePtr(env, thiz, "mPhysicsWorldID"));
	if (LIKELY(gPhysicsWorldCtrlBlocks.get(ctrlBlock))) {
		btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
		if (ctrlBlock && body) {
			// delete btMotionState
			if (body->getMotionState()) {
				delete body->getMotionState();
			}
			// remove and delete btRigidBody
			ctrlBlock->dynamicsWorld->removeRigidBody(body);
			SAFE_DELETE(body);
		}
	}
}

static jint nativeApplyForce(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_VEC id_vec_force,
		ID_VEC id_vec_applyPoint) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeApplyForce:");
#endif
	btVector3 *force = reinterpret_cast<btVector3 *>(id_vec_force);
	btVector3 *applyPoint = reinterpret_cast<btVector3 *>(id_vec_applyPoint);
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body && force && applyPoint)) {
		body->applyForce(*force, *applyPoint);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeApplyForce:invalid id of RigidBody/Vector3");
		return -1000;
	}
}


static jint nativeApplyTorque(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_VEC id_vec_torque) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeApplyTorque:");
#endif
	btVector3 *torque = reinterpret_cast<btVector3 *>(id_vec_torque);
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body && torque)) {
		body->applyTorque(*torque);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeApplyTorque:invalid id of RigidBody/Vector3");
		return -1000;
	}
}

static jint nativeApplyCentralImpulse(
		JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_VEC id_vec_impulse) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeApplyCentralImpulse:");
#endif
	btVector3 *impulse = reinterpret_cast<btVector3 *>(id_vec_impulse);
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body && impulse)) {
		body->applyCentralImpulse(*impulse);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeApplyCentralImpulse:invalid id of RigidBody/Vector3");
		return -1000;
	}
}

static jint nativeApplyTorqueImpulse(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_VEC id_vec_torque) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeApplyTorqueImpulse:");
#endif
	btVector3 *torque = reinterpret_cast<btVector3 *>(id_vec_torque);
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body && torque)) {
		body->applyTorqueImpulse(*torque);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeApplyTorqueImpulse:invalid id of RigidBody/Vector3");
		return -1000;
	}
}

static jint nativeApplyImpulse(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_VEC id_vec_impulse,
		ID_VEC id_vec_applyPoint) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeApplyTorqueImpulse:");
#endif
	btVector3 *impulse = reinterpret_cast<btVector3 *>(id_vec_impulse);
	btVector3 *applyPoint = reinterpret_cast<btVector3 *>(id_vec_applyPoint);
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body && impulse && applyPoint)) {
		body->applyImpulse(*impulse, *applyPoint);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeApplyTorqueImpulse:invalid id of RigidBody/Vector3");
		return -1000;
	}
}

static jint nativeClearForces(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeClearForces:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->clearForces();
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeClearForces:invalid id of RigidBody/Vector3");
		return -1000;
	}
}

static jint nativeSetActive(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		jboolean isActive) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetActive:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->activate(isActive);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetActive:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeGetActivationState(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeGetActivationState:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		return body->getActivationState();
	} else {
		LOGE("RigidBody#nativeGetActivationState:invalid id of RigidBody");
		return 0;
	}
}

static jint nativeSetActivationState(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		jint activationState) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetActivationState:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->setActivationState(activationState);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetActivationState:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeSetDamping(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		jfloat lin_damping,
		jfloat ang_damping) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetDamping:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->setDamping(lin_damping, ang_damping);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetDamping:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeSetDeactivationTime(JNIEnv* env, jobject thiz,
		ID_RIGIDBODY id_body,
		jfloat time) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetDeactivationTime:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->setDeactivationTime(time);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetDeactivationTime:invalid id of RigidBody");
		return -1000;
	}
}

static jfloat nativeGetDeactivationTime(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeGetDeactivationTime:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		return body->getDeactivationTime();
	} else {
		LOGE("RigidBody#nativeGetDeactivationTime:invalid id of RigidBody");
		return 0.f;
	}
}

static jint nativeSetSleepingThresholds(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		jfloat linear,
		jfloat angular) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetSleepingThresholds:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->setSleepingThresholds(linear, angular);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetSleepingThresholds:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeSetLinearFactor(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		jfloat factor_x, jfloat factor_y, jfloat factor_z) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetLinearFactor:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->setLinearFactor(btVector3(factor_x, factor_y, factor_z));
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetLinearFactor:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeSetLinearFactorVec(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_VEC id_vec_factor) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetLinearFactorVec:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		btVector3 *factor = reinterpret_cast<btVector3 *>(id_vec_factor);
		body->setLinearFactor(*factor);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetLinearFactorVec:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeSetAngularVelocity(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		jfloat velocity_x, jfloat velocity_y, jfloat velocity_z) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetAngularVelocity:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->setAngularVelocity(btVector3(velocity_x, velocity_y, velocity_z));
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetAngularVelocity:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeSetAngularVelocityVec(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_VEC id_vec_velocity) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetAngularVelocityVec:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		btVector3 *velocity = reinterpret_cast<btVector3 *>(id_vec_velocity);
		body->setAngularVelocity(*velocity);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetAngularVelocityVec:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeSetAngularFactor(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		jfloat factor_x, jfloat factor_y, jfloat factor_z) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetAngularFactor:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->setAngularFactor(btVector3(factor_x, factor_y, factor_z));
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetAngularFactor:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeSetAngularFactorVec(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_VEC id_vec_factor) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetAngularFactorVec:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		btVector3 *factor = reinterpret_cast<btVector3 *>(id_vec_factor);
		body->setAngularFactor(*factor);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetAngularFactorVec:invalid id of RigidBody");
		return -1000;
	}
}

static jint nativeGetCenterOfMassTransform(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_TRANS id_trans) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeGetCenterOfMassTransform:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	btTransform *trans = reinterpret_cast<btTransform *>(id_trans);
	if (LIKELY(body && trans)) {
		*trans = body->getCenterOfMassTransform();
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeGetCenterOfMassTransform:invalid id of RigidBody/Transform");
		return -1000;
	}
}

static jint nativeSetCenterOfMassTransform(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		ID_TRANS id_trans) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetCenterOfMassTransform:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	btTransform *trans = reinterpret_cast<btTransform *>(id_trans);
	if (LIKELY(body && trans)) {
		body->setCenterOfMassTransform(*trans);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeGetCenterOfMassTransform:invalid id of RigidBody/Transform");
		return -1000;
	}
}

static jint nativeSetFriction(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_body,
		jfloat friction) {

#if LOCAL_DEBUG
	LOGV("RigidBody#nativeSetFriction:");
#endif
	btRigidBody *body = reinterpret_cast<btRigidBody *>(id_body);
	if (LIKELY(body)) {
		body->setFriction(friction);
		return JNI_OK;
	} else {
		LOGE("RigidBody#nativeSetFriction:invalid id of RigidBody");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(JJFLcom/serenegiant/androbulletglue/MotionState;F)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeApplyForce", "(JJJ)I", (void *)nativeApplyForce},
	{"nativeApplyTorque", "(JJ)I", (void *)nativeApplyTorque},
	{"nativeApplyCentralImpulse", "(JJ)I", (void *)nativeApplyCentralImpulse},
	{"nativeApplyTorqueImpulse", "(JJ)I", (void *)nativeApplyTorqueImpulse},
	{"nativeApplyImpulse", "(JJJ)I", (void *)nativeApplyImpulse},
	{"nativeClearForces", "(J)I", (void *)nativeClearForces},
	{"nativeSetActive", "(JZ)I", (void *)nativeSetActive},
	{"nativeGetActivationState", "(J)I", (void *)nativeGetActivationState},
	{"nativeSetActivationState", "(JI)I", (void *)nativeSetActivationState},
	{"nativeSetDamping", "(JFF)I", (void *)nativeSetDamping},
	{"nativeSetDeactivationTime", "(JF)I", (void *)nativeSetDeactivationTime},
	{"nativeGetDeactivationTime", "(J)F", (void *)nativeGetDeactivationTime},
	{"nativeSetSleepingThresholds", "(JFF)I", (void *)nativeSetSleepingThresholds},
	{"nativeSetLinearFactor", "(JFFF)I", (void *)nativeSetLinearFactor},
	{"nativeSetLinearFactorVec", "(JJ)I", (void *)nativeSetLinearFactorVec},
	{"nativeSetAngularVelocity", "(JFFF)I", (void *)nativeSetAngularVelocity},
	{"nativeSetAngularVelocityVec", "(JJ)I", (void *)nativeSetAngularVelocityVec},
	{"nativeSetAngularFactor", "(JFFF)I", (void *)nativeSetAngularFactor},
	{"nativeSetAngularFactorVec", "(JJ)I", (void *)nativeSetAngularFactorVec},
	{"nativeGetCenterOfMassTransform", "(JJ)I", (void *)nativeGetCenterOfMassTransform},
	{"nativeSetCenterOfMassTransform", "(JJ)I", (void *)nativeSetCenterOfMassTransform},
	{"nativeSetFriction", "(JF)I", (void *)nativeSetFriction},
};

int register_RigidBody(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/RigidBody",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
