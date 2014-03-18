/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: physicsWorld.cpp
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

#include "physicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "variables.h"

#define LOCAL_DEBUG 0
#define MIN_FPS 1000000.f / 60.f

//**********************************************************************
//
//**********************************************************************
static btCollisionConfiguration *createCollisionConfiguration(
		JNIEnv *env, jobject collisionConfiguration_obj) {

	if (UNLIKELY(!collisionConfiguration_obj)) {
		LOGE("createCollisionConfiguration:collisionConfiguration field is null");
	}
	jint type = collisionConfiguration_obj ? getField_int(env, collisionConfiguration_obj, "mType") : DEFAULT_COLLISION_CONFIGURATION;
	btCollisionConfiguration *config = NULL;
	switch (type) {
	case DEFAULT_COLLISION_CONFIGURATION:
#if LOCAL_DEBUG
		LOGV("createCollisionConfiguration:DEFAULT_COLLISION_CONFIGURATION");
#endif
		config = new btDefaultCollisionConfiguration();
		break;
	case SOFTBODY_RIGIDBODY_COLLISION_CONFIGURATION:
#if LOCAL_DEBUG
		LOGV("createCollisionConfiguration:SOFTBODY_RIGIDBODY_COLLISION_CONFIGURATION");
#endif
		config = new btSoftBodyRigidBodyCollisionConfiguration();
		break;
	default:
		LOGE("createCollisionConfiguration:CollisionConfiguration type %d not supported", type);
		config = new btDefaultCollisionConfiguration();
		break;
	}
	return config;
}

/**
 * create btBroadphaseInterface object using specified Broadphase object</br>
 * if no Broadphase object is specified(that means broadphase_obj==NULL),
 * create btDbvtBroadphase object
 * @param env
 * @param broadphase_obj
 */
static btBroadphaseInterface *createBroadphase(
		JNIEnv *env, jobject broadphase_obj) {

	if (UNLIKELY(!broadphase_obj)) {
		LOGE("createBroadphase:broadPhase field is null");
	}
	jint broadphaseType = broadphase_obj ? getField_int(env, broadphase_obj, "mType") : BROADPHASE_DBVT;
	btBroadphaseInterface* broadphase = NULL;
	switch (broadphaseType) {
	case BROADPHASE_SIMPLE:		// 0
#if LOCAL_DEBUG
		LOGV("createBroadphase:BROADPHASE_SIMPLE");
#endif
		broadphase = new btSimpleBroadphase();
		break;
	case BROADPHASE_AXISSWEEP3:		// 1
	{
#if LOCAL_DEBUG
		LOGV("createBroadphase:BROADPHASE_AXISSWEEP3");
#endif
		int maxproxies = getField_int(env, broadphase_obj, "mMaxProxies");
		btVector3 *min = getField_vec(env, broadphase_obj, "mWorldAabbMin");
		btVector3 *max = getField_vec(env, broadphase_obj, "mWorldAabbMax");

		if ((maxproxies > 1) && (maxproxies < 32767)) {
			broadphase = new btAxisSweep3(*min, *max, maxproxies);
		} else {
			broadphase =  new btAxisSweep3(*min, *max);
		}
		break;
	}
	case BROADPHASE_32AXISSWEEP3:	// 2
	{
#if LOCAL_DEBUG
		LOGV("createBroadphase:BROADPHASE_32AXISSWEEP3");
#endif
		int maxproxies = getField_int(env, broadphase_obj, "mMaxProxies");
		btVector3 *min = getField_vec(env, broadphase_obj, "mWorldAabbMin");
		btVector3 *max = getField_vec(env, broadphase_obj, "mWorldAabbMax");

		if ((maxproxies > 1) && (maxproxies < 2147483647)) {
			broadphase = new bt32BitAxisSweep3(*min, *max, maxproxies);
		} else {
			broadphase = new bt32BitAxisSweep3(*min, *max);
		}
		break;
	}
	case BROADPHASE_DBVT:			// 3
#if LOCAL_DEBUG
		LOGV("createBroadphase:BROADPHASE_DBVT");
#endif
		broadphase = new btDbvtBroadphase();
		break;
//	case BROADPHASE_MULTISAP:		// 4
//		break;
//	case BROADPHASE_CUDA:			// 5
//		break;
	default:
		LOGE("createBroadphase:broadphase type %d not supported", broadphaseType);
		broadphase = new btDbvtBroadphase();
		break;
	}
	return broadphase;
}

