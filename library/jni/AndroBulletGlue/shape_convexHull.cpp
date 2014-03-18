/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: collisionShape_convexHull.cpp
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

#include "shape_convexHull.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static jint nativeAddPoint(
		JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape,
		ID_VEC id_vec_point) {

#if LOCAL_DEBUG
	LOGV("ConvexHullShape#nativeAddPoint:");
#endif
	btConvexHullShape *shape = reinterpret_cast<btConvexHullShape *>(id_shape);
	btVector3 *point = reinterpret_cast<btVector3 *>(id_vec_point);
	if (LIKELY(shape && point)) {
		shape->addPoint(*point);
		return JNI_OK;
	} else {
		LOGE("ConvexHullShape#nativeAddPoint:invalid id of ConvexHullShape/Vector3");
		return -1000;
	}
}

static jint nativeInitializePolyhedralFeatures(
		JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape,
		jint shiftVerticesByMargin) {

#if LOCAL_DEBUG
	LOGV("ConvexHullShape#nativeInitializePolyhedralFeatures:");
#endif
	btConvexHullShape *shape = reinterpret_cast<btConvexHullShape *>(id_shape);
	if (LIKELY(shape)) {
		shape->initializePolyhedralFeatures(shiftVerticesByMargin);
		return JNI_OK;
	} else {
		LOGE("ConvexHullShape#nativeInitializePolyhedralFeatures:invalid id of ConvexHullShape");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeAddPoint", "(JJ)I", (void *)nativeAddPoint},
	{"nativeInitializePolyhedralFeatures", "(JI)I", (void *)nativeInitializePolyhedralFeatures},
};

int register_ConvexHullShape(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/shape/ConvexHullShape",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
