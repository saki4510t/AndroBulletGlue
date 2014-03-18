/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: UniversalConstraint.java
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

public class UniversalConstraint extends Generic6DofConstraint {

	protected final Vector3 mAnchor;
	protected final Vector3 mAxis1;
	protected final Vector3 mAxis2;
	
	public UniversalConstraint(PhysicsWorld world,
			RigidBody rbA, RigidBody rbB, Vector3 anchor,
			Vector3 axis1, Vector3 axis2) {
		
		this(world, rbA, rbB, anchor, axis1, axis2, false);
	}
	
	public UniversalConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB, Vector3 anchor,
		Vector3 axis1, Vector3 axis2,
		boolean disableCollisionsBetweenLinkedBodies) {
		
		super(world, D6_CONSTRAINT_TYPE);
		mRbA = rbA;
		mRbB = rbB;
		mFrameA.set(Transform.getIdentity());
		mFrameB.set(Transform.getIdentity());
		mAnchor = anchor;
		mAxis1 = axis1;
		mAxis2 = axis2;
		createConstraint(mFrameA, mFrameB, true, disableCollisionsBetweenLinkedBodies);
	}
	
	public void setUpperLimit(float ang1max, float ang2max) {
		setAngularUpperLimit(0.f, ang1max, ang2max);
	}
	
	public void setLowerLimit(float ang1min, float ang2min) {
		setAngularLowerLimit(0.f, ang1min, ang2min);
	}

}
