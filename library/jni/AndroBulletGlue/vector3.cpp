/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: vector3.cpp
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

#include "vector3.h"

#include "btVector3.h"
#include "btMatrix3x3.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_VEC nativeCreate(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_src) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeCreate:");
#endif

	btVector3 *vec;
	btVector3 *vec_src = reinterpret_cast<btVector3 *>(id_vec_src);
	if (vec_src) {
		vec = new btVector3(*vec_src[0], *vec_src[1], *vec_src[2]);
	} else {
		vec = new btVector3();
	}
	return reinterpret_cast<ID_VEC>(vec);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGD("Vector3#nativeDestroy:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	SAFE_DELETE(vec);
}

static jint nativeSet3(JNIEnv *env, jobject thiz,
		ID_VEC id_vec,
		jfloat x, jfloat y, jfloat z) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeSet3:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		vec->setValue(x, y, z);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeSet3:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeSet4(JNIEnv *env, jobject thiz,
		ID_VEC id_vec,
		jfloat x, jfloat y, jfloat z, jfloat w) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeSet4:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		vec->setValue(x, y, z);
		vec->setW(w);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeSet4:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeSetVec(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_dest,
		ID_VEC id_vec_src) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeSetVec:");
#endif

	btVector3 *dest = reinterpret_cast<btVector3 *>(id_vec_dest);
	btVector3 *src = reinterpret_cast<btVector3 *>(id_vec_src);
	if (LIKELY(dest && src)) {
		*dest = *src;
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeSetVec:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeSetA(JNIEnv *env, jobject thiz,
		ID_VEC id_vec,
		jint axis, jfloat v) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeSetA:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec && (axis >= 0) && (axis < 4))) {
		(*vec)[axis] = v;
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeSetA:invalid id of Vector3 object");
		return -1000;
	}
}

static jfloat nativeGetA(JNIEnv *env, jobject thiz,
		ID_VEC id_vec,
		jint axis) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeGetA:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec && (axis >= 0) && (axis < 4))) {
		return (*vec)[axis];
	} else {
		LOGE("Vector3#nativeGetA:invalid id of Vector3 object");
		return 0.f;
	}
}

static jint nativeGetArray(JNIEnv *env, jobject thiz,
		ID_VEC id_vec,
		jfloatArray array) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeGetArray:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec && array)) {
		jfloat *tmp = env->GetFloatArrayElements(array, NULL);
		memcpy(tmp, vec->m_floats, sizeof(btScalar) * 4);
		// write back and release reference
		env->ReleaseFloatArrayElements(array, tmp, ARRAYELEMENTS_COPYBACK_AND_RELEASE);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeGetArray:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeSetArray(JNIEnv *env, jobject thiz,
		ID_VEC id_vec,
		jfloatArray array) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeSetArray:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec && array)) {
		jfloat *tmp = env->GetFloatArrayElements(array, NULL);
		memcpy(vec->m_floats, tmp, sizeof(btScalar) * 4);
		// only release reference
		env->ReleaseFloatArrayElements(array, tmp, ARRAYELEMENTS_ABORT_AND_RELEASE);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeSetArray:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeSetMax(JNIEnv *env, jobject thiz,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeSetMax:");
#endif

	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec1 && vec2)) {
		vec1->setMax(*vec2);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeSetMax:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeSetMin(JNIEnv *env, jobject thiz,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeSetMin:");
#endif

	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec1 && vec2)) {
		vec1->setMin(*vec2);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeSetMin:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeRotate(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec,
		ID_VEC id_vec_axis, jfloat angle) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeRotate:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	btVector3 *vec_axis = reinterpret_cast<btVector3 *>(id_vec_axis);
	if (LIKELY(vec && vec_axis)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (vec_result)
			*vec_result = vec->rotate(*vec_axis, angle);
		else
			*vec = vec->rotate(*vec_axis, angle);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeRotate:invalid id of Vector3 object");
		return -1000;
	}
}

static jfloat nativeLength2(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeLength2:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return vec->length2();
	} else {
		LOGE("Vector3#nativeLength2:invalid id of Vector3 object");
		return 0.f;
	}
}

static jfloat nativeLength(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeLength:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return vec->length();
	} else {
		LOGE("Vector3#nativeLength:invalid id of Vector3 object");
		return 0.f;
	}
}

static jfloat nativeDistance2(JNIEnv *env, jobject thiz,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeDistance2:");
#endif

	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec1 && vec2)) {
		return vec1->distance2(*vec2);
	} else {
		LOGE("Vector3#nativeDistance2:invalid id of Vector3 object");
		return 0.f;
	}
}

static jfloat nativeDistance(JNIEnv *env, jobject thiz,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeDistance:");
#endif

	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec1 && vec2)) {
		return vec1->distance(*vec2);
	} else {
		LOGE("Vector3#nativeDistance:invalid id of Vector3 object");
		return 0.f;
	}
}

