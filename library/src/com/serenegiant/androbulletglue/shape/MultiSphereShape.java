/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MultiSphereShape.java
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
import com.serenegiant.androbulletglue.interfaces.ConvexInternalAabbCachingShape;

public class MultiSphereShape extends ConvexInternalAabbCachingShape {
	
	protected final float[] mPosition;
	protected final float[] mRadius;
	protected final int mNumSpheres;
	// native code access this field and you should not remove and rename
	protected long mNativePtr;

	public MultiSphereShape(PhysicsWorld world,
		Vector3[] position, float[] radius, int numSpheres) {
		
		super(world, MULTI_SPHERE_SHAPE_PROXYTYPE);
		
		if (position == null || radius == null)
			throw new NullPointerException();
		
		int n = Math.min(position.length, radius.length);
		if (n > numSpheres)
			n = numSpheres;
		
		mPosition = new float[n * 3];
		for (int i = 0; i < n; i++) {
			if (position[i] != null) {
				mPosition[i * 3    ] = position[i].x(); 
				mPosition[i * 3 + 1] = position[i].y(); 
				mPosition[i * 3 + 2] = position[i].z(); 
			}
		}
		mRadius = radius;
		mNumSpheres = numSpheres;
		
		createCollisionShape();
	}
	
}
