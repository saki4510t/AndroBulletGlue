/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: localdefines.h
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

#ifndef LOCALDEFINES_H_
#define LOCALDEFINES_H_

#include <jni.h>

#define LOG_TAG "ANDROBULLETGLUE"
#define INVALID_ARGUMENT -451055

// write back array that got by getXXXArrayElements into original Java object and release its array
#define	ARRAYELEMENTS_COPYBACK_AND_RELEASE 0
// write back array that got by getXXXArrayElements into origianl Java object but do not release its array
#define	ARRAYELEMENTS_COPYBACK_ONLY JNI_COMMIT
// never write back array that got by getXXXArrayElements but release its array
#define ARRAYELEMENTS_ABORT_AND_RELEASE JNI_ABORT

//#define USE_LOGALL	// If you don't need to all LOG, comment out this line and select follows
// #define USE_LOGV
// #define USE_LOGD
#define USE_LOGI
#define USE_LOGW
#define USE_LOGE
#define USE_LOGF

// type of collision configuration
enum CollisionConfigurationTypes {
	DEFAULT_COLLISION_CONFIGURATION = 0,
	SOFTBODY_RIGIDBODY_COLLISION_CONFIGURATION
};

// type of broadphase collision detection
enum BroadphaseTypes {
	BROADPHASE_SIMPLE = 0,		// 0
	BROADPHASE_AXISSWEEP3,		// 1
	BROADPHASE_32AXISSWEEP3,	// 2
	BROADPHASE_DBVT,			// 3
	BROADPHASE_MULTISAP,		// 4
	BROADPHASE_CUDA				// 5
};

// type of constraint solver
enum ConstraintSolverTypes {
	SEQUENTIAL_IMPULSE_CONSTRAINT_SOLVER = 1,	// 1
	VORONOI_SIMPLEX_SOLVER,						// 2
	MINKOWSKI_PENETRATION_DEPTH_SOLVER,			// 3
	GJK_EPA_PENETRATION_DEPTH_SOLVER,			// 4
	CONVEX_PENETRATION_DEPTH_SOLVER,			// 5
	SOFT_BODY_DEFAULT_SOLVER,					// 6  this is onaly available for softBody
	SOFT_BODY_CPU_SOLVER,						// 7  not implemented yet
	SOFT_BODY_CL_SOLVER,						// 8  not implemented yet
	SOFT_BODY_CL_SIMD_SOLVER,					// 9  not implemented yet
	SOFT_BODY_DX_SOLVER,						// 10 not implemented yet
	SOFT_BODY_DX_SIMD_SOLVER					// 11 not implemented yet
};

// Absolute class name of Java object
// if you change the package name of AndroBulletGlue library, you must fix these
#define		JTYPE_SYSTEM				"Ljava/lang/System;"
#define		JTYPE_BULLETMANAGER			"Lcom/serenegiant/androbulletglue/BulletManager;"
#define		JTYPE_MOTIONSTATE			"Lcom/serenegiant/androbulletglue/MotionState;"
#define		JTYPE_RIGIDBODY				"Lcom/serenegiant/androbulletglue/RigidBody;"
#define		JTYPE_TRANSFORM				"Lcom/serenegiant/androbulletglue/math/Transform;"
#define		JTYPE_VECTOR3				"Lcom/serenegiant/androbulletglue/math/Vector3;"
#define		JTYPE_QUAT					"Lcom/serenegiant/androbulletglue/math/Quaternion;"
#define		JTYPE_MAT3X3				"Lcom/serenegiant/androbulletglue/math/Matrix3x3;"
#define		JTYPE_INDEXEDMESH			"Lcom/serenegiant/androbulletglue/mesh/IndexedMesh;"
#define		JTYPE_INDEXEDMESHARRAY		"[Lcom/serenegiant/androbulletglue/mesh/IndexedMesh;"
#define		JTYPE_NATIVEOBJECT			"Lcom/serenegiant/androbulletglue/interfaces/NativeObject;"
#define		JTYPE_TYPEDINTERFACE		"Lcom/serenegiant/androbulletglue/interfaces/TypedInterface;"
#define		JTYPE_DYNAMICSWORLD			"Lcom/serenegiant/androbulletglue/interfaces/DynamicsWorld;"
#define		JTYPE_COLLISIONCONFIG		"Lcom/serenegiant/androbulletglue/interfaces/CollisionConfiguration;"
#define		JTYPE_COLLISIONDISPATCHER	"Lcom/serenegiant/androbulletglue/interfaces/CollisionDispatcher;"
#define		JTYPE_COLLISIONSHAPE		"Lcom/serenegiant/androbulletglue/interfaces/CollisionShape;"
#define		JTYPE_CONVEXSHAPE			"Lcom/serenegiant/androbulletglue/interfaces/ConvexShape;"
#define		JTYPE_BROADPHASE			"Lcom/serenegiant/androbulletglue/interfaces/Broadphase;"
#define		JTYPE_SOLVER				"Lcom/serenegiant/androbulletglue/interfaces/Solver;"
#define		JTYPE_BVHTRIANGLEMESHSHAPE	"Lcom/serenegiant/androbulletglue/shape/BvhTriangleMeshShape;"

#define		JMETHOD_GETRIGIDBODY		"(J)Lcom/serenegiant/androbulletglue/RigidBody;"

//
typedef		jlong						ID_TYPE;
typedef		ID_TYPE						ID_CTRLBLOCK;
typedef		ID_TYPE						ID_VEC;
typedef		ID_TYPE						ID_QUAT;
typedef		ID_TYPE						ID_TRANS;
typedef		ID_TYPE						ID_MAT;
typedef		ID_TYPE						ID_SHAPE;
typedef		ID_TYPE						ID_SOFTBODY;
typedef		ID_TYPE						ID_RIGIDBODY;
typedef		ID_TYPE						ID_CONSTRAINT;
typedef		ID_TYPE						ID_COLLISION;

#endif /* LOCALDEFINES_H_ */
