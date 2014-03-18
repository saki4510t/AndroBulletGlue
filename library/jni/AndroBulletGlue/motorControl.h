/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: motorControl.h
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

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

#include "androbulletglue.h"
#include "BulletSoftBody/btSoftBody.h"

typedef struct MotorControl : btSoftBody::AJoint::IControl {
	MotorControl() {
		goal = 0.f;
		maxtorque = 0.f;
	}
	virtual ~MotorControl() {}
	btScalar Speed(btSoftBody::AJoint *joint, btScalar current) {
		return current + btMin(maxtorque, btMax(-maxtorque, goal - current));
	}
	btScalar	goal;
	btScalar	maxtorque;
} *PMotorControl;

#endif /* MOTORCONTROL_H_ */
