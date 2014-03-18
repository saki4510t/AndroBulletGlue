/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: bulletmanager.cpp
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

#include "bulletmanager.h"
#include "variables.h"
#include "physicsWorld.h"
#include "shape.h"

#define LOCAL_DEBUG 0

// Bullet Objects
btAlignedObjectArray<PhysicsWorldCtrlBlock *> gPhysicsWorldCtrlBlocks;
btAlignedObjectArray<btCollisionShape *> gCollisionShapes;

//**********************************************************************
//
//**********************************************************************
static void nativeDestroy(JNIEnv* env, jobject thiz) {

#if LOCAL_DEBUG
	LOGD("BulletManager#nativeClearObjects:");
#endif
	// delete DynamicsWorld.
	for (int i = 0; i < gPhysicsWorldCtrlBlocks.size(); i++ ) {
		destroyPhysicsWorld(gPhysicsWorldCtrlBlocks[i]);
	}
	gPhysicsWorldCtrlBlocks.clear();

	// delete Collisions.
	clearCollisionShape();

}

static jint nativeClearObjects(JNIEnv* env, jobject thiz,
	jboolean needClearCollisionShape) {

	// call clearObjects on all physics worlds
	for (int i = 0; i < gPhysicsWorldCtrlBlocks.size(); i++ ) {
		clearObjects(gPhysicsWorldCtrlBlocks[i]);
	}
	if (needClearCollisionShape)
		clearCollisionShape();

	return JNI_OK;
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeDestroy", "()V", (void *)nativeDestroy},
	{"nativeClearObjects", "(Z)I", (void *)nativeClearObjects},
};

int register_BulletManager(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/BulletManager",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
