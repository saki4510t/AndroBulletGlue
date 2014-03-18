/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: ConvexHullSoftBody.java
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

package com.serenegiant.androbulletglue.softbody;

import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.interfaces.SoftBody;

public class ConvexHullSoftBody extends SoftBody {
	protected final float[] mVertices;
	protected final int mNumVertices;
	protected final boolean mRandomizeConstraints; 
	
	/**
	 * Constructor
	 * @param vertices
	 * @param randomizeConstraints
	 */
	public ConvexHullSoftBody(PhysicsWorld world,
		Vector3[] vertices, boolean randomizeConstraints) {
		
		super(world, SOFTBODY_FROM_CONVEXHULL);
		
		final int n = vertices.length;
		if (n <= 0)
			throw new IllegalArgumentException();
		mNumVertices = n;
		mVertices = new float[n * 3];
		for (int i = 0; i < n; i++) {
			mVertices[i * 3    ] = vertices[i].x();
			mVertices[i * 3 + 1] = vertices[i].y();
			mVertices[i * 3 + 2] = vertices[i].z();
		}
		mRandomizeConstraints = randomizeConstraints;
		
		createSoftBody();
	}

	/**
	 * Constructor(randomizeConstraints = true)
	 * @param vertices
	 */
	public ConvexHullSoftBody(PhysicsWorld world, float[] vertices) {
		this(world, vertices, true);
	}

	/**
	 * Constructor
	 * @param vertices
	 * @param randomizeConstraints
	 */
	public ConvexHullSoftBody(PhysicsWorld world,
		float[] vertices, boolean randomizeConstraints) {
		
		
		super(world, SOFTBODY_FROM_CONVEXHULL);
		
		mNumVertices = vertices.length / 3;
		mVertices = vertices;
		mRandomizeConstraints = randomizeConstraints;
		
		createSoftBody();
	}

}
