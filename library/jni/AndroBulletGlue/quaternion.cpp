/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: quaternion.cpp
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

#include "quaternion.h"

#include "btVector3.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_QUAT nativeCreate(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat_src) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeCreate:");
#endif

	btQuaternion *quat;
	btQuaternion *quat_src = reinterpret_cast<btQuaternion *>(id_quat_src);
	if (quat_src) {
		quat = new btQuaternion(*quat_src);
	} else {
		quat = new btQuaternion();
	}
	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_TYPE>(quat);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat) {

#if LOCAL_DEBUG
	LOGD("Quaternion#nativeDestroy:");
#endif

	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	SAFE_DELETE(quat);
}

static jint nativeSet4(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		jfloat x, jfloat y, jfloat z, jfloat w) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSet4:");
#endif

	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	if (LIKELY(quat)) {
		quat->setValue(x, y, z, w);
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSet4:invalid id of Quaternion object");
		return -1000;
	}
}

static int nativeSetQuat(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		ID_QUAT id_quat_src) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSetQuat:");
#endif

	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	btQuaternion *quat_src = reinterpret_cast<btQuaternion *>(id_quat_src);
	if (LIKELY(quat && quat_src)) {
		*quat = *quat_src;
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSetQuat:invalid id of Quaternion object");
		return -1000;
	}
}

static jint nativeSetRotation(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		ID_VEC id_vec_axis,
		jfloat angle) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSetRotation:");
#endif

	btVector3 *vec_axis = reinterpret_cast<btVector3 *>(id_vec_axis);
	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	if (LIKELY(quat && vec_axis)) {
		quat->setRotation(*vec_axis, angle);
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSetRotation:invalid id of Quaternion object");
		return -1000;
	}
}

static jint nativeSetEuler(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		jfloat yaw, jfloat pitch, jfloat roll) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSetEuler:");
#endif

	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	if (LIKELY(quat)) {
		quat->setEuler(yaw, pitch, roll);
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSetEuler:invalid id of Quaternion object");
		return -1000;
	}
}

static jint nativeSetEulerVec(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSetEuler:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	if (LIKELY(quat && vec)) {
		quat->setEulerZYX(vec->y(), vec->x(), vec->z());
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSetEuler:invalid id of Quaternion/Vector3");
		return -1000;
	}
}

static jint nativeSetEulerZYX(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		jfloat yaw, jfloat pitch, jfloat roll) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSetEulerZYX:");
#endif

	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	if (LIKELY(quat)) {
		quat->setEulerZYX(yaw, pitch, roll);
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSetEulerZYX:invalid id of Quaternion object");
		return -1000;
	}
}

static jint nativeSetEulerZYXVec(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSetEulerZYXVec:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	if (LIKELY(quat && vec)) {
		quat->setEulerZYX(vec->z(), vec->y(), vec->x());
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSetEulerZYXVec:invalid id of Quaternion/Vector3");
		return -1000;
	}
}

static int nativeSetA(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		jint ix,
		jfloat v) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSetA:");
#endif

	btVector3 *quat = reinterpret_cast<btVector3 *>(id_quat);
	if (LIKELY(quat && ix >= 0 && ix < 4)) {
		(*quat)[ix] = v;
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSetA:invalid id of Quaternion object");
		return -1000;
	}
}

static jfloat nativeGetA(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		jint ix) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeGetA:");
#endif

	btVector3 *quat = reinterpret_cast<btVector3 *>(id_quat);
	if (LIKELY(quat && ix >= 0 && ix < 4)) {
		return (*quat)[ix];
	} else {
		LOGE("Quaternion#nativeGetA:invalid id of Quaternion object");
		return 0.f;
	}
}

static jint nativeGetArray(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		jfloatArray array) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeGetArray:");
#endif

	btVector3 *quat = reinterpret_cast<btVector3 *>(id_quat);
	if (LIKELY(quat && array)) {
		jfloat *tmp = env->GetFloatArrayElements(array, NULL);
		memcpy(tmp, &(*quat)[0], sizeof(btScalar) * 4);
		// release reference of array with writeing back its elements
		env->ReleaseFloatArrayElements(array, tmp, ARRAYELEMENTS_COPYBACK_AND_RELEASE);
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeGetArray:invalid id of Quaternion object");
		return -1000;
	}
}

static jint nativeSetArray(JNIEnv *env, jobject thiz,
		ID_QUAT id_quat,
		jfloatArray array) {

#if LOCAL_DEBUG
	LOGV("Quaternion#nativeSetArray:");
#endif

	btVector3 *quat = reinterpret_cast<btVector3 *>(id_quat);
	if (LIKELY(quat && array)) {
		jfloat *tmp = env->GetFloatArrayElements(array, NULL);
		memcpy(&(*quat)[0], tmp, sizeof(btScalar) * 4);
		// release reference of array without writing back its elements
		env->ReleaseFloatArrayElements(array, tmp, ARRAYELEMENTS_ABORT_AND_RELEASE);
		return JNI_OK;
	} else {
		LOGE("Quaternion#nativeSetArray:invalid id of Quaternion object");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(J)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeSet4", "(JFFFF)I", (void *)nativeSet4},
	{"nativeSetQuat", "(JJ)I", (void *)nativeSetQuat},
	{"nativeSetRotation", "(JJF)I", (void *)nativeSetRotation},
	{"nativeSetEuler", "(JFFF)I", (void *)nativeSetEuler},
	{"nativeSetEulerVec", "(JJ)I", (void *)nativeSetEulerVec},
	{"nativeSetEulerZYX", "(JFFF)I", (void *)nativeSetEulerZYX},
	{"nativeSetEulerZYXVec", "(JJ)I", (void *)nativeSetEulerZYXVec},
	{"nativeSetA", "(JIF)I", (void *)nativeSetA},
	{"nativeGetA", "(JI)F", (void *)nativeGetA},
	{"nativeGetArray", "(J[F)I", (void *)nativeGetArray},
	{"nativeSetArray", "(J[F)I", (void *)nativeSetArray},
};

int register_Quaternion(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/math/Quaternion",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