/**
 * create btConstraintSolver using Solver object</br>
 * if not Solver object is specified(solver_obj==null),
 * create btSequentialImpulseConstraintSolver object
 * @param env
 * @param solver_obj
 * @param ctrlBlock
 */
static btConstraintSolver *createConstraintSolver(
		JNIEnv *env, jobject solver_obj, PhysicsWorldCtrlBlock* ctrlBlock) {

	if (UNLIKELY(!solver_obj)) {
		LOGE("createConstraintSolver:solver field is null");
	}
	jint solverType = solver_obj ? getField_int(env, solver_obj, "mType") : SEQUENTIAL_IMPULSE_CONSTRAINT_SOLVER;
	btConstraintSolver *solver = NULL;
	switch (solverType) {
	case SEQUENTIAL_IMPULSE_CONSTRAINT_SOLVER:	// 1
#if LOCAL_DEBUG
		LOGV("createConstraintSolver:SEQUENTIAL_IMPULSE_CONSTRAINT_SOLVER");
#endif
		solver = new btSequentialImpulseConstraintSolver();
		break;
/*	case VORONOI_SIMPLEX_SOLVER:				// 2
		// FIXME btVoronoiSimplexSolver is not a child class of btConstraintSolver
		// and can not enter into solver
		solver = new btVoronoiSimplexSolver();
		break;
	case MINKOWSKI_PENETRATION_DEPTH_SOLVER:	// 3
		// FIXME btMinkowskiPenetrationDepthSolver is not a child class of btConstraintSolver
		// and can not enter into solver
		solver = new btMinkowskiPenetrationDepthSolver();
		break;
	case GJK_EPA_PENETRATION_DEPTH_SOLVER:		// 4
		// FIXME btGjkEpaPenetrationDepthSolver is not a child class of btConstraintSolver
		// and can not enter into solver
		solver = new btGjkEpaPenetrationDepthSolver();
		break;
	case CONVEX_PENETRATION_DEPTH_SOLVER:		// 5
		// FIXME btConvexPenetrationDepthSolver is not a child class of btConstraintSolver
		// and can not enter into solver
		solver = new btConvexPenetrationDepthSolver();
		break; */
	default:
		LOGE("createBroadphase:broadphase type %d not supported", solverType);
		solver = new btSequentialImpulseConstraintSolver();
		break;
	}
	return solver;
}

/**
 * create btDynamicsWorld using specified DynamicsWorld object</br>
 * if no DynamicsWorld object is specified(dynamicsWorld_obj==null),
 * create btDiscreteDynamicsWorld object.
 * @param env
 * @param dynamicsWorld_obj
 * @param ctrlBlock
 */
static btDynamicsWorld *createDynamicsWorld(
		JNIEnv *env, jobject dynamicsWorld_obj, PhysicsWorldCtrlBlock *ctrlBlock) {

	if (UNLIKELY(!dynamicsWorld_obj)) {
		LOGE("createDynamicsWorld:dynamicsWorld field is null");
	}
	jint dynamicsworldType = dynamicsWorld_obj ? getField_int(env, dynamicsWorld_obj, "mType") : BT_DISCRETE_DYNAMICS_WORLD;

	ctrlBlock->softBodyWorldInfo = NULL;
	btDynamicsWorld* dynamicsworld = NULL;
	switch (dynamicsworldType) {
	case BT_SIMPLE_DYNAMICS_WORLD:		// 1
#if LOCAL_DEBUG
		LOGV("createDynamicsWorld:BT_SIMPLE_DYNAMICS_WORLD");
#endif
		dynamicsworld = new btSimpleDynamicsWorld(
			ctrlBlock->dispatcher,
			ctrlBlock->broadphase,
			ctrlBlock->constraintSolver,
			ctrlBlock->collisionConfiguration);
		break;
	case BT_DISCRETE_DYNAMICS_WORLD:	// 2
#if LOCAL_DEBUG
		LOGV("createDynamicsWorld:BT_DISCRETE_DYNAMICS_WORLD");
#endif
		dynamicsworld = new btDiscreteDynamicsWorld(
			ctrlBlock->dispatcher,
			ctrlBlock->broadphase,
			ctrlBlock->constraintSolver,
			ctrlBlock->collisionConfiguration );
		break;
//	case BT_CONTINUOUS_DYNAMICS_WORLD:	// 3
		// this constant is still remain, but its definision is already removed.
	case BT_SOFT_RIGID_DYNAMICS_WORLD:	// 4
#if LOCAL_DEBUG
		LOGV("createDynamicsWorld:BT_SOFT_RIGID_DYNAMICS_WORLD");
#endif
		ctrlBlock->softBodyWorldInfo = new btSoftBodyWorldInfo();
		ctrlBlock->softBodyWorldInfo->m_broadphase = ctrlBlock->broadphase;
		ctrlBlock->softBodyWorldInfo->m_dispatcher = ctrlBlock->dispatcher;
		dynamicsworld = new btSoftRigidDynamicsWorld(
			ctrlBlock->dispatcher,
			ctrlBlock->broadphase,
			ctrlBlock->constraintSolver,
			ctrlBlock->collisionConfiguration,
			NULL);	// if pass NULL to this element, btDefaultSoftBodySolver is automaticalliy created in btSoftRigidDynamicsWorld
		break;
	default:
		LOGE("createDynamicsWorld:dynamicsworld type %d not supported", dynamicsworldType);
		dynamicsworld = new btDiscreteDynamicsWorld(
			ctrlBlock->dispatcher,
			ctrlBlock->broadphase,
			ctrlBlock->constraintSolver,
			ctrlBlock->collisionConfiguration);
		break;
	}
	return dynamicsworld;
}

