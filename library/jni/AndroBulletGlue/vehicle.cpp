/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: vehicle.cpp
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


#include "vehicle.h"

#include "physicsWorld.h"

#define LOCAL_DEBUG 0

// btRaycastVehicle is the interface for the constraint that implements the raycast vehicle
// notice that for higher-quality slow-moving vehicles, another approach might be better
// implementing explicit hinged-wheel constraints with cylinder collision, rather then raycasts

// these values are overrided with field values of VehicleBody object.
float gEngineForce = 0.f;
float gBreakingForce = 0.f;
float gMaxEngineForce = 1000.f;	// this should be engine/velocity dependent
float gMaxBreakingForce = 100.f;

float gVehicleSteering = 0.f;
float gSteeringIncrement = 0.04f;
float gSteeringClamp = 0.3f;
float gWheelRadius = 0.5f;
float gWheelWidth = 0.4f;
float gWheelFriction = 1000;	// BT_LARGE_FLOAT;
float gSuspensionStiffness = 20.f;
float gSuspensionDamping = 2.3f;
float gSuspensionCompression = 4.4f;
float gSuspensionRestLength = 0.6f;
float gRollInfluence = 0.1f;	// 1.0f;

#define CUBE_HALF_EXTENTS 0.92

static void internalClientResetScene(
	btDynamicsWorld *dynamicsWorld, btRigidBody *carChassis, btRaycastVehicle *vehicle) {
#if LOCAL_DEBUG
	LOGD("internalClientResetScene");
#endif
	gVehicleSteering = 0.f;
	carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	carChassis->setLinearVelocity(btVector3(0, 0, 0));
	carChassis->setAngularVelocity(btVector3(0, 0, 0));
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(carChassis->getBroadphaseHandle(), dynamicsWorld->getDispatcher());
	if (LIKELY(vehicle)) {
		vehicle->resetSuspension();
		for (int i = 0; i < vehicle->getNumWheels(); i++) {
			//synchronize the wheels with the (interpolated) chassis worldtransform
			vehicle->updateWheelTransform(i, true);
		}
	}
}

