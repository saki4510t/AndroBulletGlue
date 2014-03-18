/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MeshHelperSphere.java
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
import com.serenegiant.androbulletglue.interfaces.CollisionShape;
import com.serenegiant.androbulletglue.math.Transform;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.shape.BvhTriangleMeshShape;

public class MeshHelperSphere {

	protected static CollisionShape createShape(PhysicsWorld world,
		float radius, int num_theta, int num_phi) {
		
		final int num = (num_phi - 1) * num_theta;
		final float[] vertices = new float[(2 + num) * 3];
		final int[] indices = new int[num * 2 * 3];
		init(vertices, indices, radius, num_theta, num_phi);
		
    	final TriangleIndexVertexArray indexVertexArray = new TriangleIndexVertexArray(
    			indices.length / 3, indices, vertices.length / 3, vertices);
    	
    	return new BvhTriangleMeshShape(world, indexVertexArray, true);
	}
	
	public static RigidBody createRigidBody(PhysicsWorld world,
		float mass, float radius, Transform trans, Vector3 scale) {
		
		return createRigidBody(world, mass, radius, 36, 36, trans, scale);
	}

	public static RigidBody createRigidBody(PhysicsWorld world,
		float mass, float radius, int nums, Transform trans, Vector3 scale) {
		
		return createRigidBody(world, mass, radius, nums, nums, trans, scale);
	}

	public static RigidBody createRigidBody(PhysicsWorld world,
		float mass, float radius, int num_theta, int num_phi, Transform trans, Vector3 scale) {
		
		final CollisionShape shape = createShape(world, radius, num_theta, num_phi); 
    	if (scale != null) {
    		shape.setLocalScaling(scale);
    	}
        // create rigidbody
    	return new RigidBody(world,
    		shape,
        	mass,					// mass, 0 means static object
        	new MotionState(trans)	// initial motionState
        );
	}
	
	private static void init(float[] vertices, int[] indices, float radius, int num_theta, int num_phi) {
		final double d_theta = 2 * Math.PI / num_theta;
		final double d_phi = Math.PI / num_phi;
		
		int pt = 0;
		double r, y;
		for (int i = 0; i < num_phi + 1; i++) {
			y = -radius * Math.cos(i * d_phi);
			r = Math.sqrt(radius * radius - y * y);
			if (i == 0) {					// top
				vertices[pt++] = 0;			// x
				vertices[pt++] = -radius;	// y
				vertices[pt++] = 0;			// z
			} else if (i == num_phi) { 		// bottom
				vertices[pt++] = 0;			// x
				vertices[pt++] = radius;	// y
				vertices[pt++] = 0;			// z
			} else {						// side wall
				for (int j = 0; j < num_theta; j++) {
					vertices[pt++] = (float)(r * Math.cos(j * d_theta));
					vertices[pt++] = (float)y;
					vertices[pt++] = (float)(r * Math.sin(j * d_theta));
				}
			}
		}
		
		pt = 0;
		int m;
		for (int i = 0; i < num_phi; i++) {
			m = (i - 1) * num_theta;
			for (int j = 0; j < num_theta; j++) {
				if (i == 0) {					//  top
					indices[pt++] = 0;
					indices[pt++] = (short)((j + 1) % num_theta + 1);
					indices[pt++] = (short)(j + 1);					
				} else if (i == num_phi - 1) {	// bottom
					indices[pt++] = (short)(j + 1 + m);
					indices[pt++] = (short)((j + 1 + m) % num_theta + 1 + m);
					indices[pt++] = (short)(1 + m + num_theta);					
				} else {						// side wall
					indices[pt++] = (short)(j + 1 + m);
					indices[pt++] = (short)((j + 1) % num_theta + 1 + m);
					indices[pt++] = (short)(j + 1 + m + num_theta);
					
					indices[pt++] = (short)((j + 1) % num_theta + 1 + m + num_theta);
					indices[pt++] = (short)(j + 1 + m + num_theta);
					indices[pt++] = (short)((j + 1) % num_theta + 1 + m);
				}
			}
		}
	}
}