void clearObjects(PhysicsWorldCtrlBlock *ctrlBlock) {

	btDynamicsWorld *pDynamicsWorld = ctrlBlock->dynamicsWorld;

#if LOCAL_DEBUG
	LOGV("clearObjects:");
#endif
	// delete as reverse order of creation
	// delete constraint
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:remove/delete constraint");
#endif
	for (int i = pDynamicsWorld->getNumConstraints() - 1; i >= 0 ;i--) {
		btTypedConstraint *constraint = pDynamicsWorld->getConstraint(i);
		pDynamicsWorld->removeConstraint(constraint);
		SAFE_DELETE(constraint);
	}
	// remove the rigidbodies/softbodies from the dynamics world and delete
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:remove/delete rigidbodies");
#endif
	for (int i = pDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject *obj = pDynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody *rigidBody = btRigidBody::upcast(obj);
		if (rigidBody) {	// rigidBody?
			// delete MotionState
			btMotionState *motionState = rigidBody->getMotionState();
			if (motionState) {
				SAFE_DELETE(motionState);
			}
			ctrlBlock->dynamicsWorld->removeRigidBody(rigidBody);
		} else {
			btSoftBody *softBody = btSoftBody::upcast(obj);
			if (softBody) {
				((btSoftRigidDynamicsWorld *)ctrlBlock->dynamicsWorld)->removeSoftBody(softBody);
			} else {
				pDynamicsWorld->removeCollisionObject(obj);
			}
		}
		SAFE_DELETE(obj);
	}
}

void destroyPhysicsWorld(PhysicsWorldCtrlBlock *ctrlBlock) {

#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:");
#endif
	// delete as reverse order of creation
	// delete all physics world objects
	clearObjects(ctrlBlock);
	// delete bulletWorldDraw
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:bulletWorldDraw");
#endif
	SAFE_DELETE(ctrlBlock->bulletWorldDraw);
	// delete dynamicsWorld
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:dynamicsWorld");
#endif
	SAFE_DELETE(ctrlBlock->dynamicsWorld);
	// delete softBodyWorldInfo
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:softBodyWorldInfo");
#endif
	SAFE_DELETE(ctrlBlock->softBodyWorldInfo);
	// delete constraintSolver
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:constraintSolver");
#endif
	SAFE_DELETE(ctrlBlock->constraintSolver);
	// delete pairCache
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:pairCache");
#endif
	SAFE_DELETE(ctrlBlock->broadphase);
	// delete dispatcher
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:dispatcher");
#endif
	SAFE_DELETE(ctrlBlock->dispatcher);
	// delete collisionConfiguration
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:collisionConfiguration");
#endif
	SAFE_DELETE(ctrlBlock->collisionConfiguration);
	// delete control block
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:remove");
#endif
	gPhysicsWorldCtrlBlocks.remove(ctrlBlock);
#if LOCAL_DEBUG
	LOGV("destroyPhysicsWorld:ctrlBlock");
#endif
	SAFE_DELETE(ctrlBlock);
}

/**
 * write back simulation values into Java object after simulation
 */
