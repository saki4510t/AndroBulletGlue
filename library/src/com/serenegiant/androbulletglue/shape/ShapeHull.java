/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: ShapeHull.java
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

import java.nio.ShortBuffer;
import java.util.ArrayList;

import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.interfaces.ConvexShape;

public class ShapeHull {
	public static final int NUM_UNITSPHERE_POINTS = 42;
	public static final int MAX_VERTICES = 100;
	public static final int MAX_INDEX = 200;
	
	protected final ArrayList<Vector3> m_vertices;
	private final short[] m_indexArray;
	protected final ShortBuffer m_indices;
	protected int m_numIndices;
	protected ConvexShape m_shape;
	
	public ShapeHull(PhysicsWorld world, ConvexShape shape) {
		m_shape = shape;
		m_vertices = new ArrayList<Vector3>();
		m_indexArray = new short[MAX_INDEX];
		m_indices = ShortBuffer.wrap(m_indexArray);
		m_numIndices = 0;
		throw new IllegalArgumentException("ShapeHull is not supported yet");
	}
	
	protected Vector3[] getUnitSpherePoints() {
		// TODO
		return null;
	}
	
	public boolean buildHull(float margin) {
		// TODO
		return false;
	}
	
	public int numTriangles() {
		// TODO
		return 0;
	}
	
	public int numVertices() {
		// TODO
		return 0;
	}
	
	public int numIndices() {
		// TODO
		return 0;
	}
	
	public Vector3[] getVertexPointer() {
		return (Vector3[])m_vertices.toArray();
	}
	
	public short[] getIndexPointer() {
		return m_indexArray;
	}
}
