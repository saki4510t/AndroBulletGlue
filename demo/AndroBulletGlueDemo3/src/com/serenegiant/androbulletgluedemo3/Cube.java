/*
 * AndroBulletGlueDemo3
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Cube.java
*/
/*
 * The original of Cube.java came from 'http://d.hatena.ne.jp/chiakisugimoto/20110114/1294956382'
 *
This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
package com.serenegiant.androbulletgluedemo3;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import javax.microedition.khronos.opengles.GL10;

import com.serenegiant.androbulletglue.math.Vector3;

public class Cube {
	private static final int one = 0x10000;	// use as a floating point value
	
	private static final int vertices[] = {
		-one/4, 6*one/4, -one/4,
		one/4, 6*one/4, -one/4,
		one/4,  8*one/4, -one/4,
		-one/4,  8*one/4, -one/4,
		-one/4, 6*one/4,  one/4,
		one/4, 6*one/4,  one/4,
		one/4,  8*one/4,  one/4,
		-one/4,  8*one/4,  one/4,
	};
    
	private static final int colors[] = {
		0,    0,    0,  one,
		one,    0,    0,  one,
		one,  one,    0,  one,
		0,  one,    0,  one,
		0,    0,  one,  one,
		one,    0,  one,  one,
		one,  one,  one,  one,
		0,  one,  one,  one,
	};

	private static final byte indices[] = {
		0, 4, 5,    0, 5, 1,
		1, 5, 6,    1, 6, 2,
		2, 6, 7,    2, 7, 3,
		3, 7, 4,    3, 4, 0,
		4, 7, 6,    4, 6, 5,
		3, 0, 1,    3, 1, 2
	};

	private ByteBuffer  mIndexBuffer;

	public void init(GL10 gl) {
		final ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		final IntBuffer vertexBuffer = vbb.asIntBuffer();
		vertexBuffer.put(vertices);
		vertexBuffer.position(0);
		
		final ByteBuffer cbb = ByteBuffer.allocateDirect(colors.length * 4);
		cbb.order(ByteOrder.nativeOrder());
		final IntBuffer colorBuffer = cbb.asIntBuffer();
		colorBuffer.put(colors);
		colorBuffer.position(0);
		
		mIndexBuffer = ByteBuffer.allocateDirect(indices.length);
		mIndexBuffer.put(indices);
		mIndexBuffer.position(0);
		
		gl.glFrontFace(GL10.GL_CW);
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
        gl.glEnableClientState(GL10.GL_COLOR_ARRAY);
		gl.glVertexPointer(3, GL10.GL_FIXED, 0, vertexBuffer);	// vertices as a fixed floating point array
		gl.glColorPointer(4, GL10.GL_FIXED, 0, colorBuffer);	// colors as a fixed floating point array
	}
	
	public void draw(GL10 gl, Vector3 pos) {
		gl.glPushMatrix();
			gl.glTranslatef(pos.x() / 20f, pos.y() / 20f, pos.z() / 20f);
			gl.glDrawElements(GL10.GL_TRIANGLES, 36, GL10.GL_UNSIGNED_BYTE, mIndexBuffer);
		gl.glPopMatrix();
	}
}