static jint postSimulation(JNIEnv *env, jobject thiz,
		PhysicsWorldCtrlBlock *ctrlBlock) {

	if (ctrlBlock->softBodyWorldInfo) {
		ctrlBlock->softBodyWorldInfo->m_sparsesdf.GarbageCollect();
	}
	jint coll_num = ctrlBlock->dynamicsWorld->getNumCollisionObjects();
	if (ctrlBlock->needJavaResult) {	// need to write back the result?
		btCollisionObject *obj;
		btRigidBody *body;
		btCollisionShape *shape;

		btSphereShape *sphere;
		btStaticPlaneShape *plane;
		btBoxShape *box;
		btCylinderShape *cylinder;
		btConeShape *cone;
		btCapsuleShape *capsule;
		btBU_Simplex1to4 *tetrahedral;

		btTransform trans;
		btMatrix3x3 rot;
		btVector3 pos;
		int shapeType;

		jobject rigidBody_obj;
		jobject option_params_obj;
		jfloatArray *option_params;

		jfloat *options;

		for (int i = coll_num - 1; i >= 0; i--) {
#if LOCAL_DEBUG
//			LOGV("postSimulation:loop %d", i);
#endif
			obj = ctrlBlock->dynamicsWorld->getCollisionObjectArray()[i];
			body = btRigidBody::upcast(obj);

			if (body && body->getMotionState()) {	// moving rigidBody?
				// search RigidBody object(Java) with calling BulletManager#getRigidBody(long id);
				rigidBody_obj = get_rigidbody(env, thiz, reinterpret_cast<ID_RIGIDBODY>(body));
				if (UNLIKELY(!rigidBody_obj)) {
					LOGE("postSimulation:rigidBody not found");
					continue;
				}
				// get array as a Object
				option_params_obj = getField_obj(env, rigidBody_obj, "option_params", "[F");
				if (UNLIKELY(!option_params_obj)) {
					LOGE("doSimulationNative:rigidBody has no option_params field");
					continue;
				}
				// convert the Object to jfloatArray
				option_params = reinterpret_cast<jfloatArray *>( &option_params_obj );
				// get as jfloat array
				options = env->GetFloatArrayElements(*option_params, NULL);
				if (UNLIKELY(!options)) {
					LOGE("doSimulationNative:can not GetFloatArrayElements from option_params");
					continue;
				}

				if (UNLIKELY(isNullField_mat3x3(env, rigidBody_obj, "mBasis"))) {
					LOGE("doSimulationNative:rigidBody has no mBasis field");
					continue;
				}
				if (UNLIKELY(isNullField_vec(env, rigidBody_obj, "mOrigin"))) {
					LOGE("doSimulationNative:rigidBody has no mOrigin field");
					continue;
				}

				shape = body->getCollisionShape();
				shapeType = shape->getShapeType();

				trans = body->getWorldTransform();

				// set matrix
				btMatrix3x3 *mat = getField_mat(env, rigidBody_obj, "mBasis");
				*mat = trans.getBasis();				// get matrix

				// set position
				btVector3 *vec = getField_vec(env, rigidBody_obj, "mOrigin");
				*vec = trans.getOrigin();			// get position vector

				// set additional parameters depending on the shape type
				switch (shapeType) {
					case STATIC_PLANE_PROXYTYPE:
#if LOCAL_DEBUG
//						LOGV("doSimulationNative:STATIC_PLANE_PROXYTYPE");
#endif
						break;

					case BOX_SHAPE_PROXYTYPE:
#if LOCAL_DEBUG
//						LOGV("in BOX_PROXYTYPE.");
#endif
						box = static_cast<btBoxShape*>(shape);
						options[0] = (jfloat)box->getHalfExtentsWithoutMargin().x();
						options[1] = (jfloat)box->getHalfExtentsWithoutMargin().y();
						options[2] = (jfloat)box->getHalfExtentsWithoutMargin().z();
						break;

					case CAPSULE_SHAPE_PROXYTYPE:
#if LOCAL_DEBUG
//						LOGV("doSimulationNative:CAPSULE_SHAPE_PROXYTYPE");
#endif
						capsule = static_cast<btCapsuleShape*>(shape);
						options[0] = (jfloat)capsule->getRadius();
						options[1] = (jfloat)capsule->getHalfHeight();
						break;

					case CONE_SHAPE_PROXYTYPE:
#if LOCAL_DEBUG
//						LOGV("doSimulationNative:CONE_SHAPE_PROXYTYPE");
#endif
						cone = static_cast<btConeShape*>(shape);
						options[0] = (jfloat)cone->getRadius();
						options[1] = (jfloat)cone->getHeight();
						break;

					case CYLINDER_SHAPE_PROXYTYPE:
#if LOCAL_DEBUG
//						LOGV("doSimulationNative:CYLINDER_SHAPE_PROXYTYPE");
#endif
						cylinder = static_cast<btCylinderShape*>(shape);
						options[0] = (jfloat)cylinder->getHalfExtentsWithoutMargin().x();
						options[1] = (jfloat)cylinder->getHalfExtentsWithoutMargin().y();
						options[2] = (jfloat)cylinder->getHalfExtentsWithoutMargin().z();
						options[3] = (jfloat)cylinder->getRadius();
						break;

					case EMPTY_SHAPE_PROXYTYPE:
						break;

					case SPHERE_SHAPE_PROXYTYPE:
#if LOCAL_DEBUG
//						LOGV("doSimulationNative:SPHERE_SHAPE_PROXYTYPE");
#endif
						sphere = static_cast<btSphereShape*>(shape);
						options[0] = (jfloat)sphere->getRadius();
						break;

					case TETRAHEDRAL_SHAPE_PROXYTYPE:
#if LOCAL_DEBUG
//						LOGV("doSimulationNative:TETRAHEDRAL_SHAPE_PROXYTYPE");
#endif
						tetrahedral = static_cast<btBU_Simplex1to4*>(shape);
						break;

					case TRIANGLE_SHAPE_PROXYTYPE:
						break;

					default:
						LOGE("shapeType %d not found", shapeType);
						return -1010;
				}
				// write back the optional values into Java object
				env->SetFloatArrayRegion(*option_params, 0, sizeof(options)-1, options);
				// release jfloat array
				env->ReleaseFloatArrayElements(*option_params, options, ARRAYELEMENTS_COPYBACK_AND_RELEASE);
	#ifdef ANDROID_NDK
				env->DeleteLocalRef(rigidBody_obj);
				env->DeleteLocalRef(option_params_obj);
	#endif
			}
		}
	}
#if LOCAL_DEBUG
//	LOGV("postSimulation:coll_num=%d", coll_num);
#endif

    return coll_num;	// return number of objects
}

