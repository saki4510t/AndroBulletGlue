/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: GLBulletWorldSurfaceView.cpp
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


#include "GLBulletWorldSurfaceView.h"
#include "physicsworld.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static void nativeResize(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jint width, jint height) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeResize:");
#endif

	if (!width && !height) return;

	// when screen size changed
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeResize:bullet control block not found");
		return;
	}
//	ctrlBlock->bulletWorldDraw->resetFrame();
	ctrlBlock->bulletWorldDraw->resize(width, height);
#if LOCAL_DEBUG
    LOGV("GLBulletWorldSurfaceView#nativeResize:resized w=%d h=%d", width, height);
#endif
}

static void nativeLostGLContext(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock) {

	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeLostGLContext:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->lostGLContext();
}

/* Call to render the next GL frame */
static void nativeRender(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock) {

#if LOCAL_DEBUG
//	LOGV("GLBulletWorldSurfaceView#nativeRender");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeRender:bullet control block not found");
		return;
	}

	// render frame
    ctrlBlock->bulletWorldDraw->renderFrame();
	// optional but useful: debug drawing
    ctrlBlock->dynamicsWorld->debugDrawWorld();
}

/* Call to render the profile text */
static void nativeRenderProfile(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock) {

	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeRenderProfile:bullet control block not found");
		return;
	}

	// render frame
    ctrlBlock->bulletWorldDraw->renderProfile();
}

/**
 * Move camera & zoomIn/zoomOut
 * x: minus = rotate left, plus = rotate right
 * y: minus = rotate down, plus = rotate up
 * z: minus = zoon out, plus = zoom in
 */
static void nativeMoveCamera(
	JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jint x, jint y, jint z) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeMoveCamera:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeMoveCamera:bullet control block not found");
		return;
	}
	if (x < 0)
		ctrlBlock->bulletWorldDraw->stepLeft();
	else if (x > 0)
		ctrlBlock->bulletWorldDraw->stepRight();
	if (y < 0)
		ctrlBlock->bulletWorldDraw->stepFront();
	else if (y > 0)
		ctrlBlock->bulletWorldDraw->stepBack();
	if (z < 0)
		ctrlBlock->bulletWorldDraw->zoomOut();
	else if (z > 0)
		ctrlBlock->bulletWorldDraw->zoomIn();
}

static void nativeSetCameraPosition(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jfloat x, jfloat y, jfloat z) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetCameraPosition:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetCameraPosition:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setCameraPosition(x, y, z);
}

static void nativeSetCameraTargetPosition(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jfloat x, jfloat y, jfloat z) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetCameraTargetPosition:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetCameraTargetPosition:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setCameraTargetPosition(x, y, z);
}

static void nativeSetCameraDistance(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jfloat distance) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetCameraDistance:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetCameraDistance:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setCameraDistance(distance);
}

static void nativeSetCameraDistanceMinMax(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jfloat distance,
	jfloat min_distance,
	jfloat max_distance) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetCameraDistanceMinMax:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetCameraDistanceMinMax:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setCameraDistance(distance, min_distance, max_distance);
}

static void nativeSetCameraHeight(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jfloat height) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetCameraHeight:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetCameraHeight:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setCameraHeight(height);
}

static void nativeShootBox(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jint screen_x,
	jint screen_y) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeShootBox:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeShootBox:bullet control block not found");
		return;
	}
	BulletWorldDraw *bulletWorldDraw = ctrlBlock->bulletWorldDraw;
	bulletWorldDraw->shootBox(bulletWorldDraw->getRayTo(screen_x, screen_y));
}

static void nativeSetShootSpeed(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jfloat speed) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetShootSpeed:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetShootSpeed:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setShootBoxInitialSpeed(speed);
}

static void nativeClientResetScene(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeClientResetScene:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeClientResetScene:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->clientResetScene();
	if (ctrlBlock->softBodyWorldInfo) {
		ctrlBlock->softBodyWorldInfo->m_sparsesdf.Reset();
		// TODO 初期化する値を固定にしていいかな?
		ctrlBlock->softBodyWorldInfo->air_density		=	(btScalar)1.2;
		ctrlBlock->softBodyWorldInfo->water_density		=	0.f;
		ctrlBlock->softBodyWorldInfo->water_offset		=	0.f;
		ctrlBlock->softBodyWorldInfo->water_normal		=	btVector3(0.f, 0.f, 0.f);
		ctrlBlock->softBodyWorldInfo->m_gravity.setValue(0, -9.8f, 0);
	}
}

static void nativeSetTexturing(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jboolean enable) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetTexturing:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetTexturing:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setTexturing(enable);
}

static void nativeSetDrawShadows(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jboolean drawShadows) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetDrawShadows:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetDrawShadows:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setDrawShadows(drawShadows);
}

