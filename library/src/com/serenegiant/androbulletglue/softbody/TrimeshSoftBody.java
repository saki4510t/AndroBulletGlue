/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: TrimeshSoftBody.java
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
import com.serenegiant.androbulletglue.interfaces.SoftBody;
import com.serenegiant.androbulletglue.mesh.IndexedMesh;

public class TrimeshSoftBody extends SoftBody {
	protected final float[] mVertices;
	protected final int[] mTriangles;
	protected final int mNumTriangles;
	protected final boolean mRandomizeConstraints;
	
	/**
	 * Constructor
	 * @param mesh
	 * @param randomizeConstraints
	 */
	public TrimeshSoftBody(PhysicsWorld world,
		IndexedMesh mesh, boolean randomizeConstraints) {
		this(world, mesh.mVertexBase, mesh.mTriangleIndexBase,
			mesh.mNumTriangles, randomizeConstraints);
	}
	
	/**
	 * Constructor
	 * @param vertices
	 * @param triangles
	 * @param numTriangles
	 */
	public TrimeshSoftBody(PhysicsWorld world,
		float[] vertices, int[] triangles, int numTriangles) {
		
		this(world, vertices, triangles, numTriangles, true);
	}

	/**
	 * Constructor
	 * @param vertices
	 * @param triangles
	 * @param numTriangles
	 * @param randomizeConstraints
	 */
	public TrimeshSoftBody(PhysicsWorld world,
		float[] vertices, int[] triangles, int numTriangles, boolean randomizeConstraints) {
		
		super(world, SOFTBODY_FROM_TRIMESH);
		
		if (vertices == null || triangles == null)
			throw new NullPointerException();
		
		mVertices = vertices;
		mTriangles = triangles;
		mNumTriangles = numTriangles;
		mRandomizeConstraints = randomizeConstraints;
		
		createSoftBody();
	}

}
