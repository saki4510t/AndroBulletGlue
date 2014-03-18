/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Generic6DofConstraint.java
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

public class Generic6DofConstraint extends Constraint {
	

	protected Generic6DofConstraint(PhysicsWorld world, int type) {
		super(world, type);
	}
	
    public Generic6DofConstraint(PhysicsWorld world,
    	RigidBody rbB, Transform frameInB) {
            	
    	this(world, rbB, null, frameInB, null, false, false);
    }

    public Generic6DofConstraint(PhysicsWorld world,
        RigidBody rbB, Transform frameInB, boolean useLinearReferenceFrameB) {
        	
        this(world, rbB, null, frameInB, null, useLinearReferenceFrameB, false);
	}

    public Generic6DofConstraint(PhysicsWorld world,
    	RigidBody rbB, Transform frameInB, boolean useLinearReferenceFrameB,
    	boolean disableCollisionsBetweenLinkedBodies) {
    	
    	this(world, rbB, null, frameInB, null, useLinearReferenceFrameB,
    		disableCollisionsBetweenLinkedBodies);
    }

	public Generic6DofConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Transform frameInA, Transform frameInB, boolean useLinearReferenceFrameA) {
		this(world, rbA, rbB, frameInA, frameInB, useLinearReferenceFrameA, false);
	}
	
	public Generic6DofConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Transform frameInA, Transform frameInB, boolean useLinearReferenceFrameA,
		boolean disableCollisionsBetweenLinkedBodies) {
		
		super(world, D6_CONSTRAINT_TYPE);
		mRbA = rbA;
		mRbB = rbB;
		mFrameA.set(frameInA);
		if (frameInB != null) mFrameB.set(frameInB);
		createConstraint(mFrameA, mFrameB, useLinearReferenceFrameA,
			disableCollisionsBetweenLinkedBodies);
	}

	public void setLimit(int axis, float low, float high) {
		nativeSetLimit(mID, axis, low, high);
	}
	private static final native int nativeSetLimit(long id_constraint, int axis, float low, float high);
	
	public void setLinearLowerLimit(Vector3 lowerLimit) {
		nativeSetLinearLowerLimitVec(mID, lowerLimit.getID());
	}
	private static final native int nativeSetLinearLowerLimitVec(long id_constraint, long id_vec_lowerlimit);
	
	public void setLinearLowerLimit(float lowerLimit_x, float lowerLimit_y, float lowerLimit_z) {
		nativeSetLinearLowerLimit(mID, lowerLimit_x, lowerLimit_y, lowerLimit_z);
	}
	private static final native int nativeSetLinearLowerLimit(long id_constraint, float lowerLimit_x, float lowerLimit_y, float lowerLimit_z);

	public void setLinearUpperLimit(Vector3 upperLimit) {
		nativeSetLinearUpperLimitVec(mID, upperLimit.getID());
	}
	private static final native int nativeSetLinearUpperLimitVec(long id_constraint, long id_vec_upperlimit);
	
	public void setLinearUpperLimit(float upperLimit_x, float upperLimit_y, float upperLimit_z) {
		nativeSetLinearUpperLimit(mID, upperLimit_x, upperLimit_y, upperLimit_z);
	}
	private static final native int nativeSetLinearUpperLimit(long id_constraint, float upperLimit_x, float upperLimit_y, float upperLimit_z);

	public void setAngularLowerLimit(Vector3 lowerLimit) {
		nativeSetAngularLowerLimitVec(mID, lowerLimit.getID());
	}
	private static final native int nativeSetAngularLowerLimitVec(long id_constraint, long id_vec_lowerlimit);
	
	
	public void setAngularLowerLimit(float lowerLimit_x, float lowerLimit_y, float lowerLimit_z) {
		nativeSetAngularLowerLimit(mID, lowerLimit_x, lowerLimit_y, lowerLimit_z);
	}
	private static final native int nativeSetAngularLowerLimit(long id_constraint, float lowerLimit_x, float lowerLimit_y, float lowerLimit_z);
	
	public void setAngularUpperLimit(Vector3 upperLimit) {
		nativeSetAngularUpperLimitVec(mID, upperLimit.getID());
	}
	private static final native int nativeSetAngularUpperLimitVec(long id_constraint, long id_vec_upperlimit);

	public void setAngularUpperLimit(float upperLimit_x, float upperLimit_y, float upperLimit_z) {
		nativeSetAngularUpperLimit(mID, upperLimit_x, upperLimit_y, upperLimit_z);
	}
	private static final native int nativeSetAngularUpperLimit(long id_constraint, float upperLimit_x, float upperLimit_y, float upperLimit_z);
	
	public void setTranslationalLimitMotorEnableMotor(int ix, boolean enable) {
		nativeSetTranslationalLimitMotorEnableMotor(mID, ix, enable);
	}
	private static final native int nativeSetTranslationalLimitMotorEnableMotor(long id_constraint, int ix, boolean enable);

	public void setTranslationalLimitMotorTargetVelocity(int ix, float velocity) {
		nativeSetTranslationalLimitMotorTargetVelocity(mID, ix, velocity);
	}
	private static final native int nativeSetTranslationalLimitMotorTargetVelocity(long id_constraint, int ix, float velocity);

	public void setTranslationalLimitMotorMaxMotorForce(int ix, float force) {
		nativeSetTranslationalLimitMotorMaxMotorForce(mID, ix, force);
	}
	private static final native int nativeSetTranslationalLimitMotorMaxMotorForce(long id_constraint, int ix, float force);

}
