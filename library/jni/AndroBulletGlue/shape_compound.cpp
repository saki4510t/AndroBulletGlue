/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: shape_compound.cpp
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

#include "shape_compound.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static jint nativeAddChildShape(
		JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape,
		ID_TRANS id_trans,
		ID_SHAPE id_child_shape) {

#if LOCAL_DEBUG
	LOGV("CompoundShape#nativeAddChildShape:");
#endif
	btCompoundShape *compoundShape = reinterpret_cast<btCompoundShape *>(id_shape);
	btTransform *localTransform = reinterpret_cast<btTransform *>(id_trans);
	btCollisionShape *childShape = reinterpret_cast<btCollisionShape *>(id_child_shape);
	if (LIKELY(compoundShape && localTransform && childShape)) {
		compoundShape->addChildShape(*localTransform, childShape);
		return JNI_OK;
	} else {
		LOGE("CompoundShape#nativeAddChildShape:invalid id of CompoundShape/Transform/childShape");
		return -1000;
	}
}

static jint nativeRemoveChildShape(
		JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape,
		ID_SHAPE id_child_shape) {

#if LOCAL_DEBUG
	LOGV("CompoundShape#nativeRemoveChildShape:");
#endif
	btCompoundShape *compoundShape = reinterpret_cast<btCompoundShape *>(id_shape);
	btCollisionShape *childShape = reinterpret_cast<btCollisionShape *>(id_child_shape);
	if (LIKELY(compoundShape && childShape)) {
		compoundShape->removeChildShape(childShape);
		return JNI_OK;
	} else {
		LOGE("nativeRemoveChildShape:invalid id of CompoundShape/childShape");
		return -1000;
	}
}

static jint nativeGetNumChildShapes(
		JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape) {

#if LOCAL_DEBUG
	LOGV("CompoundShape#nativeGetNumChildShapes:");
#endif
	btCompoundShape *compoundShape = reinterpret_cast<btCompoundShape *>(id_shape);
	if (LIKELY(compoundShape)) {
		return compoundShape->getNumChildShapes();
	} else {
		LOGE("CompoundShape#nativeGetNumChildShapes:invalid id of CompoundShape");
		return 0;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeAddChildShape", "(JJJ)I", (void *)nativeAddChildShape},
	{"nativeRemoveChildShape", "(JJ)I", (void *)nativeRemoveChildShape},
	{"nativeGetNumChildShapes", "(J)I", (void *)nativeGetNumChildShapes},
};

int register_CompoundShape(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/shape/CompoundShape",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
