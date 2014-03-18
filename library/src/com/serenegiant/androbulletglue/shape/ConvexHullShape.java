/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: ConvexHullShape.java
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
import com.serenegiant.androbulletglue.interfaces.PolyhedralConvexAabbCachingShape;

public class ConvexHullShape extends PolyhedralConvexAabbCachingShape {
	public ConvexHullShape(PhysicsWorld world) {
		super(world, CONVEX_HULL_SHAPE_PROXYTYPE);
		createCollisionShape();
	}
	
	public void addPoint(Vector3 point) {
		nativeAddPoint(mID, point.getID());
	}
	private static final native int nativeAddPoint(long id_shape, long id_vec);
	
	/**
	 * this will enable polyhedral contact clipping, better quality, slightly slower
	 */
	public void initializePolyhedralFeatures() {
		nativeInitializePolyhedralFeatures(mID, 0);
	}
	/**
	 * this will enable polyhedral contact clipping, better quality, slightly slower
	 * @param shiftVerticesByMargin default value is 0
	 */
	public void initializePolyhedralFeatures(int shiftVerticesByMargin) {
		nativeInitializePolyhedralFeatures(mID, shiftVerticesByMargin);
	}	
	private static final native int nativeInitializePolyhedralFeatures(long id_shape, int shiftVerticesByMargin);
}
