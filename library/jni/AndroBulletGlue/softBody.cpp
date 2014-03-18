/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: softBody.cpp
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

#include "softBody.h"

#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include "variables.h"
#include "physicsWorld.h"

#include "motorControl.h"
#include "steerControl.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
static ID_SOFTBODY nativeCreate(JNIEnv *env, jobject thiz,
		ID_CTRLBLOCK id_ctrlblock,
		jint type) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeCreate:type=%d", type);
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!gPhysicsWorldCtrlBlocks.get(ctrlBlock))) {
		LOGE("SoftBody#nativeCreate::bullet control block not found");
		return (ID_SOFTBODY)NULL;
	}

	btSoftBody *softBody = NULL;
	switch (type) {
	case SOFTBODY_ROPE:				// 0
		softBody = btSoftBodyHelpers::CreateRope(*ctrlBlock->softBodyWorldInfo,
			*getField_vec(env, thiz, "mFrom"),
			*getField_vec(env, thiz, "mTo"),
			getField_int(env, thiz, "mResolution"),
			getField_int(env, thiz, "mFixeds") );
		break;
	case SOFTBODY_PATCH:			// 1
		softBody = btSoftBodyHelpers::CreatePatch(*ctrlBlock->softBodyWorldInfo,
			*getField_vec(env, thiz, "mCorner00"),
			*getField_vec(env, thiz, "mCorner10"),
			*getField_vec(env, thiz, "mCorner01"),
			*getField_vec(env, thiz, "mCorner11"),
			getField_int(env, thiz, "mResolutionX"),
			getField_int(env, thiz, "mResolutionY"),
			getField_int(env, thiz, "mFixeds"),
			getField_bool(env, thiz, "mGendiags") );
		break;
	case SOFTBODY_PATCHUV:			// 2
	{
		jobject tex_coordsArray_obj = getField_obj(env, thiz, "mTex_coords", "[F");
		jfloatArray *tex_coordsArray = reinterpret_cast<jfloatArray *>(&tex_coordsArray_obj);
		jfloat *tex_coords = env->GetFloatArrayElements(*tex_coordsArray, NULL);

		softBody = btSoftBodyHelpers::CreatePatchUV(*ctrlBlock->softBodyWorldInfo,
			*getField_vec(env, thiz, "mCorner00"),
			*getField_vec(env, thiz, "mCorner10"),
			*getField_vec(env, thiz, "mCorner01"),
			*getField_vec(env, thiz, "mCorner11"),
			getField_int(env, thiz, "mResolutionX"),
			getField_int(env, thiz, "mResolutionY"),
			getField_int(env, thiz, "mFixeds"),
			getField_bool(env, thiz, "mGendiags"),
			tex_coords);
		// jfloat配列への参照を開放
		env->ReleaseFloatArrayElements(*tex_coordsArray, tex_coords, ARRAYELEMENTS_ABORT_AND_RELEASE);
		env->DeleteLocalRef(tex_coordsArray_obj);
		break;
	}
	case SOFTBODY_ELLIPSOID:		// 3
		softBody = btSoftBodyHelpers::CreateEllipsoid(*ctrlBlock->softBodyWorldInfo,
			*getField_vec(env, thiz, "mCenter"),
			*getField_vec(env, thiz, "mRadius"),
			getField_int(env, thiz, "mResolution") );
		break;
	case SOFTBODY_FROM_TRIMESH:		// 4
	{
		jobject verticesArray_obj = getField_obj(env, thiz, "mVertices", "[F");
		jfloatArray *verticesArray = reinterpret_cast<jfloatArray *>(&verticesArray_obj);
		jfloat *vertices = env->GetFloatArrayElements(*verticesArray, NULL);

		jobject trianglesArray_obj = getField_obj(env, thiz, "mTriangles", "[I");
		jintArray *trianglesArray = reinterpret_cast<jintArray *>(&trianglesArray_obj);
		jint *triangles = env->GetIntArrayElements(*trianglesArray, NULL);

		softBody = btSoftBodyHelpers::CreateFromTriMesh(*ctrlBlock->softBodyWorldInfo,
			vertices,
			triangles,
			getField_int(env, thiz, "mNumTriangles"),
			getField_bool(env, thiz, "mRandomizeConstraints") );
		// jfloat配列への参照を開放
		env->ReleaseFloatArrayElements(*verticesArray, vertices, ARRAYELEMENTS_ABORT_AND_RELEASE);
		env->DeleteLocalRef(verticesArray_obj);
		// jint配列への参照を開放
		env->ReleaseIntArrayElements(*trianglesArray, triangles, ARRAYELEMENTS_ABORT_AND_RELEASE);
		env->DeleteLocalRef(trianglesArray_obj);
		break;
	}
	case SOFTBODY_FROM_CONVEXHULL:	// 5
	{
		jobject verticesArray_obj = getField_obj(env, thiz, "mVertices", "[F");
		jfloatArray *verticesArray = reinterpret_cast<jfloatArray *>(&verticesArray_obj);
		jfloat *vertices = env->GetFloatArrayElements(*verticesArray, NULL);
		jint numVertices = getField_int(env, thiz, "mNumVertices");
		btAlignedObjectArray<btVector3>	pts;
		for (int i = 0; i < numVertices; i++) {
			pts.push_back(btVector3(
				vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]));
		}
		softBody = btSoftBodyHelpers::CreateFromConvexHull(*ctrlBlock->softBodyWorldInfo,
			&pts[0],
			getField_bool(env, thiz, "mRandomizeConstraints") );
		softBody->generateBendingConstraints(2);
		// jfloat配列への参照を開放
		env->ReleaseFloatArrayElements(*verticesArray, vertices, ARRAYELEMENTS_ABORT_AND_RELEASE);
		env->DeleteLocalRef(verticesArray_obj);
		break;
	}
	case SOFTBODY_FROM_TETGENDATA:	// 6
		// TODO 未実装
		break;
	}
	if (LIKELY(softBody)) {
		((btSoftRigidDynamicsWorld *)ctrlBlock->dynamicsWorld)->addSoftBody(softBody);
	} else {
		LOGE("SoftBody#nativeCreate: softbody type%d not supported", type);
	}
	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_SOFTBODY>(softBody);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody) {

#if LOCAL_DEBUG
	LOGD("SoftBody#nativeDestroy:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(
			getField_nativePtr(env, thiz, "mPhysicsWorldID"));
	if (UNLIKELY(!gPhysicsWorldCtrlBlocks.get(ctrlBlock))) {
		LOGE("SoftBody#nativeDestroy::bullet control block not found");
	}
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		if (ctrlBlock)
			((btSoftRigidDynamicsWorld *)ctrlBlock->dynamicsWorld)->removeSoftBody(softBody);
		SAFE_DELETE(softBody);
	}
}

