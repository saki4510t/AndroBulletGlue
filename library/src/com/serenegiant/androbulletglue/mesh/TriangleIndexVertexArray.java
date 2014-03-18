/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: TriangleIndexVertexArray.java
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

package com.serenegiant.androbulletglue.mesh;

import java.util.ArrayList;

import com.serenegiant.androbulletglue.math.Vector3;

public class TriangleIndexVertexArray {
	private final ArrayList<IndexedMesh> mIndexedMeshes = new ArrayList<IndexedMesh>();
	public boolean mHasAabb;
	public final Vector3 mAabbMin = new Vector3();
	public final Vector3 mAabbMax = new Vector3();
	
	public TriangleIndexVertexArray(
		int numTriangles, int[] triangleIndexBase,
		int numVertices, float[] vertexBase) {
		
		final IndexedMesh mesh = new IndexedMesh(
			numTriangles, triangleIndexBase,
			numVertices, vertexBase
		);

		addIndexedMesh(mesh);
		
	}
	
	public void addIndexedMesh(IndexedMesh mesh) {
		mIndexedMeshes.add(mesh);
	}

	public void addIndexedMesh(IndexedMesh[] meshs) {
		final int n = meshs != null ? meshs.length : 0;
		for (int i = 0; i < n; i++) {
			mIndexedMeshes.add(meshs[i]);
		}
	}
	
	public void clearIndexedMesh() {
		mIndexedMeshes.clear();
	}

	public IndexedMesh[] getIndexedMeshArray() {
//		return (IndexedMesh[])mIndexedMeshes.toArray();	// this does not work well, so we need copy by myself
		final int n = mIndexedMeshes.size();
		final int n4 = n % 4;
		final IndexedMesh[] result = new IndexedMesh[n];
		for (int i = 0; i < n4; i++) {
			result[i] = mIndexedMeshes.get(i);
		}
		for (int i = n4; i < n; i += 4) {
			result[i    ] = mIndexedMeshes.get(i    );
			result[i + 1] = mIndexedMeshes.get(i + 1);
			result[i + 2] = mIndexedMeshes.get(i + 2);
			result[i + 3] = mIndexedMeshes.get(i + 3);
		}
		return result;
	}
	
	public void setPremadeAabb(Vector3 aabbMin, Vector3 aabbMax) {
		mAabbMin.set(mAabbMin);
		mAabbMax.set(mAabbMax);
		mHasAabb = true;
	}
	
}
