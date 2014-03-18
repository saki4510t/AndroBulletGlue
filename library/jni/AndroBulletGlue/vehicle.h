/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: vehicle.h
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

#ifndef VEHICLE_H_
#define VEHICLE_H_

class btVehicleTuning;
struct btVehicleRaycaster;
class btCollisionShape;

#include "androbulletglue.h"

#include "LinearMath/btVector3.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

class VehicleBody {
protected:
	btDynamicsWorld *m_dynamicsWorld;
public:
	btRigidBody *m_carChassis;
	btRaycastVehicle::btVehicleTuning m_tuning;
	btVehicleRaycaster *m_vehicleRayCaster;
	btRaycastVehicle *m_vehicle;
	btCollisionShape *m_wheelShape;

	VehicleBody(btDynamicsWorld *dynamicsWorld, btRigidBody *carChassis, btVehicleRaycaster *rayCaster, bool force_zaxis_up);
	~VehicleBody();
	void clientResetScene(void);
	void renderVehicle();
};

#endif /* VEHICLE_H_ */
