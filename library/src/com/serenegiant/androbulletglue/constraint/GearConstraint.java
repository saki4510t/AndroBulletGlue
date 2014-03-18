/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: GearConstraint.java
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

public class GearConstraint extends Constraint {
	
/*	public final Vector3 mAxisInA;
	public final Vector3 mAxisInB;
	public final float mRatio; */
	
	public GearConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Vector3 axisInA, Vector3 axisInB, float ratio) {
		
		this(world, rbA, rbB, axisInA, axisInB, ratio, false);
	}
	
	public GearConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Vector3 axisInA, Vector3 axisInB, float ratio,
		boolean disableCollisionsBetweenLinkedBodies) {
		
		super(world, GEAR_CONSTRAINT_TYPE);
		mRbA = rbA;
		mRbB = rbB;
/*		mAxisInA = axisInA;
		mAxisInB = axisInB;
		mRatio = ratio; */
		mID = nativeCreate(rbA.getID(), rbB.getID(),
			axisInA.getID(), axisInB.getID(), ratio);
		addConstraint(disableCollisionsBetweenLinkedBodies);
	}
	private final native long nativeCreate(long id_rbA, long id_rbB,
		long id_vec_axisInA, long id_vec_axisInB, float ratio);
}