static void nativeSetTotalMass(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jfloat mass,
		jboolean fromFaces) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeSetTotalMass:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->setTotalMass(mass, fromFaces);
	} else {
		LOGE("SoftBody#nativeSetTotalMass:invalid id of softBody");
	}
}

static void nativeAppendAnchor(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		ID_RIGIDBODY id_rigidBody,
		jint node,
		ID_VEC id_vec_localPivot,
		jboolean disableCollisionBetweenLinkedBodies,
		jfloat influence) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeAppendAnchor:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	btRigidBody *rigidBody = reinterpret_cast<btRigidBody *>(id_rigidBody);
	if (LIKELY(softBody && rigidBody)) {
		btVector3 *localPivot = reinterpret_cast<btVector3 *>(id_vec_localPivot);
		if (localPivot) {
			softBody->appendAnchor(node, rigidBody,
				*localPivot,
				disableCollisionBetweenLinkedBodies, influence);
		} else {
			softBody->appendAnchor(node, rigidBody,
				disableCollisionBetweenLinkedBodies, influence);
		}
	} else {
		LOGE("SoftBody#nativeAppendAnchor:invalid id of softBody or rigidBody");
	}
}

static void nativeAppendLinearJoint(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		ID_VEC id_vec_position,
		ID_COLLISION id_body) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeAppendLinearJoint:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	btCollisionObject *body = reinterpret_cast<btCollisionObject *>(id_body);
	if (LIKELY(softBody && body)) {
		btVector3 *position = reinterpret_cast<btVector3 *>(id_vec_position);

		btSoftBody::LJoint::Specs lJoint;
		lJoint.position	= *position;

		btRigidBody *rBody = btRigidBody::upcast(body);
		if (rBody) {
			softBody->appendLinearJoint(lJoint, rBody);
		} else {
			btSoftBody *sBody = btSoftBody::upcast(body);
			if (sBody) {
				softBody->appendLinearJoint(lJoint, sBody);
			}
		}
	} else {
		LOGE("SoftBody#nativeAppendLinearJoint:invalid id of softBody or rigidBody");
	}
}