//**********************************************************************
//
//**********************************************************************
static ID_CTRLBLOCK nativeCreate(JNIEnv *env, jobject thiz) {

#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:");
#endif
	// create control block
	PhysicsWorldCtrlBlock *ctrlBlock = new PhysicsWorldCtrlBlock;
	// create configuration object
	jobject collisionConfiguration_obj = getField_obj(env, thiz, "collisionConfiguration", JTYPE_COLLISIONCONFIG);
	ctrlBlock->collisionConfiguration = createCollisionConfiguration(env, collisionConfiguration_obj);
#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:collisionConfiguration.");
#endif
	// create narrowphase collision detection
	// jobject collisionDispatcher_obj = getField_obj(env, physicsWorld_obj, "collisionDispatcher", JTYPE_COLLISIONDISPATCHER);
	// XXX basically we should change the type of CollisionDispatcher according to the collisionDispatcher_obj,
	// but current Bullet library has only one CollisionDispatcher and we directory create it.
	ctrlBlock->dispatcher = new btCollisionDispatcher(ctrlBlock->collisionConfiguration);
#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:dispatcher.");
#endif

	// create broadphase collision detection
	jobject broadphase_obj = getField_obj(env, thiz, "broadPhase", JTYPE_BROADPHASE);
	ctrlBlock->broadphase = createBroadphase(env, broadphase_obj);
#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:broadphase");
#endif
	// create rigid body constraint solver
	jobject solver_obj = getField_obj(env, thiz, "solver", JTYPE_SOLVER);
	ctrlBlock->constraintSolver = createConstraintSolver(env, solver_obj, ctrlBlock);
#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:constraintSolver");
#endif
	// create dynamicsworld
	jobject dynamicsWorld_obj = getField_obj(env, thiz, "dynamicsWorld", JTYPE_DYNAMICSWORLD);
	ctrlBlock->dynamicsWorld = createDynamicsWorld(env, dynamicsWorld_obj, ctrlBlock);
#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:dynamicsWorld");
#endif
	// if gravity is specified, set it to the world
	if (LIKELY(!isNullField_vec(env, thiz, "gravity"))) {
		btVector3 *gravity = getField_vec(env, thiz, "gravity");
		ctrlBlock->dynamicsWorld->setGravity(*gravity);
		if (ctrlBlock->softBodyWorldInfo) {
			ctrlBlock->softBodyWorldInfo->m_gravity.setValue(gravity->x(), gravity->y(), gravity->z());
		}
#if LOCAL_DEBUG
		LOGV("PhysicsWorld_nativeCreate:setGravity");
#endif
	}
	if (ctrlBlock->softBodyWorldInfo) {
		ctrlBlock->softBodyWorldInfo->m_sparsesdf.Initialize();
	}

	// create drawing engine
	ctrlBlock->bulletWorldDraw = new BulletWorldDraw(ctrlBlock->dynamicsWorld, ctrlBlock->softBodyWorldInfo);
#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:BulletWorldDraw");
#endif

	getDeltaTimeMicroseconds(ctrlBlock->prev_time);

#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:setup using BulletManager");
#endif
	jobject bulletmanager_obj = getField_obj(env, thiz, "mBulletManager", JTYPE_BULLETMANAGER);
	if (LIKELY(bulletmanager_obj)) {
		ctrlBlock->needJavaResult = getField_bool(env, bulletmanager_obj, "mNeedJavaResult");
	} else {
		ctrlBlock->needJavaResult = true;	// XXX
	}
	gPhysicsWorldCtrlBlocks.push_back(ctrlBlock);

#if LOCAL_DEBUG
	LOGV("PhysicsWorld_nativeCreate:fin");
#endif
	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_CTRLBLOCK>(ctrlBlock);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_CTRLBLOCK id_ctrlblock) {

#if LOCAL_DEBUG
	LOGD("PhysicsWorld#nativeDestroy:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!gPhysicsWorldCtrlBlocks.get(ctrlBlock))) {
		LOGE("destroyPhysicsWorld:bullet control block not found");
	}

	destroyPhysicsWorld(ctrlBlock);
}

