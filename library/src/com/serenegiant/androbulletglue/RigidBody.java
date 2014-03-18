/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: RigidBody.java
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

package com.serenegiant.androbulletglue;

import com.serenegiant.androbulletglue.interfaces.Body;
import com.serenegiant.androbulletglue.interfaces.CollisionShape;
import com.serenegiant.androbulletglue.math.Matrix3x3;
import com.serenegiant.androbulletglue.math.Transform;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.shape.EmptyShape;

public class RigidBody extends Body {
	public static final int ACTIVE_TAG = 1;
	public static final int ISLAND_SLEEPING  = 2;
	public static final int WANTS_DEACTIVATION  = 3;
	public static final int DISABLE_DEACTIVATION = 4;
	public static final int DISABLE_SIMULATION = 5;
	private final int mShapeType;
	// fields for simulation result
	private final Vector3 mOrigin;
	private final Matrix3x3 mBasis;
	private final float[] option_params;

	/**
	 * constructor
	 * @param manager
	 * @param physicsWorld
	 * @param collisionShape
	 */
	public RigidBody(PhysicsWorld world, CollisionShape collisionShape) {
		this(world, collisionShape, 0f, new MotionState());
	}

	/**
	 * constructor
	 * @param manager
	 * @param physicsWorld
	 * @param collisionShape
	 */
	public RigidBody(PhysicsWorld world, CollisionShape collisionShape, float mass) {
		this(world, collisionShape, mass, new MotionState());
	}
	
	/**
	 * constructor
	 * @param collisionShape
	 * @param mass
	 * @param transform
	 */
	public RigidBody(PhysicsWorld world, CollisionShape collisionShape, float mass, Transform transform) {
		this(world, collisionShape, mass, new MotionState(transform));
	}
	
	/**
	 * constructor
	 * @param manager
	 * @param physicsWorld
	 * @param collisionShape
	 * @param motionState
	 */
	public RigidBody(PhysicsWorld world, CollisionShape collisionShape, float mass, MotionState motionState) { 
		super(world, 0);
		// whether or not to store simulation result into Java object?
		if (world.needJavaResult()) {
			mOrigin = new Vector3();
			mBasis = new Matrix3x3();
			option_params = new float[9];
			if (motionState != null) {
				mOrigin.set(motionState.worldTransform.getOrigin());
				mBasis.set(motionState.worldTransform.getBasis());
			} else {
				mBasis.setIdentity();
			}
		} else if (motionState != null) {
			mOrigin = motionState.worldTransform.getOrigin();
			mBasis = motionState.worldTransform.getBasis();
			option_params = null;
		} else {
			mOrigin = null;
			mBasis = null;
			option_params = null;
		}
		collisionShape = collisionShape != null ? collisionShape : new EmptyShape(world);
		mShapeType = collisionShape.getType();
		// create native object
		mID = nativeCreate(mPhysicsWorldID, collisionShape.getID(), mass, motionState, LARGE_FLOAT);
		world.add(this);
	}
	// nativeCreate in this class can not be a static method
	// because native code access fields in this object instance.
	private final native long nativeCreate(
		long id_physicsWorld, long id_shape, float mass, MotionState motionState, float contactProcessingThreshold);
	
	/**
	 * dispose
	 */
	@Override
	public void dispose() {
		if (mID != 0) {
			mWorld.remove(this);
		}
		super.dispose();
	}
	
	@Override
	protected final native void nativeDestroy(long id_rigidBody);	

	public void applyForce(Vector3 force, Vector3 applyPoint) {
		nativeApplyForce(mID, force.getID(), applyPoint.getID());
	}
	private static final native int nativeApplyForce(long id_rigibody, long id_vec_force, long id_vec_applyPoint);

	public void applyTorque(Vector3 torque) {
		nativeApplyTorque(mID, torque.getID());
	}
	private static final native int nativeApplyTorque(long id_rigibody, long id_vec_torque);
	
	public void applyCentralImpulse(Vector3 impulse) {
		nativeApplyCentralImpulse(mID, impulse.getID());
	}
	private static final native int nativeApplyCentralImpulse(long id_rigibody, long id_vec_impulse);
	
	public void applyTorqueImpulse(Vector3 torque) {
		nativeApplyTorqueImpulse(mID, torque.getID());
	}
	private static final native int nativeApplyTorqueImpulse(long id_rigibody, long id_vec_torque);
	
