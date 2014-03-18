/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: utils.h
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

#ifndef UTILS_H_
#define UTILS_H_

#include <jni.h>
#include <android/log.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "localdefines.h"

#ifdef USE_LOGALL
	#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
	#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
	#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
	#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
	#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
	#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)
#else
	#ifdef USE_LOGV
		#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
	#else
		#define LOGV(...)
	#endif
	#ifdef USE_LOGD
		#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
	#else
		#define LOGD(...)
	#endif
	#ifdef USE_LOGI
		#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
	#else
		#define LOGI(...)
	#endif
	#ifdef USE_LOGW
		#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
	#else
		#define LOGW(...)
	#endif
	#ifdef USE_LOGE
		#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
	#else
		#define LOGE(...)
	#endif
	#ifdef USE_LOGF
		#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)
	#else
		#define LOGF(...)
	#endif
#endif

#define		SAFE_DELETE(p)				{ if (p) { delete (p); (p) = NULL; } }
#define		SAFE_DELETE_ARRAY(p)		{ if (p) { delete [](p); (p) = NULL; } }
#define		NUM_ARRAY_ELEMENTS(p)		((int) sizeof(p) / sizeof(p[0]))

// the macro for branch prediction optimaization for gcc
#define LIKELY(x)	__builtin_expect(!!(x), 1)	// x is likely true
#define	UNLIKELY(x)	__builtin_expect(!!(x), 0)	// x is likely false

// Utility functions
/**
 * get current time millis
 */
long getTimeMilliseconds(void);
/**
 * get elapsed time as microseconds from previous call
 * @param env
 */
jfloat getDeltaTimeMicroseconds(clock_t &prev_time);


/**
 * get RigidBody object(Java) that has specified id by calling PhysicsWorld#getRigidBody
 * @param env
 * @param bullet_obj
 * @param rigidBodyId
 * @return RigidBody
 */
jobject get_rigidbody(JNIEnv *env, jobject bullet_obj, ID_TYPE rigidBodyId);

/**
 * return whether or not the specified field is null
 * @param env
 * @param java_obj
 * @param field_name
 * @param field_type
 * @return return true if the field does not exist or the field value is null or the filed type is wrong
 */
bool isNullField(JNIEnv* env, jobject java_obj, const char* field_name, const char* field_type);

/**
 * return whether the Vector3 field is null or does not exist
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @param field_name
 * @return return true if the field does not exist or the field value is null
 */
inline bool isNullField_vec(JNIEnv *env, jobject java_obj, const char *field_name) {
	return isNullField(env, java_obj, field_name, JTYPE_VECTOR3);
}

/**
 * return whether the Quaternion field is null or does not exist
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @param field_name
 * @return return true if the field does not exist or the field value is null
 */
inline bool isNullField_quat(JNIEnv *env, jobject java_obj, const char *field_name) {
	return isNullField(env, java_obj, field_name, JTYPE_QUAT);
}

/**
 * return whether the Matrix3x3 field is null or does not exist
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @param field_name
 * @return return true if the field does not exist or the field value is null
 */
inline bool isNullField_mat3x3(JNIEnv *env, jobject java_obj, const char *field_name) {
	return isNullField(env, java_obj, field_name, JTYPE_MAT3X3);
}

/**
 * return a field value as boolean
 * @param env
 * @param java_obj
 * @return	return the value, returan false(0) if the field does not exist
 */
bool getField_bool(JNIEnv* env, jobject java_obj, const char* field_name);

/**
 * return the static int field value
 * @param env
 * @param java_obj
 * @return return the value, return 0 if the field does not exist
 */
jint getStaticField_int(JNIEnv* env, jobject java_obj, const char* field_name);

/**
 * set specified value into the static int field
 * @param env
 * @param java_obj
 * @param field_name
 * @params val
 */
jint setStaticField_int(JNIEnv* env, jobject java_obj, const char* field_name, jint val);

/**
 * get int field that has specified name from specified Java object
 * @param env
 * @param java_obj
 * @return return the value, return 0 if the field does not exist
 */
jint getField_int(JNIEnv* env, jobject java_obj, const char* field_name);
jint __getField_int(JNIEnv* env, jobject java_obj, jclass clazz, const char* field_name);

/**
 * set the value to int field of specified Java object
 * @param env
 * @param java_obj
 * @param field_name
 * @params val
 */
jint setField_int(JNIEnv* env, jobject java_obj, const char* field_name, jint val);
jint __setField_int(JNIEnv* env, jobject java_obj, jclass clazz, const char* field_name, jint val);

/**
 * get long field that has specified name from specified Java object
 * @param env
 * @param java_obj
 * @return return the value, return 0 if the field does not exist
 */
jlong getField_long(JNIEnv* env, jobject java_obj, const char* field_name);
jlong __getField_long(JNIEnv* env, jobject java_obj, jclass clazz, const char* field_name);

