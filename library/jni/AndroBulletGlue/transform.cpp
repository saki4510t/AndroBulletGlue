/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: transform.cpp
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

#include "transform.h"

#include "btVector3.h"
#include "btMatrix3x3.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_TRANS nativeCreate(JNIEnv *env, jobject thiz,
		ID_TRANS id_trans_src) {

#if LOCAL_DEBUG
	LOGV("Transform#nativeCreate:");
#endif

	btTransform *trans;
	btTransform *trans_src = reinterpret_cast<btTransform *>(id_trans_src);
	if (trans_src) {
		trans = new btTransform(*trans_src);
	} else {
		trans = new btTransform();
	}
	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_TRANS>(trans);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_TRANS id_trans) {

#if LOCAL_DEBUG
	LOGD("Transform#nativeDestroy:");
#endif

	btTransform *trans = reinterpret_cast<btTransform *>(id_trans);
	SAFE_DELETE(trans);
}

static ID_VEC nativeGetOrigin(JNIEnv *env, jobject thiz,
		ID_TRANS id_trans) {

#if LOCAL_DEBUG
	LOGV("Transform#nativeGetOrigin:");
#endif

	btTransform *trans = reinterpret_cast<btTransform *>(id_trans);
	if (LIKELY(trans)) {
		return reinterpret_cast<ID_VEC>(&(trans->getOrigin()));
	} else
		return (ID_VEC)NULL;
}

static ID_MAT nativeGetBasis(JNIEnv *env, jobject thiz,
		ID_TRANS id_trans) {

#if LOCAL_DEBUG
	LOGV("Transform#nativeGetBasis:");
#endif

	btTransform *trans = reinterpret_cast<btTransform *>(id_trans);
	if (LIKELY(trans)) {
		return reinterpret_cast<ID_MAT>(&(trans->getBasis()));
	} else
		return (ID_MAT)NULL;
}

static jint nativeMultVec(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_TRANS id_trans,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Transform#nativeMultVec:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	btTransform *trans = reinterpret_cast<btTransform *>(id_trans);
	if (LIKELY(trans && vec)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (vec_result)
			*vec_result = (*trans)(*vec);
		else
			*vec = (*trans)(*vec);
		return JNI_OK;
	} else
		return -1000;
}

static jint nativeMultTrans(JNIEnv *env, jobject thiz,
		ID_TRANS id_trans_result,
		ID_TRANS id_trans1,
		ID_TRANS id_trans2) {

#if LOCAL_DEBUG
	LOGV("Transform#nativeMultTrans:");
#endif

	btTransform *trans1 = reinterpret_cast<btTransform *>(id_trans1);
	btTransform *trans2 = reinterpret_cast<btTransform *>(id_trans2);
	if (LIKELY(trans1 && trans2)) {
		btTransform *trans_result = reinterpret_cast<btTransform *>(id_trans_result);
		if (trans_result && (trans_result != trans1)) {
			*trans_result = *trans1 * *trans2;
		} else {
			*trans1 *= *trans2;
		}
		return JNI_OK;
	} else
		return -1000;
}

static jint nativeMultQuat(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat_result,
		ID_TRANS id_trans,
		ID_QUAT id_quat) {

#if LOCAL_DEBUG
	LOGV("Transform#nativeMultQuat:");
#endif

	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	btTransform *trans = reinterpret_cast<btTransform *>(id_trans);
	if (LIKELY(trans && quat)) {
		btQuaternion *quat_result = reinterpret_cast<btQuaternion *>(id_quat_result);
		if (quat_result && (quat_result != quat)) {
			*quat_result = *trans * *quat;
		} else {
			*quat = *trans * *quat;
		}
		return JNI_OK;
	} else
		return -1000;
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(J)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeGetOrigin", "(J)J", (void *)nativeGetOrigin},
	{"nativeGetBasis", "(J)J", (void *)nativeGetBasis},
	{"nativeMultVec", "(JJJ)I", (void *)nativeMultVec},
	{"nativeMultTrans", "(JJJ)I", (void *)nativeMultTrans},
	{"nativeMultQuat", "(JJJ)I", (void *)nativeMultQuat},
};

int register_Transform(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/math/Transform",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
