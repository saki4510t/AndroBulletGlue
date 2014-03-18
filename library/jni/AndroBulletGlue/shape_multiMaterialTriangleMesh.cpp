/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: shape_multiMaterialTriangleMesh.cpp
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

#include "shape_multiMaterialTriangleMesh.h"
#include "btMultimaterialTriangleMeshShape.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static jint nativeGetMaterialProperties(
		JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape,
		jobject material_obj,
		jint partID, jint triIndex) {

#if LOCAL_DEBUG
	LOGV("MultimaterialTriangleMeshShape#nativeGetMaterialProperties:");
#endif
	btMultimaterialTriangleMeshShape *shape = reinterpret_cast<btMultimaterialTriangleMeshShape *>(id_shape);
	if (LIKELY(shape && material_obj)) {
		const btMaterial *material = shape->getMaterialProperties(partID, triIndex);

		jclass clazz = env->GetObjectClass(material_obj);

		__setField_float(env, material_obj, clazz, "mFriction", material->m_friction);
		__setField_float(env, material_obj, clazz, "mRestitution", material->m_restitution);
		__setField_int(env, material_obj, clazz, "mPad0", material->pad[0]);
		__setField_int(env, material_obj, clazz, "mPad1", material->pad[1]);

		env->DeleteLocalRef(clazz);

		setField_long(env, material_obj, "mID", reinterpret_cast<ID_TYPE>(material));	// set_id(env, material_obj, reinterpret_cast<ID_TYPE>(material));
		return JNI_OK;
	} else {
		LOGE("MultimaterialTriangleMeshShape#nativeGetMaterialProperties:invalid id of MultimaterialTriangleMeshShape/Material");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeGetMaterialProperties", "(JLcom/serenegiant/androbulletglue/mesh/TriangleMaterial;II)I", (void *)nativeGetMaterialProperties},
};

int register_MultimaterialTriangleMeshShape(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/shape/MultimaterialTriangleMeshShape",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