/**
 * set the value into the long field
 * @param env
 * @param java_obj
 * @param field_name
 * @params val
 */
jlong setField_long(JNIEnv* env, jobject java_obj, const char* field_name, jlong val);
jlong __setField_long(JNIEnv* env, jobject java_obj, jclass clazz, const char* field_name, jlong val);

/**
 * get static float field that has specified name from specified Java object
 * @param env
 * @param java_obj
 * @param field_name
 * @return return the value, return 0 if the field does not exist
 */
jfloat getStaticField_float(JNIEnv *env, jobject java_obj, const char *field_name);

/**
 * 指定したJavaオブジェクトの指定した名前のstatic float型のフィールド値を取得
 * @param env
 * @param java_obj
 * @param field_name
 */
jfloat setStaticField_float(JNIEnv *env, jobject java_obj, const char *field_name, jfloat val);

/**
 * get the value of float field that has specified name from specified Java object
 * @param env
 * @param java_obj
 * @param field_name
 * @return return the value, return 0 if the field does not exist
 */
jfloat getField_float(JNIEnv* env, jobject java_obj, const char* field_name);
jfloat __getField_float(JNIEnv* env, jobject java_obj, jclass clazz, const char* field_name);

/**
 * set float value into the specific Java object & field
 * @param env
 * @param java_obj
 * @param field_name
 * @params val
 */
jfloat setField_float(JNIEnv* env, jobject java_obj, const char* field_name, jfloat val);
jfloat __setField_float(JNIEnv *env, jobject java_obj, jclass clazz, const char *field_name, jfloat val);

/**
 * return specific Java object and its field value as a naitive pointer
 * @return pointer value
 */
ID_TYPE getField_NativeObj(JNIEnv *env, jobject java_obj, const char *field_name, const char *field_type);

/**
 * Helper method of getField_NativeObj for Vector3 field
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
inline btVector3 *getField_vec(JNIEnv *env, jobject java_obj, const char *field_name) {
	return reinterpret_cast<btVector3 *>(getField_NativeObj(env, java_obj, field_name, JTYPE_VECTOR3));
}

/**
 * Helper method of getField_NativeObj for Quaternion field
 */
inline btQuaternion *getField_quat(JNIEnv *env, jobject java_obj, const char *field_name) {
	return reinterpret_cast<btQuaternion *>(getField_NativeObj(env, java_obj, field_name, JTYPE_QUAT));
}

/**
 * Helper method of getField_NativeObj for Transform field
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
inline btTransform *getField_trans(JNIEnv *env, jobject java_obj, const char *field_name) {
	return reinterpret_cast<btTransform *>(getField_NativeObj(env, java_obj, field_name, JTYPE_TRANSFORM));
}

/**
 * Helper method of getField_NativeObj for Matrix3x3 field
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
inline btMatrix3x3 *getField_mat(JNIEnv *env, jobject java_obj, const char *field_name) {
	return reinterpret_cast<btMatrix3x3 *>(getField_NativeObj(env, java_obj, field_name, JTYPE_MAT3X3));
}

/**
 * return jobject filed that is specified type from specified field.
 * you should check the field exist and is not null with #isNullField
 * before you call this function.
 * @param env
 * @param java_obj
 * @param field_name
 * @param field_type
 * @return jobject
 */
jobject getField_obj(JNIEnv* env, jobject java_obj, const char* field_name, const char* obj_type);

/**
 * return id from specified field name and type
 * @param env
 * @param java_obj
 * @param field_name
 * @param field_type
 * @return ID_TYPE
 */
ID_TYPE getField_obj_id(JNIEnv* env, jobject java_obj, const char* field_name, const char* obj_type);

/**
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @return
 */
inline void *getField_nativePtr(JNIEnv *env, jobject java_obj, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("get_nativeRec:");
#endif
	return reinterpret_cast<void *>(getField_long(env, java_obj, field_name));
}

/**
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 */
inline void setField_nativePtr(JNIEnv *env, jobject java_obj, const char *field_name, void *ptr) {
#if LOCAL_DEBUG
	LOGV("get_nativeRec:");
#endif
	setField_long(env, java_obj, field_name, reinterpret_cast<ID_TYPE>(ptr));
}

/**
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @return return 0 if the field "mID" not exist
 */
inline ID_TYPE get_id(JNIEnv *env, jobject java_obj) {
#if LOCAL_DEBUG
	LOGV("get_id:");
#endif
	return getField_long(env, java_obj, "mID");
}

/**
 * set id into the field
 * @param env
 * @param java_obj
 * @param id
 */
inline ID_TYPE set_id(JNIEnv *env, jobject java_obj, ID_TYPE id) {
#if LOCAL_DEBUG
	LOGV("set_id:");
#endif

	setField_long(env, java_obj, "mID", id);
	return id;
}

jint registerNativeMethods(JNIEnv* env, const char *class_name, JNINativeMethod *methods, int num_methods);

#endif /* UTILS_H_ */