static void nativeSetDrawClusters(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jboolean drawClusters) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetDrawClusters:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetDrawClusters:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setDrawClusters(drawClusters);
}

static jint nativeToggleDebugMode(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jint mode) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeToggleDebugMode:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeToggleDebugMode:bullet control block not found");
		return 0;
	}
	ctrlBlock->bulletWorldDraw->toggleDebugMode(mode);
	return ctrlBlock->bulletWorldDraw->getDebugMode();
}

static jint nativeSetDebugMode(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jint mode) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetDebugMode:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetDebugMode:bullet control block not found");
		return 0;
	}
	ctrlBlock->bulletWorldDraw->setDebugMode(mode);
	return ctrlBlock->bulletWorldDraw->getDebugMode();
}

static jint nativeGetDebugMode(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeGetDebugMode:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeGetDebugMode:bullet control block not found");
		return 0;
	}
	return ctrlBlock->bulletWorldDraw->getDebugMode();
}

static jint nativeProcessTouch(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jint state,			// 0: UP, 1:DOWN, 2:MOVE
	jint screen_x,
	jint screen_y) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeProcessTouch:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeProcessTouch:bullet control block not found");
		return 0;
	}
	switch (state) {
	case 0:
	case 1:
		return ctrlBlock->bulletWorldDraw->processTouch(state, screen_x, screen_y);
	case 2:
		return ctrlBlock->bulletWorldDraw->touchMove(screen_x, screen_y);
	default:
		return 0;
	}
}

static void nativeSetPickMode(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	jint mode) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetPickMode:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetPickMode:bullet control block not found");
		return;
	}
	ctrlBlock->bulletWorldDraw->setPickMode(mode);
}

static jboolean nativeIsPicking(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeIsPicking:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeIsPicking:bullet control block not found");
		return 0;
	}
	return ctrlBlock->bulletWorldDraw->isPicking();
}

static void nativeSetTracking(JNIEnv *env, jobject thiz,
	ID_CTRLBLOCK id_ctrlblock,
	ID_RIGIDBODY rigidBodyID,
	jfloat minDistance,
	jfloat maxDistance) {

#if LOCAL_DEBUG
	LOGV("GLBulletWorldSurfaceView#nativeSetTracking:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("GLBulletWorldSurfaceView#nativeSetTracking:bullet control block not found");
		return;
	}
	btRigidBody *trackingBody = reinterpret_cast<btRigidBody *>(rigidBodyID);
	if (LIKELY(trackingBody)) {
		ctrlBlock->bulletWorldDraw->setTracking(
			trackingBody, minDistance,  maxDistance);
	} else {
		LOGE("GLBulletWorldSurfaceView#nativeSetTracking:rigidBody not found");
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeResize", "(JII)V", (void *)nativeResize},
	{"nativeLostGLContext", "(J)V", (void *)nativeLostGLContext},
	{"nativeRender", "(J)V", (void *)nativeRender},
	{"nativeRenderProfile", "(J)V", (void *)nativeRenderProfile},
	{"nativeMoveCamera", "(JIII)V", (void *)nativeMoveCamera},
	{"nativeSetCameraPosition", "(JFFF)V", (void *)nativeSetCameraPosition},
	{"nativeSetCameraTargetPosition", "(JFFF)V", (void *)nativeSetCameraTargetPosition},
	{"nativeSetCameraDistance", "(JF)V", (void *)nativeSetCameraDistance},
	{"nativeSetCameraDistanceMinMax", "(JFFF)V", (void *)nativeSetCameraDistanceMinMax},
	{"nativeSetCameraHeight", "(JF)V", (void *)nativeSetCameraHeight},
	{"nativeShootBox", "(JII)V", (void *)nativeShootBox},
	{"nativeSetShootSpeed", "(JF)V", (void *)nativeSetShootSpeed},
	{"nativeClientResetScene", "(J)V", (void *)nativeClientResetScene},
	{"nativeSetTexturing", "(JZ)V", (void *)nativeSetTexturing},
	{"nativeSetDrawShadows", "(JZ)V", (void *)nativeSetDrawShadows},
	{"nativeSetDrawClusters", "(JZ)V", (void *)nativeSetDrawClusters},
	{"nativeToggleDebugMode", "(JI)I", (void *)nativeToggleDebugMode},
	{"nativeSetDebugMode", "(JI)I", (void *)nativeSetDebugMode},
	{"nativeGetDebugMode", "(J)I", (void *)nativeGetDebugMode},
	{"nativeProcessTouch", "(JIII)I", (void *)nativeProcessTouch},
	{"nativeSetPickMode", "(JI)V", (void *)nativeSetPickMode},
	{"nativeIsPicking", "(J)Z", (void *)nativeIsPicking},
	{"nativeSetTracking", "(JJFF)V", (void *)nativeSetTracking},
};

int register_GLBulletWorldSurfaceView(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/GLBulletWorldSurfaceView",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