static void nativeAppendAngularJoint(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		ID_VEC id_vec_axis,
		ID_TYPE id_motor,
		ID_COLLISION id_body) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeAppendAngularJoint:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	MotorControl *motor = reinterpret_cast<MotorControl *>(id_motor);
	btCollisionObject *body = reinterpret_cast<btCollisionObject *>(id_body);
	if (LIKELY(softBody && body)) {
		btVector3 *axis = reinterpret_cast<btVector3 *>(id_vec_axis);

		btSoftBody::AJoint::Specs aJoint;
		aJoint.axis	= *axis;
		if (motor) {
			aJoint.icontrol = motor;
		}

		btRigidBody *rBody = btRigidBody::upcast(body);
		if (rBody) {
			softBody->appendAngularJoint(aJoint, rBody);
		} else {
			btSoftBody *sBody = btSoftBody::upcast(body);
			if (sBody) {
				softBody->appendAngularJoint(aJoint, sBody);
			}
		}
	} else {
		LOGE("SoftBody#nativeAppendAngularJoint:invalid id of softBody or rigidBody");
	}
}

static jint nativeGetNodeNum(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody) {

#if LOCAL_DEBUG
	LOGV("SoftBody#getNodeNum:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		return softBody->m_nodes.size();
	} else {
		LOGE("SoftBody#getNodeNum:invalid id of softBody or rigidBody");
		return 0;
	}
}

static void nativeSetCfgSolverIterations(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jint type,
		jint iterations) {

#if LOCAL_DEBUG
	LOGV("SoftBody#setCfgSolverIterations:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		switch (type) {
		case 0:	softBody->m_cfg.viterations = iterations; break;	// Velocities solver iterations
		case 1:	softBody->m_cfg.piterations = iterations; break;	// Positions solver iterations
		case 2:	softBody->m_cfg.diterations = iterations; break;	// Drift solver iterations
		case 3:	softBody->m_cfg.citerations = iterations; break;	// Cluster solver iterations
		}
	} else {
		LOGE("SoftBody#setCfgSolverIterations:invalid id of softBody");
	}
}

static void nativeSetMaterialLinearStiffnessCoefficient(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jint ix,
		jfloat klst) {

#if LOCAL_DEBUG
	LOGV("SoftBody#setMaterialLinearStiffnessCoefficient:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->m_materials[ix]->m_kLST = klst;
	} else {
		LOGE("SoftBody#setMaterialLinearStiffnessCoefficient:invalid id of softBody");
	}
}

static void nativeSetMaterialAngularStiffnessCoefficient(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jint ix,
		jfloat kAST) {

#if LOCAL_DEBUG
	LOGV("SoftBody#setMaterialAngularStiffnessCoefficient:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->m_materials[ix]->m_kAST = kAST;
	} else {
		LOGE("SoftBody#setMaterialAngularStiffnessCoefficient:invalid id of softBody");
	}
}

static void nativeSetMaterialVolumeStiffnessCoefficient(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jint ix,
		jfloat kVST) {

#if LOCAL_DEBUG
	LOGV("SoftBody#setMaterialVolumeStiffnessCoefficient:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->m_materials[ix]->m_kVST = kVST;
	} else {
		LOGE("SoftBody#setMaterialVolumeStiffnessCoefficient:invalid id of softBody");
	}
}

static void nativeAppendMaterial(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jobject material_obj) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeAppendMaterial:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody && material_obj)) {
		btSoftBody::Material *material = softBody->appendMaterial();

		jclass clazz = env->GetObjectClass(material_obj);

		__setField_float(env, material_obj, clazz, "m_kLST", material->m_kLST);	// Linear stiffness coefficient [0,1]
		__setField_float(env, material_obj, clazz, "m_kAST", material->m_kAST);	// Area/Angular stiffness coefficient [0,1]
		__setField_float(env, material_obj, clazz, "m_kVST", material->m_kVST);	// Volume stiffness coefficient [0,1]
		__setField_int(env, material_obj, clazz, "m_flags", material->m_flags);	// Flags

		env->DeleteLocalRef(clazz);

		setField_long(env, material_obj, "mID", reinterpret_cast<ID_TYPE>(material));	// set_id(env, material_obj, reinterpret_cast<ID_TYPE>(material));
	} else {
		LOGE("SoftBody#appendMaterialNative:invalid id of softBody");
	}
}

