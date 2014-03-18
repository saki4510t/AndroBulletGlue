/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: ScaledBvhTriangleMeshShape.java
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

package com.serenegiant.androbulletglue.shape;

import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.interfaces.ConcaveShape;

/**
 * BvhTriangleMeshShape with scale conversion</br>
 * this is almost same as BvhTriangleMeshShape with calling BvhTriangleMeshShape#setLocalScaling
 */
public class ScaledBvhTriangleMeshShape extends ConcaveShape {
	
	protected final BvhTriangleMeshShape mChildShape;
	protected final Vector3 mLocalScaling;
	
	public ScaledBvhTriangleMeshShape(PhysicsWorld world,
		BvhTriangleMeshShape childShape, Vector3 localScaling) {
		
		super(world, SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE);
		
		if ( (childShape == null) || (childShape.getID() == 0)
			|| (localScaling == null) || (localScaling.getID() == 0) ) {
			throw new NullPointerException();
		}
		
		mChildShape = childShape;
		mLocalScaling = localScaling;
		createCollisionShape();
	}
}