static jint nativeLerp(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec1,
		ID_VEC id_vec2,
		jfloat t) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeLerp:");
#endif

	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec1 && vec2)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (vec_result)
			*vec_result = vec1->lerp(*vec2, t);
		else
			*vec1 = vec1->lerp(*vec2, t);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeLerp:invalid id of Vector3 object");
		return -1000;
	}
}

static jfloat nativeDot(JNIEnv *env, jobject thiz,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeDot:");
#endif

	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec1 && vec2)) {
		return vec1->dot(*vec2);
	} else {
		LOGE("Vector3#nativeDot:invalid id of Vector3 object");
		return 0.f;
	}
 }

static jint nativeCross(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeCross:");
#endif

	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec1 && vec2)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (vec_result)
			*vec_result = vec1->cross(*vec2);
		else
			*vec1 = vec1->cross(*vec2);;
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeCross:invalid id of Vector3 object");
		return -1000;
	}
}

static jfloat nativeTriple(JNIEnv *env, jobject thiz,
		ID_VEC id_vec,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeTriple:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec && vec1 && vec2)) {
		return vec->triple(*vec1, *vec2);
	} else {
		LOGE("Vector3#nativeTriple:invalid id of Vector3 object");
		return 0.f;
	}
}

static jint nativeAddVec(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec,
		ID_VEC id_vec_add,
		jfloat factor) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeAddVec:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	btVector3 *vec_add = reinterpret_cast<btVector3 *>(id_vec_add);
	if (LIKELY(vec && vec_add)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (!vec_result || (vec_result == vec)) {
			(*vec) += (*vec_add);
		} else {
			(*vec_result) = (*vec) + (*vec_add);
		}
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeAddVec:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeMultScalar(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec,
		jfloat v) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeMultScalar:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (!vec_result || (vec_result == vec)) {
			*vec *= v;
		} else {
			*vec_result = *vec * v;
		}
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeMultScalar:invalid id of Vector3 object");
		return -1000;
	}
}

jint nativeDivScalar(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec,
		jfloat v) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeDivScalar:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (!vec_result || (vec_result == vec)) {
			*vec /= v;
		} else {
			*vec_result = *vec / v;
		}
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeDivScalar:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeNormalize(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeNormalize:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		vec->normalize();
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeNormalize:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeSafeNormalize(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeSafeNormalize:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		vec->safeNormalize();
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeSafeNormalize:invalid id of Vector3 object");
		return -1000;
	}
}

static jfloat nativeAngle(JNIEnv *env, jobject thiz,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeAngle:");
#endif

	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec1 && vec2)) {
		return vec1->angle(*vec2);
	} else {
		LOGE("Vector3#nativeAngle:invalid id of Vector3 object");
		return 0.f;
	}
}

static jint nativeAbsolute(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeAbsolute:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (vec_result)
			*vec_result = vec->absolute();
		else
			*vec = vec->absolute();
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeAbsolute:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeMinAxis(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeMinAxis:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return vec->minAxis();
	} else {
		LOGE("Vector3#nativeMinAxis:invalid id of Vector3 object");
		return 0;
	}
}

static jfloat nativeMinAxisValue(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeMinAxisValue:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return (*vec)[vec->minAxis()];
	} else {
		LOGE("Vector3#nativeMinAxisValue:invalid id of Vector3 object");
		return 0.f;
	}
}

static jint nativeMaxAxis(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeMaxAxis:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return vec->maxAxis();
	} else {
		LOGE("Vector3#nativeMaxAxis:invalid id of Vector3 object");
		return 0;
	}
}

static jfloat nativeMaxAxisValue(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeMaxAxisValue:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return (*vec)[vec->maxAxis()];
	} else {
		LOGE("Vector3#nativeMaxAxisValue:invalid id of Vector3 object");
		return 0.f;
	}
}

static jint nativeFurthestAxis(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeFurthestAxis:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return vec->furthestAxis();
	} else {
		LOGE("Vector3#nativeFurthestAxis:invalid id of Vector3 object");
		return 0;
	}
}

static jfloat nativeFurthestAxisValue(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeFurthestAxisValue:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return (*vec)[vec->furthestAxis()];
	} else {
		LOGE("Vector3#nativeFurthestAxisValue:invalid id of Vector3 object");
		return 0.f;
	}
}

static jint nativeClosesttAxis(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeClosesttAxis:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return vec->closestAxis();
	} else {
		LOGE("Vector3#nativeClosesttAxis:invalid id of Vector3 object");
		return 0;
	}
}

static jfloat nativeClosesttAxisValue(JNIEnv *env, jobject thiz,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeClosesttAxisValue:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec)) {
		return (*vec)[vec->closestAxis()];
	} else {
		LOGE("Vector3#nativeClosesttAxisValue:invalid id of Vector3 object");
		return 0.f;
	}
}

