/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Generic6DofSpringConstraint.java
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

public class Generic6DofSpringConstraint extends Generic6DofConstraint {
	
	protected Generic6DofSpringConstraint(PhysicsWorld world, int type) {
		super(world, type);
	}
	
	public Generic6DofSpringConstraint(PhysicsWorld world,
			RigidBody rbB, Transform frameInB) {
		
		this(world, rbB, null, frameInB, null, false, false);		
	}

	public Generic6DofSpringConstraint(PhysicsWorld world,
			RigidBody rbB, Transform frameInB, boolean useLinearReferenceFrameB) {
		
		this(world, rbB, null, frameInB, null, useLinearReferenceFrameB, false);		
	}
	
	public Generic6DofSpringConstraint(PhysicsWorld world,
		RigidBody rbB, Transform frameInB, boolean useLinearReferenceFrameB,
		boolean disableCollisionsBetweenLinkedBodies) {
			
		this(world, rbB, null, frameInB, null, useLinearReferenceFrameB,
			disableCollisionsBetweenLinkedBodies);
	}
	
	public Generic6DofSpringConstraint(PhysicsWorld world,
			RigidBody rbA, RigidBody rbB,
			Transform frameInA, Transform frameInB, boolean useLinearReferenceFrameA) {
		
		this(world, rbA, rbB, frameInA, frameInB, useLinearReferenceFrameA, false);		
	}

	public Generic6DofSpringConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Transform frameInA, Transform frameInB, boolean useLinearReferenceFrameA,
		boolean disableCollisionsBetweenLinkedBodies) {
		
		super(world, D6_SPRING_CONSTRAINT_TYPE);
		mRbA = rbA;
		mRbB = rbB;
		if (frameInA!= null) mFrameA.set(frameInA);
		if (frameInA != null) mFrameB.set(frameInB);
		createConstraint(mFrameA, mFrameB, useLinearReferenceFrameA,
			disableCollisionsBetweenLinkedBodies);
	}
	
	public void enableSpring(int index, boolean onOff) {
		nativeEnableSpring(mID, index, onOff);
	}
	private static final native int nativeEnableSpring(long id_constraint, int index, boolean onOff);
	
	public void setStiffness(int index, float stiffness) {
		nativeSetStiffness(mID, index, stiffness);
	}
	private static final native int nativeSetStiffness(long id_constraint, int index, float stiffness);
	
	public void setDamping(int index, float damping) {
		nativeSetDamping(mID, index, damping);
	}
	private static final native int nativeSetDamping(long id_constraint, int index, float damping);
	
	// set the current constraint position/orientation as an equilibrium point for all DOF
	public void setEquilibriumPoint() {
		nativeSetEquilibriumPoint(mID);
	}
	private static final native int nativeSetEquilibriumPoint(long id_constraint);
	
	// set the current constraint position/orientation as an equilibrium point for given DOF
	public void setEquilibriumPoint(int index) {
		nativeSetEquilibriumPointIndex(mID, index);
	}
	private static final native int nativeSetEquilibriumPointIndex(long id_constraint, int index);
	
	public void setEquilibriumPoint(int index, float val) {
		nativeSetEquilibriumPointIndexVal(mID, index, val);
	}
	private static final native int nativeSetEquilibriumPointIndexVal(long id_constraint, int index, float val);

}
