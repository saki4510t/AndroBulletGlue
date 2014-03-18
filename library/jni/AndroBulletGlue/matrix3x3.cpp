/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: matrix3x3.cpp
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

#include "matrix3x3.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_MAT nativeCreate(JNIEnv *env, jobject thiz,
		ID_MAT id_mat_src) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeCreate:");
#endif

	btMatrix3x3 *matrix;
	btMatrix3x3 *mat_src = reinterpret_cast<btMatrix3x3 *>(id_mat_src);
	if (mat_src)
		matrix = new btMatrix3x3(*mat_src);
	else
		matrix = new btMatrix3x3();
	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_MAT>(matrix);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_MAT id_mat) {

#if LOCAL_DEBUG
	LOGD("Matrix3x3#nativeDestroy:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	SAFE_DELETE(mat);
}

static jint nativeSet(JNIEnv *env, jobject thiz,
		ID_MAT id_mat,
		jfloat xx, jfloat xy, jfloat xz,
		jfloat yx, jfloat yy, jfloat yz,
		jfloat zx, jfloat zy, jfloat zz) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeSet:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	if (LIKELY(mat)) {
		mat->setValue(xx, xy, xz, yx, yy, yz, zx, zy, zz);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeSet:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeCopy(JNIEnv *env, jobject thiz,
		ID_MAT id_mat_dest,
		ID_MAT id_mat_src) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeCopy:");
#endif

	btMatrix3x3 *mat_dest = reinterpret_cast<btMatrix3x3 *>(id_mat_dest);
	btMatrix3x3 *mat_src = reinterpret_cast<btMatrix3x3 *>(id_mat_src);
	if (LIKELY(mat_dest && mat_src)) {
		*mat_dest = *mat_src;
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeCopy:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeGetRotation(JNIEnv *env, jobject thiz,
		ID_MAT id_mat,
		ID_QUAT id_quat) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeGetRotation:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	if (LIKELY(mat && quat)) {
		mat->getRotation(*quat);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeGetRotation:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeSetRotation(JNIEnv *env, jobject thiz,
		ID_MAT id_mat,
		ID_QUAT id_quat) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeSetRotation:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btQuaternion *quat = reinterpret_cast<btQuaternion *>(id_quat);
	if (LIKELY(mat && quat)) {
		mat->setRotation(*quat);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeSetRotation:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeGetEulerZYX(JNIEnv *env, jobject thiz,
		ID_MAT id_mat,
		ID_VEC id_vec_result) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeGetEulerZYX:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
	if (LIKELY(mat && vec_result)) {
		mat->getEulerZYX((*vec_result)[0], (*vec_result)[1], (*vec_result)[2]);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeGetEulerZYX:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeSetEulerZYX(JNIEnv *env, jobject thiz,
		ID_MAT id_mat,
		jfloat yaw, jfloat pitch, jfloat roll) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeSetEulerZYX:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	if (LIKELY(mat)) {
		mat->setEulerZYX(roll, pitch, yaw);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeSetEulerZYX:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeSetEulerZYXVec(JNIEnv *env, jobject thiz,
		ID_MAT id_mat,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeSetEulerZYXVec:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(mat && vec)) {
		mat->setEulerZYX((*vec)[2], (*vec)[1], (*vec)[0]);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeSetEulerZYXVec:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeMultMat(JNIEnv *env, jobject thiz,
		ID_MAT id_mat_result,
		ID_MAT id_mat1,
		ID_MAT id_mat2) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeMultMat:");
#endif

	btMatrix3x3 *mat1 = reinterpret_cast<btMatrix3x3 *>(id_mat1);
	btMatrix3x3 *mat2 = reinterpret_cast<btMatrix3x3 *>(id_mat2);
	if (LIKELY(mat1 && mat2)) {
		btMatrix3x3 *mat_result = reinterpret_cast<btMatrix3x3 *>(id_mat_result);
		if (!mat_result || (mat_result == mat1)) {
			*mat1 *= *mat2;
		} else {
			*mat_result = (*mat1) * (*mat2);
		}
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeMultMat:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeMultMatVec(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_MAT id_mat,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeMultMatVec:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
	if (LIKELY(mat && vec_result && vec)) {
		*vec_result = *mat * *vec;
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeMultMatVec:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeScaledMat(JNIEnv *env, jobject thiz,
		ID_MAT id_mat_result,
		ID_MAT id_mat,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeScaledMat:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btVector3 *vec = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(mat && vec)) {
		btMatrix3x3 *mat_result = reinterpret_cast<btMatrix3x3 *>(id_mat_result);
		if (!mat_result || (mat_result == mat)) {
			*mat = mat->scaled(*vec);
		} else {
			*mat_result = mat->scaled(*vec);
		}
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeScaledMat:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeTranspose(JNIEnv *env, jobject thiz,
		ID_MAT id_mat_result,
		ID_MAT id_mat) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeTranspose:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	if (LIKELY(mat)) {
		btMatrix3x3 *mat_result = reinterpret_cast<btMatrix3x3 *>(id_mat_result);
		if (!mat_result || (mat_result == mat)) {
			*mat = mat->transpose();
		} else {
			*mat_result = mat->transpose();
		}
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeTranspose:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeInverse(JNIEnv *env, jobject thiz,
		ID_MAT id_mat_result,
		ID_MAT id_mat) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeInverse:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	if (LIKELY(mat)) {
		btMatrix3x3 *mat_result = reinterpret_cast<btMatrix3x3 *>(id_mat_result);
		if (!mat_result || (mat_result == mat)) {
			*mat = mat->inverse();
		} else {
			*mat_result = mat->inverse();
		}
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeInverse:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeAbsolute(JNIEnv *env, jobject thiz,
		ID_MAT id_mat_result,
		ID_MAT id_mat) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeAbsolute:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	if (LIKELY(mat)) {
		btMatrix3x3 *mat_result = reinterpret_cast<btMatrix3x3 *>(id_mat_result);
		if (!mat_result || (mat_result == mat)) {
			*mat = mat->absolute();
		} else {
			*mat_result = mat->absolute();
		}
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeAbsolute:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeGetOpenGLSubMatrix(JNIEnv *env, jobject thiz,
		ID_MAT id_mat,
		jfloatArray resultArray) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeGetOpenGLSubMatrix:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	// 配列の長さチェックはjava側で行う
	if (LIKELY(mat && resultArray)) {
		jfloat *result = env->GetFloatArrayElements(resultArray, NULL);
		mat->getOpenGLSubMatrix(result);
		// jfloat配列へ値を書き戻して参照を開放
		env->ReleaseFloatArrayElements(resultArray, result, ARRAYELEMENTS_COPYBACK_AND_RELEASE);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeGetOpenGLSubMatrix:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeSetFromOpenGLSubMatrix(JNIEnv *env, jobject thiz,
		ID_MAT id_mat,
		jfloatArray glMatArray) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeGetOpenGLSubMatrix:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	// 配列の長さチェックはjava側で行う
	if (LIKELY(mat && glMatArray)) {
		jfloat *array = env->GetFloatArrayElements(glMatArray, NULL);
		mat->setFromOpenGLSubMatrix(array);
		// jfloat配列へ値を書き戻さず参照を開放
		env->ReleaseFloatArrayElements(glMatArray, array, ARRAYELEMENTS_ABORT_AND_RELEASE);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeGetOpenGLSubMatrix:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeGetRow(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_MAT id_mat,
		jint row) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeGetRow:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
	if (LIKELY(mat && vec_result)) {
		*vec_result = mat->getRow(row);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeGetRow:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

static jint nativeGetColumn(JNIEnv *env, jobject thiz,
		ID_VEC id_vec_result,
		ID_MAT id_mat,
		jint col) {

#if LOCAL_DEBUG
	LOGV("Matrix3x3#nativeGetColumn:");
#endif

	btMatrix3x3 *mat = reinterpret_cast<btMatrix3x3 *>(id_mat);
	btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
	if (LIKELY(mat && vec_result)) {
		*vec_result = mat->getColumn(col);
		return JNI_OK;
	} else {
		LOGE("Matrix3x3#nativeGetColumn:invalid id of btMatrix3x3 object");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(J)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeSet", "(JFFFFFFFFF)I", (void *)nativeSet},
	{"nativeCopy", "(JJ)I", (void *)nativeCopy},
	{"nativeGetRotation", "(JJ)I", (void *)nativeGetRotation},
	{"nativeSetRotation", "(JJ)I", (void *)nativeSetRotation},
	{"nativeGetEulerZYX", "(JJ)I", (void *)nativeGetEulerZYX},
	{"nativeSetEulerZYX", "(JFFF)I", (void *)nativeSetEulerZYX},
	{"nativeSetEulerZYXVec", "(JJ)I", (void *)nativeSetEulerZYXVec},
	{"nativeMultMat", "(JJJ)I", (void *)nativeMultMat},
	{"nativeMultMatVec", "(JJJ)I", (void *)nativeMultMatVec},
	{"nativeScaledMat", "(JJJ)I", (void *)nativeScaledMat},
	{"nativeTranspose", "(JJ)I", (void *)nativeTranspose},
	{"nativeInverse", "(JJ)I", (void *)nativeInverse},
	{"nativeAbsolute", "(JJ)I", (void *)nativeAbsolute},
	{"nativeGetOpenGLSubMatrix", "(J[F)I", (void *)nativeGetOpenGLSubMatrix},
	{"nativeSetFromOpenGLSubMatrix", "(J[F)I", (void *)nativeSetFromOpenGLSubMatrix},
	{"nativeGetRow", "(JJI)I", (void *)nativeGetRow},
	{"nativeGetColumn", "(JJI)I", (void *)nativeGetColumn},
};

int register_Matrix3x3(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/math/Matrix3x3",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
