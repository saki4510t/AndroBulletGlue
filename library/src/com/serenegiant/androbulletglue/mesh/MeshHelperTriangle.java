/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MeshHelperTriangle.java
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

import com.serenegiant.androbulletglue.MotionState;
import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.shape.BvhTriangleMeshShape;

public class MeshHelperTriangle {

	/**
	 * Create triangle-mesh ground
	 * @param size
	 * @param num_x
	 * @param num_y
	 * @param height
	 * @param length
	 * @return
	 */
	public static RigidBody createGround(PhysicsWorld world, float size, int num_x, int num_y, float height, float length) {
		return createGround(world, size, num_x, num_y, height, length, 0f);
	}
	/**
	 * Create triangle-mesh ground
	 * @param size		triangle size
	 * @param num_x		num of vertex(x)
	 * @param num_y		num of vertex(y)
	 * @param height	wave height
	 * @param length	wave length
	 * @param phase		phase
	 * @return			RigidBody
	 */
	public static RigidBody createGround(PhysicsWorld world, float size, int num_x, int num_y, float height, float length, float phase) {
    	final int totalVertNum = num_x * num_y;
    	final int totalIndexNum = 2 * (num_x - 1) * (num_y - 1);
    	
    	float[] vertices = new float[totalVertNum * 3];
    	int[] indices = new int[totalIndexNum * 3];

    	int ix;
    	for (int i = 0;i < num_x; i++) {
    		for (int j = 0; j < num_y; j++) {
    			ix = (i + j * num_x) * 3;
				vertices[ix + 0] = (i - num_x * 0.5f) * size;
				vertices[ix + 1] = height * (float)(Math.sin(i * length) * Math.cos(j * length + phase));
				vertices[ix + 2] = (j - num_y * 0.5f) * size;
    		}
    	}

    	int index = 0;
    	for (int i = 0; i < num_x - 1; i++) {
    		for (int j = 0; j < num_y - 1; j++) {
    			indices[index++] = j * num_x + i;
    			indices[index++] = j * num_x + i + 1;
    			indices[index++] = (j + 1) * num_x + i + 1;

    			indices[index++] = j * num_x + i;
    			indices[index++] = (j + 1) * num_x + i + 1;
    			indices[index++] = (j + 1) * num_x + i;
    		}
    	}

    	final TriangleIndexVertexArray indexVertexArray = new TriangleIndexVertexArray(
    		totalIndexNum, indices, totalVertNum, vertices);

        // create ground
        return new RigidBody(world,
        	new BvhTriangleMeshShape(world, indexVertexArray, true),
       		0f,									// mass, 0 means static object
       		new MotionState(0f, -10f, 0f)		// initial position
        );

	}
}