static jint nativeGenerateBendingConstraints(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jint distance,
		jobject material_obj) {

#if LOCAL_DEBUG
	LOGV("SoftBody#generateBendingConstraintsNative:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody && material_obj)) {
		btSoftBody::Material *material = (btSoftBody::Material *)get_id(env, material_obj);
		if (material) {
			jclass clazz = env->GetObjectClass(material_obj);

			material->m_kLST = __getField_float(env, material_obj, clazz, "m_kLST");	// Linear stiffness coefficient [0,1]
			material->m_kAST = __getField_float(env, material_obj, clazz, "m_kAST");	// Area/Angular stiffness coefficient [0,1]
			material->m_kVST = __getField_float(env, material_obj, clazz, "m_kVST");	// Volume stiffness coefficient [0,1]
			material->m_flags = __getField_int(env, material_obj, clazz, "m_flags");	// Flags

			env->DeleteLocalRef(clazz);

			softBody->generateBendingConstraints(distance, material);
			return JNI_OK;
		} else {
			LOGE("SoftBody#setMaterialNative:invalid id of material");
		}
	} else {
		LOGE("SoftBody#setMaterialNative:invalid id of softBody");
	}
	return -1000;
}

static jint nativeSetClustersMatching(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jint ix, jfloat matching) {

#if LOCAL_DEBUG
	LOGV("SoftBody#setClustersMatching:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->m_clusters[ix]->m_matching = matching;
		return JNI_OK;
	} else {
		LOGE("SoftBody#setClustersMatching:invalid id of softBody");
		return -1000;
	}
}

static jint nativeSetClustersNdamping(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jint ix, jfloat damping) {

#if LOCAL_DEBUG
	LOGV("SoftBody#setClustersNdamping:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->m_clusters[ix]->m_ndamping = damping;
		return JNI_OK;
	} else {
		LOGE("SoftBody#setClustersNdamping:invalid id of softBody");
		return -1000;
	}
}

static jint nativeGenerateClusters(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jint k, jint maxiterations) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeGenerateClusters:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->generateClusters(k, maxiterations);
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeGenerateClusters:invalid id of softBody");
		return -1000;
	}
}

