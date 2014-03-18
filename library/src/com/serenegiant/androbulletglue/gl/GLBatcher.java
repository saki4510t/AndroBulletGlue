/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: GLBatcher.java
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

package com.serenegiant.androbulletglue.gl;

import java.util.Arrays;

import javax.microedition.khronos.opengles.GL10;

/**
 * Helper class to draw vertices to improve performance 
 */
public final class GLBatcher {
	private final float[] vertexBuffer;
	private final Vertex vertex;
	private int bufferIndex;
	private int numVertices;
	private int mPrimitiveType;
	
	public GLBatcher(GL10 gl, int maxVertices) {
		vertexBuffer = new float[maxVertices * 4 * 4];
		vertex = new Vertex(Vertex.DIM_3D, gl, maxVertices * 3, 0, true, true, true);
	}
	
	public void glBegin(int primitiveType) {
		mPrimitiveType = primitiveType;
		bufferIndex = numVertices = 0;
		Arrays.fill(vertexBuffer, 0f);
	}
	
	public void glEnd() {
		vertex.setVertex(vertexBuffer, 0, bufferIndex);
		vertex.bind();
		vertex.draw(mPrimitiveType, 0, bufferIndex);
		vertex.unbind();
	}
	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
	/**
	 * set x/y/z coordinates 
	 * @param x
	 * @param y
	 * @param z
	 */
	public void glVertex3f(float x, float y, float z) {
		if (!hasColor)
			glColor4f(color_r, color_g, color_b, color_a);
		vertexBuffer[bufferIndex+0] = x;
		vertexBuffer[bufferIndex+1] = y;
		vertexBuffer[bufferIndex+2] = z;
		numVertices++;
		bufferIndex = numVertices * 4;
		hasColor = false;
	}

	private boolean hasColor;
	private float color_r;
	private float color_g;
	private float color_b;
	private float color_a;
	/**
	 * set color information
	 * @param r
	 * @param g
	 * @param b
	 * @param a
	 */
	public void glColor4f(float r, float g, float b, float a) {
		vertexBuffer[bufferIndex+3] = color_r = r;
		vertexBuffer[bufferIndex+4] = color_g = g;
		vertexBuffer[bufferIndex+5] = color_b = b;
		vertexBuffer[bufferIndex+6] = color_a = a;
		hasColor = true;
	}

	/**
	 * set color information with alpha=1
	 * @param r
	 * @param g
	 * @param b
	 */
	public void glColor3f(float r, float g, float b) {
		vertexBuffer[bufferIndex+3] = r;
		vertexBuffer[bufferIndex+4] = g;
		vertexBuffer[bufferIndex+5] = b;
		vertexBuffer[bufferIndex+6] = 1f;
	}

	/**
	 * set texture t/u coordinates
	 * @param t
	 * @param u
	 */
	public void glTexture3f(float t, float u) {
		vertexBuffer[bufferIndex+7] = t;
		vertexBuffer[bufferIndex+8] = u;
	}

	/**
	 * set normal x/y/z coordinates
	 * @param x
	 * @param y
	 * @param z
	 */
	public void glNormal3f(float x, float y, float z) {
		vertexBuffer[bufferIndex+9] = x;
		vertexBuffer[bufferIndex+10] = y;
		vertexBuffer[bufferIndex+11] = z;
	}
}