VehicleBody::VehicleBody(btDynamicsWorld *dynamicsWorld, btRigidBody *carChassis, btVehicleRaycaster *rayCaster, bool force_zaxis_up) :
m_dynamicsWorld(dynamicsWorld),
m_carChassis(carChassis),
m_vehicleRayCaster(rayCaster)
{
#if LOCAL_DEBUG
	LOGD("VehicleBody:constructor");
#endif
	m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);

	// never deactivate the vehicle
	m_carChassis->setActivationState(DISABLE_DEACTIVATION);

	m_dynamicsWorld->addVehicle(m_vehicle);

	int rightIndex;
	int upIndex;
	int forwardIndex;
	btVector3 wheelDirectionCS0;
	btVector3 wheelAxleCS;
	if (force_zaxis_up) {
		rightIndex = 0;
		upIndex = 2;
		forwardIndex = 1;
		wheelDirectionCS0.setValue(0, 0, -1);
		wheelAxleCS.setValue(1, 0, 0);
	} else {
		rightIndex = 0;
		upIndex = 1;
		forwardIndex = 2;
		wheelDirectionCS0.setValue(0, -1, 0);
		wheelAxleCS.setValue(-1, 0, 0);
	}

	float connectionHeight = 1.2f;


	bool isFrontWheel = true;

	//choose coordinate system
	m_vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

	btVector3 connectionPointCS0;
	if (force_zaxis_up) {
		connectionPointCS0.setValue(
			CUBE_HALF_EXTENTS - (0.3 * gWheelWidth), 2 * CUBE_HALF_EXTENTS - gWheelRadius, connectionHeight);
	} else {
		connectionPointCS0.setValue(
			CUBE_HALF_EXTENTS - (0.3 * gWheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - gWheelRadius);
	}

	m_vehicle->addWheel(
		connectionPointCS0, wheelDirectionCS0, wheelAxleCS,
		gSuspensionRestLength, gWheelRadius, m_tuning, isFrontWheel);

	if (force_zaxis_up) {
		connectionPointCS0 = btVector3(
			-CUBE_HALF_EXTENTS + (0.3 * gWheelWidth), 2 * CUBE_HALF_EXTENTS - gWheelRadius, connectionHeight);
	} else {
		connectionPointCS0 = btVector3(
			-CUBE_HALF_EXTENTS + (0.3 * gWheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - gWheelRadius);
	}

	m_vehicle->addWheel(
		connectionPointCS0, wheelDirectionCS0, wheelAxleCS,
		gSuspensionRestLength, gWheelRadius, m_tuning, isFrontWheel);

	if (force_zaxis_up) {
		connectionPointCS0 = btVector3(
			-CUBE_HALF_EXTENTS + (0.3 * gWheelWidth), -2 * CUBE_HALF_EXTENTS + gWheelRadius, connectionHeight);
	} else {
		connectionPointCS0 = btVector3(
			-CUBE_HALF_EXTENTS + (0.3 * gWheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + gWheelRadius);
	}

	isFrontWheel = false;
	m_vehicle->addWheel(
		connectionPointCS0, wheelDirectionCS0, wheelAxleCS,
		gSuspensionRestLength, gWheelRadius, m_tuning, isFrontWheel);

	if (force_zaxis_up) {
		connectionPointCS0 = btVector3(
			CUBE_HALF_EXTENTS - (0.3 * gWheelWidth), -2 * CUBE_HALF_EXTENTS + gWheelRadius, connectionHeight);
	} else {
		connectionPointCS0 = btVector3(
			CUBE_HALF_EXTENTS - (0.3 * gWheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + gWheelRadius);
	}

	m_vehicle->addWheel(
		connectionPointCS0, wheelDirectionCS0, wheelAxleCS,
		gSuspensionRestLength, gWheelRadius, m_tuning, isFrontWheel);

	for (int i = 0; i < m_vehicle->getNumWheels(); i++) {
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = gSuspensionStiffness;
		wheel.m_wheelsDampingRelaxation = gSuspensionDamping;
		wheel.m_wheelsDampingCompression = gSuspensionCompression;
		wheel.m_frictionSlip = gWheelFriction;
		wheel.m_rollInfluence = gRollInfluence;
	}
}

VehicleBody::~VehicleBody() {
#if LOCAL_DEBUG
	LOGD("VehicleBody:destructor");
#endif
	m_dynamicsWorld->removeVehicle(m_vehicle);
	SAFE_DELETE(m_vehicle);
	SAFE_DELETE(m_vehicleRayCaster);

}

void VehicleBody::clientResetScene(void) {
	internalClientResetScene(m_dynamicsWorld, m_carChassis, m_vehicle);
}

//********************************************************************************

//********************************************************************************
static void syncVehicleParams(JNIEnv *env, jobject vehicle_obj) {
#if LOCAL_DEBUG
	LOGD("syncVehicleParams");
#endif
	gEngineForce = getStaticField_float(env, vehicle_obj, "gEngineForce");
	gBreakingForce = getStaticField_float(env, vehicle_obj, "gBreakingForce");

	gMaxEngineForce = getStaticField_float(env, vehicle_obj, "gMaxEngineForce");
	gMaxBreakingForce = getStaticField_float(env, vehicle_obj, "gMaxBreakingForce");

	gVehicleSteering = getStaticField_float(env, vehicle_obj, "gVehicleSteering");
	gSteeringIncrement = getStaticField_float(env, vehicle_obj, "gSteeringIncrement");
	gSteeringClamp = getStaticField_float(env, vehicle_obj, "gSteeringClamp");
	gWheelRadius = getStaticField_float(env, vehicle_obj, "gWheelRadius");
	gWheelWidth = getStaticField_float(env, vehicle_obj, "gWheelWidth");
	gWheelFriction = getStaticField_float(env, vehicle_obj, "gWheelFriction");
	gSuspensionStiffness = getStaticField_float(env, vehicle_obj, "gSuspensionStiffness");
	gSuspensionDamping = getStaticField_float(env, vehicle_obj, "gSuspensionDamping");
	gSuspensionCompression = getStaticField_float(env, vehicle_obj, "gSuspensionCompression");
	gRollInfluence = getStaticField_float(env, vehicle_obj, "gRollInfluence");
	gSuspensionRestLength = getStaticField_float(env, vehicle_obj, "gSuspensionRestLength");
}

//**********************************************************************
//
//**********************************************************************
static ID_TYPE nativeCreate(
	JNIEnv *env, jobject thiz,	// thiz is Vehecle object(Java)
	ID_CTRLBLOCK id_ctrlblock,
	jboolean force_zaxis_up) {

#if LOCAL_DEBUG
	LOGD("nativeCreateVehicleBody:");
#endif
	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("nativeCreateVehicleBody:bullet control block not found");
		return (ID_TYPE)NULL;
	}
	// get parameters from Java object
	syncVehicleParams(env, thiz);

#if LOCAL_DEBUG
	LOGD("nativeCreateVehicleBody:get carChassis");
#endif
	btRigidBody *carChassis = reinterpret_cast<btRigidBody *>(getField_obj_id(env, thiz, "mCarChassis", JTYPE_RIGIDBODY));

	internalClientResetScene(ctrlBlock->dynamicsWorld, carChassis, NULL);

	/// create vehicle
#if LOCAL_DEBUG
	LOGD("nativeCreateVehicleBody:create vehicle");
#endif
	btVehicleRaycaster *vehicleRayCaster = new btDefaultVehicleRaycaster(ctrlBlock->dynamicsWorld);
	VehicleBody *vehicleBody = new VehicleBody(ctrlBlock->dynamicsWorld, carChassis, vehicleRayCaster, force_zaxis_up);

	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_TYPE>(vehicleBody);
}

static void nativeDestroy(
	JNIEnv *env, jobject thiz,	// thiz is Vehecle object(Java)
	ID_TYPE id_vehiclebody) {

#if LOCAL_DEBUG
	LOGD("VehicleBody#nativeDestroy:");
#endif
	VehicleBody *vehicleBody = reinterpret_cast<VehicleBody *>(id_vehiclebody);
	SAFE_DELETE(vehicleBody);
}

static void nativeRender(
	JNIEnv *env, jobject thiz,	// thiz is Vehecle object(Java)
	ID_CTRLBLOCK id_ctrlblock,
	ID_TYPE id_vehiclebody,
	ID_TYPE id_wheel_shape) {

	PhysicsWorldCtrlBlock *ctrlBlock = reinterpret_cast<PhysicsWorldCtrlBlock *>(id_ctrlblock);
	if (UNLIKELY(!ctrlBlock)) {
		LOGE("nativeRenderVehicleBody:bullet control block not found");
		return;
	}

	VehicleBody *vehicleBody = reinterpret_cast<VehicleBody *>(id_vehiclebody);
	if (UNLIKELY(!vehicleBody || !vehicleBody->m_vehicle)) {
		LOGE("nativeRenderVehicleBody:vehicleBody not found");
		return;
	}

	btCollisionShape *wheelShape = reinterpret_cast<btCollisionShape *>(id_wheel_shape);
	if (UNLIKELY(!wheelShape)) {
		LOGE("nativeRenderVehicleBody:invalid id of wheelShape");
		return;
	}

//	ctrlBlock->bulletWorldDraw->updateCamera();

	btScalar m[16];
	btVector3 wheelColor(1, 0, 0);	// XXX the wheel color is fixed red now.

	btVector3 worldBoundsMin, worldBoundsMax;
	ctrlBlock->dynamicsWorld->getBroadphase()->getBroadphaseAabb(worldBoundsMin,worldBoundsMax);

	btRaycastVehicle *vehicle = vehicleBody->m_vehicle;
	for (int i = 0; i < vehicle->getNumWheels(); i++) {
		//synchronize the wheels with the (interpolated) chassis worldtransform
		vehicle->updateWheelTransform(i, true);
		//draw wheels (cylinders)
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(m);
		ctrlBlock->bulletWorldDraw->getGLShapeDrawer()->drawOpenGL(
			m, wheelShape, wheelColor,
			ctrlBlock->bulletWorldDraw->getDebugMode(), worldBoundsMin, worldBoundsMax);
	}

}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "(JZ)J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeRender", "(JJJ)V", (void *)nativeRender},
};

int register_VehicleBody(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/VehicleBody",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
