/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: HingeConstraint.java
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
import com.serenegiant.androbulletglue.math.Transform;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.interfaces.Constraint;

public class HingeConstraint extends Constraint {
	
	public boolean mUseReferenceFrameA;
	
	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, Vector3 pivotInA, Vector3 axisInA) {
		
		this(world, rbA, null, pivotInA, null, axisInA, null, false, false);
	}

	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, Vector3 pivotInA, Vector3 axisInA,
		boolean useReferenceFrameA) {
		
		this(world, rbA, null, pivotInA, null, axisInA, null, useReferenceFrameA, false);
	}

	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, Vector3 pivotInA, Vector3 axisInA,
		boolean useReferenceFrameA,
		boolean disableCollisionsBetweenLinkedBodies) {
		
		this(world, rbA, null, pivotInA, null, axisInA, null, useReferenceFrameA,
				disableCollisionsBetweenLinkedBodies);
	}

	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Vector3 pivotInA, Vector3 pivotInB,
		Vector3 axisInA, Vector3 axisInB) {
			
		this(world, rbA, rbB, pivotInA, pivotInB,
				axisInA, axisInB, false, false);
	}

	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Vector3 pivotInA, Vector3 pivotInB,
		Vector3 axisInA, Vector3 axisInB,
		boolean useReferenceFrameA) {
		
		this(world, rbA, rbB, pivotInA, pivotInB,
				axisInA, axisInB, useReferenceFrameA, false);
	}
	
	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Vector3 pivotInA, Vector3 pivotInB,
		Vector3 axisInA, Vector3 axisInB,
		boolean useReferenceFrameA,
		boolean disableCollisionsBetweenLinkedBodies) {
		
		super(world, HINGE_CONSTRAINT_TYPE);
		mRbA = rbA;
		mRbB = rbB;
		mUseReferenceFrameA = useReferenceFrameA;
		mID = nativeCreate(mPhysicsWorldID,
			pivotInA != null ? pivotInA.getID() : 0,
			pivotInB != null ? pivotInB.getID() : 0,
			axisInA != null ? axisInA.getID() : 0,
			axisInB != null ? axisInB.getID() : 0);	
		addConstraint(disableCollisionsBetweenLinkedBodies);
	}
	
	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Transform frameA, Transform frameB) {
		
		this(world, rbA, rbB, frameA, frameB, false, false);
	}
	
	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Transform frameA, Transform frameB,
		boolean useReferenceFrameA) {
		
		this(world, rbA, rbB, frameA, frameB, useReferenceFrameA, false);
	}

	public HingeConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Transform frameA, Transform frameB,
		boolean useReferenceFrameA,
		boolean disableCollisionsBetweenLinkedBodies) {
		
		super(world, HINGE_CONSTRAINT_TYPE);
		mRbA = rbA;
		mRbB = rbB;
		mFrameA.set(frameA);
		mFrameB.set(frameB);
		mUseReferenceFrameA = useReferenceFrameA;
		createConstraint(frameA, frameB, useReferenceFrameA, disableCollisionsBetweenLinkedBodies);	
	}
	
	private final native long nativeCreate(
			long id_physicsworld,
			long id_pivotInA, long id_pivotInB,
			long id_axisInA, long id_axisInB);

	/**
	 * set limit range that can move
	 * @param low
	 * @param high
	 */
	public void setLimit(float low, float high) {
		nativeSetLimit(mID, low, high);
	}
	private static final native int nativeSetLimit(long id_constraint, float low, float high);

	/**
	 * @param enable
	 * @param targetVelocity
	 * @param maxMotorImpulse
	 */
	public void enableAngularMotor(boolean enable, float targetVelocity, float maxMotorImpulse) {
		nativeEnableAngularMotor(mID, enable, targetVelocity, maxMotorImpulse);
	}
	private static final native int nativeEnableAngularMotor(long id_constraint, boolean enable, float targetVelocity, float maxMotorImpulse);
}