static jint nativeGetConfig(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jobject config_obj) {	// SoftBodyConfig

#if LOCAL_DEBUG
	LOGV("SoftBody#getConfigNative:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody && config_obj)) {
		jclass clazz = env->GetObjectClass(config_obj);

		__setField_float(env, config_obj, clazz, "kVCF", softBody->m_cfg.kVCF);					// Velocities correction factor (Baumgarte)
		__setField_float(env, config_obj, clazz, "kDP", softBody->m_cfg.kDP);					// Damping coefficient [0,1]
		__setField_float(env, config_obj, clazz, "kDG", softBody->m_cfg.kDG);					// Drag coefficient [0,+inf]
		__setField_float(env, config_obj, clazz, "kLF", softBody->m_cfg.kLF);					// Lift coefficient [0,+inf]
		__setField_float(env, config_obj, clazz, "kPR", softBody->m_cfg.kPR);					// Pressure coefficient [-inf,+inf]
		__setField_float(env, config_obj, clazz, "kVC", softBody->m_cfg.kVC);					// Volume conversation coefficient [0,+inf]
		__setField_float(env, config_obj, clazz, "kDF", softBody->m_cfg.kDF);					// Dynamic friction coefficient [0,1]
		__setField_float(env, config_obj, clazz, "kMT", softBody->m_cfg.kMT);					// Pose matching coefficient [0,1]
		__setField_float(env, config_obj, clazz, "kCHR", softBody->m_cfg.kCHR);					// Rigid contacts hardness [0,1]
		__setField_float(env, config_obj, clazz, "kKHR", softBody->m_cfg.kKHR);					// Kinetic contacts hardness [0,1]
		__setField_float(env, config_obj, clazz, "kSHR", softBody->m_cfg.kSHR);					// Soft contacts hardness [0,1]
		__setField_float(env, config_obj, clazz, "kAHR", softBody->m_cfg.kAHR);					// Anchors hardness [0,1]
		__setField_float(env, config_obj, clazz, "kSRHR_CL", softBody->m_cfg.kSRHR_CL);			// Soft vs rigid hardness [0,1] (cluster only)
		__setField_float(env, config_obj, clazz, "kSKHR_CL", softBody->m_cfg.kSKHR_CL);			// Soft vs kinetic hardness [0,1] (cluster only)
		__setField_float(env, config_obj, clazz, "kSSHR_CL", softBody->m_cfg.kSSHR_CL);			// Soft vs soft hardness [0,1] (cluster only)
		__setField_float(env, config_obj, clazz, "kSR_SPLT_CL", softBody->m_cfg.kSR_SPLT_CL);	// Soft vs rigid impulse split [0,1] (cluster only)
		__setField_float(env, config_obj, clazz, "kSK_SPLT_CL", softBody->m_cfg.kSK_SPLT_CL);	// Soft vs rigid impulse split [0,1] (cluster only)
		__setField_float(env, config_obj, clazz, "kSS_SPLT_CL", softBody->m_cfg.kSS_SPLT_CL);	// Soft vs rigid impulse split [0,1] (cluster only)
		__setField_float(env, config_obj, clazz, "maxvolume", softBody->m_cfg.maxvolume);		// Maximum volume ratio for pose
		__setField_float(env, config_obj, clazz, "timescale", softBody->m_cfg.timescale);		// Time scale

		__setField_int(env, config_obj, clazz, "viterations", softBody->m_cfg.viterations);		// Velocities solver iterations
		__setField_int(env, config_obj, clazz, "piterations", softBody->m_cfg.piterations);		// Positions solver iterations
		__setField_int(env, config_obj, clazz, "diterations", softBody->m_cfg.diterations);		// Drift solver iterations
		__setField_int(env, config_obj, clazz, "citerations", softBody->m_cfg.citerations);		// Cluster solver iterations
		__setField_int(env, config_obj, clazz, "collisions", softBody->m_cfg.collisions);		// Collisions flags

		env->DeleteLocalRef(clazz);
		return JNI_OK;
	} else {
		LOGE("SoftBody#getConfigNative:invalid id of SoftBody");
		return -1000;
	}
}

static jint nativeSetConfig(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jobject config_obj) {	// SoftBodyConfig

#if LOCAL_DEBUG
	LOGV("SoftBody#setConfigNative:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody && config_obj)) {
		jclass clazz = env->GetObjectClass(config_obj);

		softBody->m_cfg.kVCF = __getField_float(env, config_obj, clazz, "kVCF");				// Velocities correction factor (Baumgarte)
		softBody->m_cfg.kDP = __getField_float(env, config_obj, clazz, "kDP");					// Damping coefficient [0,1]
		softBody->m_cfg.kDG = __getField_float(env, config_obj, clazz, "kDG");					// Drag coefficient [0,+inf]
		softBody->m_cfg.kLF = __getField_float(env, config_obj, clazz, "kLF");					// Lift coefficient [0,+inf]
		softBody->m_cfg.kPR = __getField_float(env, config_obj, clazz, "kPR");					// Pressure coefficient [-inf,+inf]
		softBody->m_cfg.kVC = __getField_float(env, config_obj, clazz, "kVC");					// Volume conversation coefficient [0,+inf]
		softBody->m_cfg.kDF = __getField_float(env, config_obj, clazz, "kDF");					// Dynamic friction coefficient [0,1]
		softBody->m_cfg.kMT = __getField_float(env, config_obj, clazz, "kMT");					// Pose matching coefficient [0,1]
		softBody->m_cfg.kCHR = __getField_float(env, config_obj, clazz, "kCHR");				// Rigid contacts hardness [0,1]
		softBody->m_cfg.kKHR = __getField_float(env, config_obj, clazz, "kKHR");				// Kinetic contacts hardness [0,1]
		softBody->m_cfg.kSHR = __getField_float(env, config_obj, clazz, "kSHR");				// Soft contacts hardness [0,1]
		softBody->m_cfg.kAHR = __getField_float(env, config_obj, clazz, "kAHR");				// Anchors hardness [0,1]
		softBody->m_cfg.kSRHR_CL = __getField_float(env, config_obj, clazz, "kSRHR_CL");		// Soft vs rigid hardness [0,1] (cluster only)
		softBody->m_cfg.kSKHR_CL = __getField_float(env, config_obj, clazz, "kSKHR_CL");		// Soft vs kinetic hardness [0,1] (cluster only)
		softBody->m_cfg.kSSHR_CL = __getField_float(env, config_obj, clazz, "kSSHR_CL");		// Soft vs soft hardness [0,1] (cluster only)
		softBody->m_cfg.kSR_SPLT_CL = __getField_float(env, config_obj, clazz, "kSR_SPLT_CL");	// Soft vs rigid impulse split [0,1] (cluster only)
		softBody->m_cfg.kSK_SPLT_CL = __getField_float(env, config_obj, clazz, "kSK_SPLT_CL");	// Soft vs rigid impulse split [0,1] (cluster only)
		softBody->m_cfg.kSS_SPLT_CL = __getField_float(env, config_obj, clazz, "kSS_SPLT_CL");	// Soft vs rigid impulse split [0,1] (cluster only)
		softBody->m_cfg.maxvolume = __getField_float(env, config_obj, clazz, "maxvolume");		// Maximum volume ratio for pose
		softBody->m_cfg.timescale = __getField_float(env, config_obj, clazz, "timescale");		// Time scale

		softBody->m_cfg.viterations = __getField_int(env, config_obj, clazz, "viterations");	// Velocities solver iterations
		softBody->m_cfg.piterations = __getField_int(env, config_obj, clazz, "piterations");	// Positions solver iterations
		softBody->m_cfg.diterations = __getField_int(env, config_obj, clazz, "diterations");	// Drift solver iterations
		softBody->m_cfg.citerations = __getField_int(env, config_obj, clazz, "citerations");	// Cluster solver iterations
		softBody->m_cfg.collisions = __getField_int(env, config_obj, clazz, "collisions");		// Collisions flags

		env->DeleteLocalRef(clazz);
		return JNI_OK;
	} else {
		LOGE("SoftBody#setConfigNative:invalid id of SoftBody");
		return -1000;
	}
}