	public void applyImpulse(Vector3 impulse, Vector3 applyPoint) {
		nativeApplyImpulse(mID, impulse.getID(), applyPoint.getID());
	}
	private static final native int nativeApplyImpulse(long id_rigibody, long id_vec_impulse, long id_vec_applyPoint);
	
	public void clearForces() {
		nativeClearForces(mID);
	}
	private static final native int nativeClearForces(long id_rigibody);

	public void setActive(boolean isActive) {
		nativeSetActive(mID, isActive);
	}
	private static final native int nativeSetActive(long id_rigibody, boolean isActive);
	
	public int getActivationState() {
		return nativeGetActivationState(mID);
	}
	private static final native int nativeGetActivationState(long id_rigidbody);
	
	public void setActivationState(int activationState) {
		nativeSetActivationState(mID, activationState);
	}
	private static final native int nativeSetActivationState(long id_rigidbody, int activationState);
	
	
	public void setDamping(float lin_damping, float ang_damping) {
		nativeSetDamping(mID, lin_damping, ang_damping);
	}
	private static final native int nativeSetDamping(long id_rigidbody, float lin_damping, float ang_damping);
	
	public void setDeactivationTime(float time) {
		nativeSetDeactivationTime(mID, time);
	}
	private static final native int nativeSetDeactivationTime(long id_rigidbody, float time);
	
	public float getDeactivationTime() {
		return nativeGetDeactivationTime(mID);
	}
	private static final native float nativeGetDeactivationTime(long id_rigidbody);
	
	public void setSleepingThresholds(float linear, float angular) {
		nativeSetSleepingThresholds(mID, linear, angular);
	}
	private static final native int nativeSetSleepingThresholds(long id_rigidbody, float linear, float angular);
	
	public void setLinearFactor(Vector3 factor) {
		nativeSetLinearFactorVec(mID, factor.getID());
	}
	public void setLinearFactor(float factor_x, float factor_y, float factor_z) {
		nativeSetLinearFactor(mID, factor_x, factor_y, factor_z);
	}
	private static final native int nativeSetLinearFactor(long id_rigidbody, float factor_x, float factor_y, float factor_z);
	private static final native int nativeSetLinearFactorVec(long id_rigidbody, long id_vec_factor);

	public void setAngularVelocity(float velocity_x, float velocity_y, float velocity_z) {
		nativeSetAngularVelocity(mID, velocity_x, velocity_y, velocity_z);
	}
	public void setAngularVelocity(Vector3 velocity) {
		nativeSetAngularVelocityVec(mID, velocity.getID());
	}
	private static final native int nativeSetAngularVelocity(long id_rigidbody, float velocity_x, float velocity_y, float velocity_z);
	private static final native int nativeSetAngularVelocityVec(long id_rigidbody, long id_vec_velocity);

	public void setAngularFactor(float factor_x, float factor_y, float factor_z) {
		nativeSetAngularFactor(mID, factor_x, factor_y, factor_z);
	}
	public void setAngularFactor(Vector3 factor) {
		nativeSetAngularFactorVec(mID, factor.getID());
	}
	private static final native int nativeSetAngularFactor(long id_rigidbody, float factor_x, float factor_y, float factor_z);
	private static final native int nativeSetAngularFactorVec(long id_rigidbody, long id_vec_factor);

	public int getShapeType() {
		return mShapeType;
	}

	public Vector3 getOrigin() {
		return mOrigin;
	}
	
	public Matrix3x3 getBasis() {
		return mBasis;
	}
	
	public Transform getCenterOfMassTransform() {
		final Transform transform = new Transform();
		nativeGetCenterOfMassTransform(mID, transform.getID());
		return transform;
	}
	private static final native int nativeGetCenterOfMassTransform(long id_rigidbody, long id_trans);
	
	public void setCenterOfMassTransform(Transform transform) {
		nativeSetCenterOfMassTransform(mID, transform.getID());
	}
	private static final native int nativeSetCenterOfMassTransform(long id_rigidbody, long id_trans);

	public final void setFriction(float friction) {
		nativeSetFriction(mID, friction);
	}
	private static final native int nativeSetFriction(long id_rigidbody, float friction);
}
