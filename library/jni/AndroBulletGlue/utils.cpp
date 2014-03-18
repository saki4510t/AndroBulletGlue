/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: utils.cpp
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

#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define LOCAL_DEBUG 0
/**
 * get current time millis
 */
long getTimeMilliseconds(void) {
    struct timeval  now;
    gettimeofday(&now, NULL);
    return (long)(now.tv_sec * 1000 + now.tv_usec / 1000);
}

/**
 * get elapsed time as microseconds from previous call
 * @param env
 */
float getDeltaTimeMicroseconds(clock_t &prev_time) {
	clock_t current = clock();
	clock_t delta = current - prev_time;
	prev_time = current;
	return (delta * 1000000.f) / CLOCKS_PER_SEC;
}

/**
 * get RigidBody object(Java) that has specified id by calling PhysicsWorld#getRigidBody
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 * @param rigidBodyId
 * @return RigidBody
 */
jobject get_rigidbody(JNIEnv *env, jobject bullet_obj, ID_TYPE rigidBodyId) {
#if LOCAL_DEBUG
	LOGV("getRigidBody:");
#endif

	jclass method_clazz = env->GetObjectClass(bullet_obj);

	jmethodID method = env->GetMethodID(method_clazz, "getRigidBody", JMETHOD_GETRIGIDBODY);
	jobject obj;
	if (LIKELY(method))
		obj = env->CallObjectMethod(bullet_obj, method, rigidBodyId);
	else {
		LOGE("get_type:method 'getRigidBody' not found");
		obj = NULL;
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(method_clazz);
#endif
	return obj;
}
/**
 * return whether or not the specified field is null
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @param field_name
 * @param field_type
 * @return return true if the field does not exist or the field value is null or the filed type is wrong
 */
bool isNullField(JNIEnv *env, jobject java_obj, const char *field_name, const char *field_type) {
#if LOCAL_DEBUG
	LOGV("isNullField:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	jfieldID id = env->GetFieldID(clazz, field_name, field_type);
	bool is_null = false;
	if (LIKELY(id)) {
		jobject obj = env->GetObjectField(java_obj, id);
		if (!obj) {
#if LOCAL_DEBUG
			LOGD("isNullField:field %s(type=%s) is NULL!", field_name, field_type);
#endif
			is_null = true;
		}
#ifdef ANDROID_NDK
		env->DeleteLocalRef(obj);
#endif
	} else {
#if LOCAL_DEBUG
		LOGD("isNullField:fieldname '%s' not found or fieldtype '%s' mismatch", field_name, field_type);
#endif
		env->ExceptionClear();	// clear java.lang.NoSuchFieldError exception
		is_null = true;
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return is_null;
}

/**
 * return a field value as boolean
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @return	return the value, return false(0) if the field does not exist.
 */
bool getField_bool(JNIEnv *env, jobject java_obj, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("getField_bool:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	jfieldID id = env->GetFieldID(clazz, field_name, "Z");
	bool val;
	if (LIKELY(id))
		val = env->GetBooleanField(java_obj, id);
	else {
		LOGE("getField_bool:field '%s' not found", field_name);
		val = false;
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * return the static int field value
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @return	return the value, return 0 if the field does not exist.
 */
jint getStaticField_int(JNIEnv *env, jobject java_obj, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("getField_int:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	jfieldID id = env->GetStaticFieldID(clazz, field_name, "I");
	jint val;
	if (LIKELY(id))
		val = env->GetStaticIntField(clazz, id);
	else {
		LOGE("getField_int:field '%s' not found", field_name);
		val = 0;
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * set specified value into the static int field
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @param field_name
 * @params val
 */
jint setStaticField_int(JNIEnv *env, jobject java_obj, const char *field_name, jint val) {
#if LOCAL_DEBUG
	LOGV("setField_int:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	jfieldID id = env->GetStaticFieldID(clazz, field_name, "I");
	if (LIKELY(id))
		env->SetStaticIntField(clazz, id, val);
	else {
		LOGE("setField_int:field '%s' not found", field_name);
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * get int field that has specified name from specified Java object
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @return retun the value, return 0 if the field does not exist
 */
jint getField_int(JNIEnv *env, jobject java_obj, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("getField_int:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);
	jint val = __getField_int(env, java_obj, clazz, field_name);
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 */
jint __getField_int(JNIEnv *env, jobject java_obj, jclass clazz, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("__getField_int:");
#endif

	jfieldID id = env->GetFieldID(clazz, field_name, "I");
	jint val;
	if (LIKELY(id))
		val = env->GetIntField(java_obj, id);
	else {
		LOGE("__getField_int:field '%s' not found", field_name);
		val = 0;
	}
	return val;
}

/**
 * set the value into int field
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @param field_name
 * @params val
 */
jint setField_int(JNIEnv *env, jobject java_obj, const char *field_name, jint val) {
#if LOCAL_DEBUG
	LOGV("setField_int:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);
	__setField_int(env, java_obj, clazz, field_name, val);
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
jint __setField_int(JNIEnv *env, jobject java_obj, jclass clazz, const char *field_name, jint val) {
#if LOCAL_DEBUG
	LOGV("__setField_int:");
#endif

	jfieldID id = env->GetFieldID(clazz, field_name, "I");
	if (LIKELY(id))
		env->SetIntField(java_obj, id, val);
	else {
		LOGE("__setField_int:field '%s' not found", field_name);
	}
	return val;
}

/**
 * return the long field value
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 * @return retun the value, return 0 if the field does not exist
  */
jlong getField_long(JNIEnv *env, jobject java_obj, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("getField_long:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);
	jlong val = __getField_long(env, java_obj, clazz, field_name);
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
jlong __getField_long(JNIEnv *env, jobject java_obj, jclass clazz, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("__getField_long:");
#endif

	jfieldID id = env->GetFieldID(clazz, field_name, "J");
	jlong val;
	if (LIKELY(id))
		val = env->GetLongField(java_obj, id);
	else {
		LOGE("__getField_long:field '%s' not found", field_name);
		val = 0;
	}
	return val;
}

/**
 * set the value into the long field
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 * @param field_name
 * @params val
 */
jlong setField_long(JNIEnv *env, jobject java_obj, const char *field_name, jlong val) {
#if LOCAL_DEBUG
	LOGV("setField_long:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);
	__setField_long(env, java_obj, clazz, field_name, val);
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
jlong __setField_long(JNIEnv *env, jobject java_obj, jclass clazz, const char *field_name, jlong val) {
#if LOCAL_DEBUG
	LOGV("__setField_long:");
#endif

	jfieldID field = env->GetFieldID(clazz, field_name, "J");
	if (LIKELY(field))
		env->SetLongField(java_obj, field, val);
	else {
		LOGE("__setField_long:field '%s' not found", field_name);
	}
	return val;
}

/**
 * return the static float field value
 * @param env: this param should not be null
 * @param java_obj: this param should not be null
 * @param field_name
 * @return retun the value, return 0 if the field does not exist
 */
jfloat getStaticField_float(JNIEnv *env, jobject java_obj, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("getField_float:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	jfieldID id = env->GetStaticFieldID(clazz, field_name, "F");
	jfloat val;
	if (LIKELY(id))
		val = env->GetStaticFloatField(clazz, id);
	else {
		LOGE("getStaticField_float:field '%s' not found", field_name);
		val = 0.f;
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * set the value into static float field
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 * @param field_name
 */
jfloat setStaticField_float(JNIEnv *env, jobject java_obj, const char *field_name, jfloat val) {
#if LOCAL_DEBUG
	LOGV("getField_float:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	jfieldID id = env->GetStaticFieldID(clazz, field_name, "F");
	if (LIKELY(id))
		env->SetStaticFloatField(clazz, id, val);
	else {
		LOGE("setStaticField_float:field '%s' not found", field_name);
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * return the value of float field
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 * @param field_name
 * @return retun the value, return 0 if the field does not exist
 */
jfloat getField_float(JNIEnv *env, jobject java_obj, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("getField_float:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	jfloat val = __getField_float(env, java_obj, clazz, field_name);
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
jfloat __getField_float(JNIEnv *env, jobject java_obj, jclass clazz, const char *field_name) {
#if LOCAL_DEBUG
	LOGV("__getField_float:");
#endif

	jfieldID id = env->GetFieldID(clazz, field_name, "F");
	jfloat val;
	if ((id))
		val = env->GetFloatField(java_obj, id);
	else {
		LOGE("__getField_float:field '%s' not found", field_name);
		val = 0.f;
	}
	return val;
}

/**
 * set the value into the float field
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 * @param field_name
 * @params val
 */
jfloat setField_float(JNIEnv *env, jobject java_obj, const char *field_name, jfloat val) {
#if LOCAL_DEBUG
	LOGV("setField_Float:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);
	__setField_float(env, java_obj, clazz, field_name, val);
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return val;
}

/**
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
jfloat __setField_float(JNIEnv *env, jobject java_obj, jclass clazz, const char *field_name, jfloat val) {
#if LOCAL_DEBUG
	LOGV("__setField_Float:");
#endif

	jfieldID field = env->GetFieldID(clazz, field_name, "F");
	if (LIKELY(field))
		env->SetFloatField(java_obj, field, val);
	else {
		LOGE("__setField_Float:field '%s' not found", field_name);
	}
	return val;
}

/**
 * return specific Java object and its field value as a naitive pointer
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 */
ID_TYPE getField_NativeObj(JNIEnv *env, jobject java_obj, const char *field_name, const char *field_type) {
#if LOCAL_DEBUG
	LOGV("getField_NativeObj:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	ID_TYPE native_id = (ID_TYPE)NULL;
	jfieldID fieldID = env->GetFieldID(clazz, field_name, field_type);
	if (LIKELY(fieldID)) {
		jobject obj = env->GetObjectField(java_obj, fieldID);
		if (LIKELY(obj)) {
			native_id = get_id(env, obj);
#ifdef ANDROID_NDK
			env->DeleteLocalRef(obj);
#endif
		}
	} else {
		LOGE("getField_NativeObj:field object %s(type='%s') not found", field_name, field_type);
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return native_id;
}

/**
 * return jobject filed that is specified type from specified field.
 * you should check the field exist and is not null with #isNullField
 * before you call this function.
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 * @param field_name
 * @param field_type
 * @return jobject
 */
jobject getField_obj(JNIEnv *env, jobject java_obj, const char *field_name, const char *field_type) {
#if LOCAL_DEBUG
	LOGV("getField_obj:");
#endif

	jclass clazz = env->GetObjectClass(java_obj);

	jobject obj = NULL;
	jfieldID id = env->GetFieldID(clazz, field_name, field_type);
	if (LIKELY(id)) {
		obj = env->GetObjectField(java_obj, id);
	} else {
		LOGE("getField_obj:field object %s(type='%s') not found", field_name, field_type);
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return obj;
}

/**
 * return id from specified field name and type
 * @param env: this param should not be null
 * @param bullet_obj: this param should not be null
 * @param field_name
 * @param field_type
 * @return ID_TYPE
 */
ID_TYPE getField_obj_id(JNIEnv* env, jobject java_obj, const char* field_name, const char* obj_type) {
#if LOCAL_DEBUG
	LOGV("getField_obj_id:");
#endif

	ID_TYPE obj_id = 0;

	jclass clazz = env->GetObjectClass(java_obj);
	jfieldID id = env->GetFieldID(clazz, field_name, obj_type);
	if (LIKELY(id)) {
		jobject obj = env->GetObjectField(java_obj, id);
		if (LIKELY(obj)) {
			obj_id = get_id(env, obj);
			env->DeleteLocalRef(obj);
		} else {
			LOGE("getField_obj_id:field object %s(type='%s') is null", field_name, obj_type);
		}
	} else {
		LOGE("getField_obj_id:field object %s(type='%s') not found", field_name, obj_type);
	}
#ifdef ANDROID_NDK
	env->DeleteLocalRef(clazz);
#endif
	return obj_id;
}

jint registerNativeMethods(JNIEnv* env, const char *class_name, JNINativeMethod *methods, int num_methods) {
	int result = 0;

	jclass clazz = env->FindClass(class_name);
	if (LIKELY(clazz)) {
		int result = env->RegisterNatives(clazz, methods, num_methods);
		if (UNLIKELY(result < 0)) {
			LOGE("registerNativeMethods failed(class=%s)", class_name);
		}
	} else {
		LOGE("registerNativeMethods: class'%s' not found", class_name);
	}
	return result;
}
