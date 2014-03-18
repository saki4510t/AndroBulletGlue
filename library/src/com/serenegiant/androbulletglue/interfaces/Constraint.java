/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Constraint.java
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

package com.serenegiant.androbulletglue.interfaces;

import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.math.Transform;

public abstract class Constraint extends BulletObject {

	public static final int	POINT2POINT_CONSTRAINT_TYPE = 3;	// btPoint2PointConstraint
	public static final int	HINGE_CONSTRAINT_TYPE = 4;			// btHingeConstraint
	public static final int	CONETWIST_CONSTRAINT_TYPE = 5;		// btConeTwistConstraint
	public static final int	D6_CONSTRAINT_TYPE = 6;				// btGeneric6DofConstraint
	public static final int	SLIDER_CONSTRAINT_TYPE = 7;			// btSliderConstraint
	public static final int	CONTACT_CONSTRAINT_TYPE = 8;		// not supported because there is no demo in original Bullet
	public static final int	D6_SPRING_CONSTRAINT_TYPE = 9;		// btGeneric6DofSpringConstraint
	public static final int	GEAR_CONSTRAINT_TYPE = 10;			// btGearConstraint
//	public static final int	MAX_CONSTRAINT_TYPE = 11;

	protected RigidBody mRbA;
	protected RigidBody mRbB;
	protected final Transform mFrameA = new Transform();
	protected final Transform mFrameB = new Transform();
	

	protected Constraint(PhysicsWorld world, int type) {
		super(world, type);
	}

	protected void createConstraint(
		Transform transformA, Transform transformB,
		boolean useReferenceFrameA, boolean disableCollisionsBetweenLinkedBodies) {
		
		mID = nativeCreate(
			transformA != null ? transformA.mID : 0,
			transformB != null ? transformB.mID : 0, useReferenceFrameA);
		addConstraint(disableCollisionsBetweenLinkedBodies);
	}
	// nativeCreate in this class can not be a static method
	// because native code access fields in this object instance.
	private final native long nativeCreate(long id_transA, long id_transB, boolean useReferenceFrameA);
	
	@Override
	public void dispose() {
		mWorld.remove(this);
		super.dispose();
	}
	@Override
	protected native void nativeDestroy(long id);
	
	protected void addConstraint(boolean disableCollisionsBetweenLinkedBodies) {
		mWorld.add(this);
		nativeAddConstraint(mID, mPhysicsWorldID, disableCollisionsBetweenLinkedBodies);
	}
	private static final native int nativeAddConstraint(long id_constraint, long id_physicsworld, boolean disableCollisionsBetweenLinkedBodies);

	public int setDbgDrawSize(float dbgDrawSize) {
		return nativeSetDbgDrawSize(mID, dbgDrawSize);
	}
	private static final native int nativeSetDbgDrawSize(long id_constraint, float dbgDrawSize);

}