static jint nativeDoSimulation(JNIEnv* env, jobject thiz,
		ID_CTRLBLOCK id_ctrlblock,
		jfloat exec_time,
		jint subSteps) {

#if LOCAL_DEBUG
//	LOGV("doSimulationNative:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (LIKELY(ctrlBlock)) {
		ctrlBlock->dynamicsWorld->stepSimulation(exec_time, subSteps);
		return postSimulation(env, thiz, ctrlBlock);
	} else {
		LOGE("doSimulationNative:bullet control block not found");
		return -1000;
	}
}

static jint nativeStepSimulation(JNIEnv *env, jobject thiz,
		ID_CTRLBLOCK id_ctrlblock) {

#if LOCAL_DEBUG
//	LOGV("stepSimulationNative:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (LIKELY(ctrlBlock)) {
		// simple dynamics world doesn't handle fixed-time-stepping
		float us = getDeltaTimeMicroseconds(ctrlBlock->prev_time);
		if (us > MIN_FPS) us = MIN_FPS;
		ctrlBlock->dynamicsWorld->stepSimulation(us / 1000000.f);
		return postSimulation(env, thiz, ctrlBlock);
	} else {
		LOGE("stepSimulationNative:bullet control block not found");
		return -1000;
	}
}

static jint nativeClearObjects(JNIEnv *env, jobject thiz,
		ID_CTRLBLOCK id_ctrlblock) {

	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("destroyPhysicsWorld:bullet control block not found");
		return -1000;
	}
	clearObjects(ctrlBlock);
	return JNI_OK;
}

static jint nativeSetActiveAll(JNIEnv *env, jobject thiz,
		ID_CTRLBLOCK id_ctrlblock,
		jboolean isActive) {

#if LOCAL_DEBUG
	LOGV("setActiveAll:");
#endif

	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("destroyPhysicsWorld:bullet control block not found");
		return -1000;
	}

	btCollisionObject *obj;
	btRigidBody *body;

	int coll_obj = ctrlBlock->dynamicsWorld->getNumCollisionObjects();
	for (int i = coll_obj - 1; i >= 0; i--){
		obj = ctrlBlock->dynamicsWorld->getCollisionObjectArray()[i];
		body = btRigidBody::upcast(obj);
		body->activate(isActive);
	}
	return JNI_OK;
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "()J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeDoSimulation", "(JFI)I", (void *)nativeDoSimulation},
	{"nativeStepSimulation", "(J)I", (void *)nativeStepSimulation},
	{"nativeClearObjects", "(J)I", (void *)nativeClearObjects},
	{"nativeSetActiveAll", "(JZ)I", (void *)nativeSetActiveAll},
};

int register_PhysicsWorld(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/PhysicsWorld",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
