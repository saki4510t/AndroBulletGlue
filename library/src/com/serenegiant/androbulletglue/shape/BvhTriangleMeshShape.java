/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: BvhTriangleMeshShape.java
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
import com.serenegiant.androbulletglue.interfaces.TriangleMeshShape;
import com.serenegiant.androbulletglue.mesh.IndexedMesh;
import com.serenegiant.androbulletglue.mesh.TriangleIndexVertexArray;

public class BvhTriangleMeshShape extends TriangleMeshShape {
	protected final int mIndexedMeshNum;
	protected final IndexedMesh[] mIndexedMeshes;
	protected final boolean mUseQuantizedAabbCompression;
	protected boolean mHasAabb;
	
	protected BvhTriangleMeshShape(PhysicsWorld world, int type,
		TriangleIndexVertexArray triangleIndexVertexArray,
		boolean useQuantizedAabbCompression) {
		
		super(world, type);
		mUseQuantizedAabbCompression = useQuantizedAabbCompression;
		mIndexedMeshes = triangleIndexVertexArray.getIndexedMeshArray();
		mIndexedMeshNum = mIndexedMeshes != null ? mIndexedMeshes.length : 0; 
		mHasAabb = triangleIndexVertexArray.mHasAabb;
		mAabbMin.set(triangleIndexVertexArray.mAabbMin);
		mAabbMax.set(triangleIndexVertexArray.mAabbMax);
		// clear origianl object to reduce memory usage
		triangleIndexVertexArray.clearIndexedMesh();
		createCollisionShape();
	}
	/**
	 * Constructor
	 * @param triangleIndexVertexArray
	 * @param useQuantizedAabbCompression
	 */
	public BvhTriangleMeshShape(PhysicsWorld world,
		TriangleIndexVertexArray triangleIndexVertexArray, boolean useQuantizedAabbCompression) {
		
		this(world, TRIANGLE_MESH_SHAPE_PROXYTYPE, triangleIndexVertexArray, useQuantizedAabbCompression);
	}
}
