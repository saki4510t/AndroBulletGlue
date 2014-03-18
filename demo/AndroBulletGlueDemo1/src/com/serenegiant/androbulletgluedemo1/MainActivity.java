/*
 * AndroBulletGlueDemo1
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MainActivity.java
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
package com.serenegiant.androbulletgluedemo1;

import com.serenegiant.androbulletglue.GLBulletActivity;
import com.serenegiant.androbulletglue.GLBulletWorldSurfaceView;
import com.serenegiant.androbulletglue.MotionState;
import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.interfaces.CollisionShape;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.shape.BoxShape;

public class MainActivity extends GLBulletActivity {
    private static final int ARRAY_SIZE_X = 5;
    private static final int ARRAY_SIZE_Y = 6;
    private static final int ARRAY_SIZE_Z = 1;

    private static final float BOX_SIZE = 1f;
    
    private static final float START_POS_X = -ARRAY_SIZE_X * BOX_SIZE + BOX_SIZE;
    private static final float START_POS_Y = BOX_SIZE + BOX_SIZE * 2;
    private static final float START_POS_Z = -ARRAY_SIZE_Y * BOX_SIZE * BOX_SIZE;

	@Override
	public PhysicsWorld initPhysics(GLBulletWorldSurfaceView view) {
		setCamDistance(30f);
		return new PhysicsWorld(-9.8f);
	}
	
	@Override
	public void createObjects(PhysicsWorld world) {
        // create ground plane(use top surface of box, static object)
		// You can also use StaticPlaneShape as a ground plane,
        new RigidBody(world,
            new BoxShape(world, new Vector3(50f, 0.1f, 50f)),
       		0f,	// mass(0 means static object)
        	new MotionState(0f, -0.1f, 0f)
        );
//    	new RigidBody(new StaticPlaneShape(0f, 1f, 0f, 0f));		// static plane
            
        // create stacked falling box(cube) objects 
        // Sharing and re-useing collision shape are better for performance.
        final CollisionShape colShape
        	= new BoxShape(world, BOX_SIZE, BOX_SIZE, BOX_SIZE);

        // stack boxies 
		for (int k = 0; k < ARRAY_SIZE_Z * 2; k += 2) {
			for (int j = 0; j < ARRAY_SIZE_Y * 2; j += 2) {
				for (int i = 0; i < ARRAY_SIZE_X * 2; i += 2) {
					new RigidBody(world,
						colShape,			// collision shape
						1f,					// mass
				    	new MotionState(	// initial position
				    		BOX_SIZE * i + START_POS_X,
							BOX_SIZE * j + START_POS_Y,
							BOX_SIZE * k + START_POS_Z
						)
				    );
				}
			}
		}
	}

	@Override
	public void destroyObjects(PhysicsWorld world) {
		// nothing to do in this demo
	}

}
