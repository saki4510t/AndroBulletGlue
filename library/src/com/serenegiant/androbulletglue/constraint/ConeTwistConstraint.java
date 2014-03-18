/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: ConeTwistConstraint.java
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
import com.serenegiant.androbulletglue.interfaces.Constraint;

public class ConeTwistConstraint extends Constraint {
	
	public ConeTwistConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Transform frameA, Transform frameB) {
		
		this(world, rbA, rbB, frameA, frameB, false);
	}
	
	public ConeTwistConstraint(PhysicsWorld world,
		RigidBody rbA, RigidBody rbB,
		Transform frameA, Transform frameB, boolean disableCollisionsBetweenLinkedBodies) {
		
		super(world, CONETWIST_CONSTRAINT_TYPE);
		mRbA = rbA;
		mRbB = rbB;
		mFrameA.set(frameA);
		mFrameB.set(frameB);
		createConstraint(frameA, frameB, false, disableCollisionsBetweenLinkedBodies);		
	}
	
	public void setLimitValue(int limitIndex, float limitValue) {
		nativeSetLimitValue(mID, limitIndex, limitValue);
	}
	private static final native int nativeSetLimitValue(long id, int limitIndex, float limitValue);
	
	public void setLimit(float swingSpan1, float swingSpan2, float twistSpan) {
		nativeSetLimit(mID, swingSpan1, swingSpan2, twistSpan, 1.f, 0.3f, 1.0f);
	}
	public void setLimit(float swingSpan1, float swingSpan2, float twistSpan, float softness) {
		nativeSetLimit(mID, swingSpan1, swingSpan2, twistSpan, softness, 0.3f, 1.0f);
	}
	public void setLimit(float swingSpan1, float swingSpan2, float twistSpan, float softness, float biasFactor) {
		nativeSetLimit(mID, swingSpan1, swingSpan2, twistSpan, softness, biasFactor, 1.0f);
	}
	public void setLimit(float swingSpan1, float swingSpan2, float  twistSpan, float softness, float biasFactor, float relaxationFactor) {
		nativeSetLimit(mID, swingSpan1, swingSpan2, twistSpan, softness, biasFactor, relaxationFactor);
	}
	private static final native int nativeSetLimit(long id, float swingSpan1, float swingSpan2, float  twistSpan, float softness, float biasFactor, float relaxationFactor);
}
