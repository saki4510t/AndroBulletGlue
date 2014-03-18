/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: constraint_coneTwist.cpp
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

#include "constraint_coneTwist.h"

#define LOCAL_DEBUG 0

static jint nativeSetLimitValue(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jint limitIndex,
		jfloat limitValue) {

#if LOCAL_DEBUG
	LOGV("ConeTwistConstraint#setLimitValue:");
#endif
	btConeTwistConstraint *constraint = reinterpret_cast<btConeTwistConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setLimit(limitIndex, limitValue);
		return JNI_OK;
	} else {
		LOGE("ConeTwistConstraint#setLimitValue:invalid ID of ConeTwistConstraint");
		return -1000;
	}
}

static jint nativeSetLimit(JNIEnv *env, jobject thiz,
		ID_CONSTRAINT id_constraint,
		jfloat swingSpan1,
		jfloat swingSpan2,
		jfloat twistSpan,
		jfloat softness,
		jfloat biasFactor,
		jfloat relaxationFactor) {

#if LOCAL_DEBUG
	LOGV("ConeTwistConstraint#setLimit:");
#endif
	btConeTwistConstraint *constraint = reinterpret_cast<btConeTwistConstraint *>(id_constraint);
	if (LIKELY(constraint)) {
		constraint->setLimit(swingSpan1, swingSpan2, twistSpan, softness, biasFactor, relaxationFactor);
		return JNI_OK;
	} else {
		LOGE("ConeTwistConstraint#setLimit:invalid ID of ConeTwistConstraint");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeSetLimitValue", "(JIF)I", (void *)nativeSetLimitValue},
	{"nativeSetLimit", "(JFFFFFF)I", (void *)nativeSetLimit},
};

int register_ConeTwistConstraint(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/constraint/ConeTwistConstraint",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
