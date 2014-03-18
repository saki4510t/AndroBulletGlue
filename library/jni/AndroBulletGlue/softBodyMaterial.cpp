/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: softbodyMaterial.cpp
 */
/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "softBodyMaterial.h"
#include "BulletSoftBody/btSoftBody.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static void nativeSetMaterial(JNIEnv *env, jobject thiz,
		ID_TYPE id_material) {

#if LOCAL_DEBUG
	LOGV("SoftBodyMaterial#setMaterial:");
#endif
	btSoftBody::Material *material = reinterpret_cast<btSoftBody::Material *>(id_material);
	if (material) {
		jclass clazz = env->GetObjectClass(thiz);

		material->m_kLST = __getField_float(env, thiz, clazz, "m_kLST");	// Linear stiffness coefficient [0,1]
		material->m_kAST = __getField_float(env, thiz, clazz, "m_kAST");	// Area/Angular stiffness coefficient [0,1]
		material->m_kVST = __getField_float(env, thiz, clazz, "m_kVST");	// Volume stiffness coefficient [0,1]
		material->m_flags = __getField_int(env, thiz, clazz, "m_flags");	// Flags

		env->DeleteLocalRef(clazz);
	} else {
		LOGE("SoftBodyMaterial#setMaterial:invalid id of material");
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeSetMaterial", "(J)V", (void *)nativeSetMaterial},
};

int register_SoftBodyMaterial(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/softbody/SoftBodyMaterial",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
