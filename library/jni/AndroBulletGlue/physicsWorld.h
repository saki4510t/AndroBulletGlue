/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: physicsworld.h
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


#ifndef PHYSICSWORLD_H_
#define PHYSICSWORLD_H_

#include "androbulletglue.h"

#include "BulletSoftBody/btSoftBodySolvers.h"
#include "BulletSoftBody/btSoftBody.h"
#include "bulletWorldDraw.h"

typedef struct _PhysicsWorldCtrlBlock {
	btCollisionConfiguration *collisionConfiguration;	// CollisionConfiguration
	btDispatcher *dispatcher;							// narrowphase collision detection
	btBroadphaseInterface *broadphase;					// broadphase collision detection
	btConstraintSolver *constraintSolver;				// rigidbody constraint solver
	btSoftBodyWorldInfo	*softBodyWorldInfo;				// info for softbody world
	btDynamicsWorld *dynamicsWorld;						// dynamics world
	BulletWorldDraw *bulletWorldDraw;					// drawing engine
	clock_t prev_time;
	bool needJavaResult;
} PhysicsWorldCtrlBlock;

void clearObjects(PhysicsWorldCtrlBlock *ctrlBlock);
void destroyPhysicsWorld(PhysicsWorldCtrlBlock *ctrlBlock);

#endif /* PHYSICSWORLD_H_ */
