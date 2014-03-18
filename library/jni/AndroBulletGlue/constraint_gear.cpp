/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: constraint_gear.cpp
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


#include "constraint_gear.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_CONSTRAINT nativeCreate(JNIEnv *env, jobject thiz,
		ID_RIGIDBODY id_rbA,
		ID_RIGIDBODY id_rbB,
		ID_VEC id_vec_axisInA,
		ID_VEC id_vec_axisInB,
		jfloat ratio) {

#if LOCAL_DEBUG
	LOGV("GearConstraint#createConstraint:");
#endif
	btRigidBody *bodyA = reinterpret_cast<btRigidBody *>(id_rbA);
	if (UNLIKELY(!bodyA)) {
		LOGE("GearConstraint#createConstraint:invalid ID of rbA");
		return (ID_CONSTRAINT)NULL;
	}
	btRigidBody *bodyB = reinterpret_cast<btRigidBody *>(id_rbB);
	if (UNLIKELY(!bodyB)) {
		LOGE("GearConstraint#createConstraint:invalid ID of rbB");
		return (ID_CONSTRAINT)NULL;
	}

	btVector3 *axisInA = reinterpret_cast<btVector3 *>(id_vec_axisInA);
	btVector3 *axisInB = reinterpret_cast<btVector3 *>(id_vec_axisInB);
	btGearConstraint *gear = new btGearConstraint(
		*bodyA, *bodyB, *axisInA, *axisInB, ratio);

	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_CONSTRAINT>(gear);

}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(JJJJF)J", (void *)nativeCreate},
};

int register_GearConstraint(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/constraint/GearConstraint",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
