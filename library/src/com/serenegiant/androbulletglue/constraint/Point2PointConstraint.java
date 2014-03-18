/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Point2PointConstraint.java
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

package com.serenegiant.androbulletglue.constraint;

import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.interfaces.Constraint;

public class Point2PointConstraint extends Constraint {
	
	public Point2PointConstraint(PhysicsWorld world,
		RigidBody rbA, Vector3 pivotInA) {
		
		this(world, rbA, null, pivotInA, null, false);
	}

	public Point2PointConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB, Vector3 pivotInA, Vector3 pivotInB,
		boolean disableCollisionsBetweenLinkedBodies) {
		
		super(world, POINT2POINT_CONSTRAINT_TYPE);
		this.mRbA = rbA;
		this.mRbB = rbB;
		
		mID = nativeCreate(
			pivotInA != null ? pivotInA.getID() : 0,
			pivotInB != null ? pivotInB.getID() : 0);
		addConstraint(disableCollisionsBetweenLinkedBodies);
	}
	private final native long nativeCreate(long id_vec_pivotInA, long id_vec_pivotInB);
	
	public void setBreakingImpulseThreshold(float threshold) {
		nativeSetBreakingImpulseThreshold(mID, threshold);
	}
	private static final native int nativeSetBreakingImpulseThreshold(long id_constraint, float threshold);

}