static jint nativeRandomizeConstraints(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody) {

#if LOCAL_DEBUG
	LOGV("SoftBody#randomizeConstraints:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->randomizeConstraints();
		return JNI_OK;
	} else {
		LOGE("SoftBody#randomizeConstraints:invalid id of softBody");
		return -1000;
	}
}

static jint nativeTransform(
		JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		ID_TRANS id_trans) {

#if LOCAL_DEBUG
	LOGV("SoftBody#transformNative:");
#endif
	btTransform *trans = reinterpret_cast<btTransform *>(id_trans);
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody && trans)) {
		softBody->transform(*trans);
		return JNI_OK;
	} else {
		LOGE("SoftBody#transformNative:invalid id of SoftBody/Transform");
		return -1000;
	}
}

static jint nativeRotate(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jfloat angle_x, jfloat angle_y, jfloat angle_z) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeRotate:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->rotate(btQuaternion(angle_x, angle_y, angle_z));
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeRotate:invalid id of SoftBody");
		return -1000;
	}
}

static jint nativeRotateVec(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		ID_VEC id_vec_angle) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeRotateVec:");
#endif
	btVector3 *angle = reinterpret_cast<btVector3 *>(id_vec_angle);
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody && angle)) {
		softBody->rotate(btQuaternion(angle->x(), angle->y(), angle->z()));
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeRotateVec:invalid id of SoftBody/Vector3");
		return -1000;
	}
}

static jint nativeRotateQuat(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		ID_QUAT id_quat_rot) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeRotateQuat:");
#endif
	btQuaternion *quat_rot = reinterpret_cast<btQuaternion *>(id_quat_rot);
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody && quat_rot)) {
		softBody->rotate(*quat_rot);
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeRotateQuat:invalid id of softBody");
		return -1000;
	}
}

static jint nativeTranslate(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jfloat dx, jfloat dy, jfloat dz) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeTranslate:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->translate(btVector3(dx, dy, dz));
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeTranslate:invalid id of softBody");
		return -1000;
	}
}

static jint nativeTranslateVec(JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		ID_VEC id_vec_delta) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeTranslateVec:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		btVector3 *delta = reinterpret_cast<btVector3 *>(id_vec_delta);
		softBody->translate(*delta);
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeTranslateVec:invalid id of softBody");
		return -1000;
	}
}