static jint nativeMultVecMat(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec,
		ID_TYPE id_mat) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeMultVecMat:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(vec && mat)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (vec_result)
			*vec_result = (*vec) * (*mat);
		else
			*vec = (*vec) * (*mat);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeMultVecMat:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativePlaneSpace1(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_n,
		ID_VEC id_vec_p,
		ID_VEC id_vec_q) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativePlaneSpace1:");
#endif

	btVector3 *vec_n = reinterpret_cast<btVector3 *>(id_vec_n);
	btVector3 *vec_p = reinterpret_cast<btVector3 *>(id_vec_p);
	btVector3 *vec_q = reinterpret_cast<btVector3 *>(id_vec_q);
	if (LIKELY(vec_n && vec_p && vec_q)) {
		btPlaneSpace1(*vec_n, *vec_p, *vec_q);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativePlaneSpace1:invalid id of Vector3 object");
		return -1000;
	}
}

static jint nativeDot3(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_VEC id_vec,
		ID_VEC id_vec0,
		ID_VEC id_vec1,
		ID_VEC id_vec2) {

#if LOCAL_DEBUG
	LOGV("Vector3#nativeDot3:");
#endif

	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	btVector3 *vec0 = reinterpret_cast<btVector3 *>(id_vec0);
	btVector3 *vec1 = reinterpret_cast<btVector3 *>(id_vec1);
	btVector3 *vec2 = reinterpret_cast<btVector3 *>(id_vec2);
	if (LIKELY(vec && vec0 && vec1 && vec2)) {
		btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
		if (vec_result)
			*vec_result = vec->dot3(*vec0, *vec1, *vec2);
		else
			*vec = vec->dot3(*vec0, *vec1, *vec2);
		return JNI_OK;
	} else {
		LOGE("Vector3#nativeDot3:invalid id of Vector3 object");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(J)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeSet3", "(JFFF)I", (void *)nativeSet3},
	{"nativeSet4", "(JFFFF)I", (void *)nativeSet4},
	{"nativeSetVec", "(JJ)I", (void *)nativeSetVec},
	{"nativeSetA", "(JIF)I", (void *)nativeSetA},
	{"nativeGetA", "(JI)F", (void *)nativeGetA},
	{"nativeGetArray", "(J[F)I", (void *)nativeGetArray},
	{"nativeSetArray", "(J[F)I", (void *)nativeSetArray},
	{"nativeSetMax", "(JJ)I", (void *)nativeSetMax},
	{"nativeSetMin", "(JJ)I", (void *)nativeSetMin},
	{"nativeRotate", "(JJJF)I", (void *)nativeRotate},
	{"nativeLength2", "(J)F", (void *)nativeLength2},
	{"nativeLength", "(J)F", (void *)nativeLength},
	{"nativeDistance2", "(JJ)F", (void *)nativeDistance2},
	{"nativeDistance", "(JJ)F", (void *)nativeDistance},
	{"nativeLerp", "(JJJF)I", (void *)nativeLerp},
	{"nativeDot", "(JJ)F", (void *)nativeDot},
	{"nativeCross", "(JJJ)I", (void *)nativeCross},
	{"nativeTriple", "(JJJ)F", (void *)nativeTriple},
	{"nativeAddVec", "(JJJF)I", (void *)nativeAddVec},
	{"nativeMultScalar", "(JJF)I", (void *)nativeMultScalar},
	{"nativeDivScalar", "(JJF)I", (void *)nativeDivScalar},
	{"nativeNormalize", "(J)I", (void *)nativeNormalize},
	{"nativeSafeNormalize", "(J)I", (void *)nativeSafeNormalize},
	{"nativeAngle", "(JJ)F", (void *)nativeAngle},
	{"nativeAbsolute", "(JJ)I", (void *)nativeAbsolute},
	{"nativeMinAxis", "(J)I", (void *)nativeMinAxis},
	{"nativeMinAxisValue", "(J)F", (void *)nativeMinAxisValue},
	{"nativeMaxAxis", "(J)I", (void *)nativeMaxAxis},
	{"nativeMaxAxisValue", "(J)F", (void *)nativeMaxAxisValue},
	{"nativeFurthestAxis", "(J)I", (void *)nativeFurthestAxis},
	{"nativeFurthestAxisValue", "(J)F", (void *)nativeFurthestAxisValue},
	{"nativeClosesttAxis", "(J)I", (void *)nativeClosesttAxis},
	{"nativeClosesttAxisValue", "(J)F", (void *)nativeClosesttAxisValue},
	{"nativeMultVecMat", "(JJJ)I", (void *)nativeMultVecMat},
	{"nativePlaneSpace1", "(JJJ)I", (void *)nativePlaneSpace1},
	{"nativeDot3", "(JJJJJ)I", (void *)nativeDot3},
};

int register_Vector3(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/math/Vector3",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