static jint nativeScale(
		JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jfloat scale_x, jfloat scale_y, jfloat scale_z) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeScale:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->scale(btVector3((btScalar)scale_x, (btScalar)scale_y, (btScalar)scale_z));
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeScale:invalid id of softBody");
		return -1000;
	}
}

static jint nativeScaleVec(
		JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		ID_VEC id_vec_scale) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeScaleVec:");
#endif
	btVector3 *scale = reinterpret_cast<btVector3 *>(id_vec_scale);
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody && scale)) {
		softBody->scale(*scale);
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeScaleVec:invalid id of SoftBody/Vector3");
		return -1000;
	}
}

static jint nativeSetCollisionShapeMargin(
		JNIEnv *env, jobject thiz,
		ID_SOFTBODY id_softBody,
		jfloat margin) {

#if LOCAL_DEBUG
	LOGV("SoftBody#nativeSetCollisionShapeMargin:");
#endif
	btSoftBody *softBody = reinterpret_cast<btSoftBody *>(id_softBody);
	if (LIKELY(softBody)) {
		softBody->getCollisionShape()->setMargin(margin);
		return JNI_OK;
	} else {
		LOGE("SoftBody#nativeSetCollisionShapeMargin:invalid id of softBody");
		return -1000;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(JI)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeSetTotalMass", "(JFZ)V", (void *)nativeSetTotalMass},
	{"nativeAppendAnchor", "(JJIJZF)V", (void *)nativeAppendAnchor},
	{"nativeAppendLinearJoint", "(JJJ)V", (void *)nativeAppendLinearJoint},
	{"nativeAppendAngularJoint", "(JJJJ)V", (void *)nativeAppendAngularJoint},
	{"nativeGetNodeNum", "(J)I", (void *)nativeGetNodeNum},
	{"nativeSetCfgSolverIterations", "(JII)V", (void *)nativeSetCfgSolverIterations},
	{"nativeSetMaterialLinearStiffnessCoefficient", "(JIF)V", (void *)nativeSetMaterialLinearStiffnessCoefficient},
	{"nativeSetMaterialAngularStiffnessCoefficient", "(JIF)V", (void *)nativeSetMaterialAngularStiffnessCoefficient},
	{"nativeSetMaterialVolumeStiffnessCoefficient", "(JIF)V", (void *)nativeSetMaterialVolumeStiffnessCoefficient},
	{"nativeAppendMaterial", "(JLcom/serenegiant/androbulletglue/softbody/SoftBodyMaterial;)V", (void *)nativeAppendMaterial},
	{"nativeGenerateBendingConstraints", "(JILcom/serenegiant/androbulletglue/softbody/SoftBodyMaterial;)I", (void *)nativeGenerateBendingConstraints},
	{"nativeSetClustersMatching", "(JIF)I", (void *)nativeSetClustersMatching},
	{"nativeSetClustersNdamping", "(JIF)I", (void *)nativeSetClustersNdamping},
	{"nativeGenerateClusters", "(JII)I", (void *)nativeGenerateClusters},
	{"nativeGetConfig", "(JLcom/serenegiant/androbulletglue/softbody/SoftBodyConfig;)I", (void *)nativeGetConfig},
	{"nativeSetConfig", "(JLcom/serenegiant/androbulletglue/softbody/SoftBodyConfig;)I", (void *)nativeSetConfig},
	{"nativeRandomizeConstraints", "(J)I", (void *)nativeRandomizeConstraints},
	{"nativeTransform", "(JJ)I", (void *)nativeTransform},
	{"nativeRotate", "(JFFF)I", (void *)nativeRotate},
	{"nativeRotateVec", "(JJ)I", (void *)nativeRotateVec},
	{"nativeRotateQuat", "(JJ)I", (void *)nativeRotateQuat},
	{"nativeTranslate", "(JFFF)I", (void *)nativeTranslate},
	{"nativeTranslateVec", "(JJ)I", (void *)nativeTranslateVec},
	{"nativeScale", "(JFFF)I", (void *)nativeScale},
	{"nativeScaleVec", "(JJ)I", (void *)nativeScaleVec},
	{"nativeSetCollisionShapeMargin", "(JF)I", (void *)nativeSetCollisionShapeMargin},
};

int register_SoftBody(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/interfaces/SoftBody",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
