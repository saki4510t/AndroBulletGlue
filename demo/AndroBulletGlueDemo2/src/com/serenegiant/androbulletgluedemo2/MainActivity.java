/*
 * AndroBulletGlueDemo2
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
package com.serenegiant.androbulletgluedemo2;

import com.serenegiant.androbulletglue.GLBulletActivity;
import com.serenegiant.androbulletglue.GLBulletWorldSurfaceView;
import com.serenegiant.androbulletglue.MotionState;
import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.interfaces.CollisionShape;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.shape.BoxShape;
import com.serenegiant.androbulletglue.shape.CylinderShapeY;
import com.serenegiant.androbulletglue.shape.SphereShape;

public class MainActivity extends GLBulletActivity {
    // create 25 (5x5x1) dynamic object
    private static final int ARRAY_SIZE_X = 5;
    private static final int ARRAY_SIZE_Y = 5;
    private static final int ARRAY_SIZE_Z = 1;
    // The total number of simultaneously moving and/or colliding objects
    // are prefered smaller than 30-40 for Nexus7 and the value depends
    // on the device performance.
    // Too many simultaneously moving objects make simulation and render slow.
    // Much more objects are acceptable for the case of static objects and
    // non simultaneously moving objects and intermittently moving objects.
    
    /**
     * scaling of the objects (0.1[m] = 0.1 x 2[m] = 20[cm] boxes )
     */
    private static final float BOX_SIZE = 1f;
    
    private static final float START_POS_X = -ARRAY_SIZE_X * BOX_SIZE + BOX_SIZE;
    private static final float START_POS_Y = BOX_SIZE + BOX_SIZE * 25;
    private static final float START_POS_Z = -ARRAY_SIZE_Y * BOX_SIZE + BOX_SIZE;
    
	@Override
	protected void onResume() {
		super.onResume();
		setDebugMode(GLBulletWorldSurfaceView.DBG_NOHELPTEXT);
	}
	
	@Override
	public PhysicsWorld initPhysics(GLBulletWorldSurfaceView view) {
		// set distance between camera and target position
		setCamDistance(40f);
		// set shoot speed
		setShootSpeed(120.f);
		// create physics world with -9.8 of gravity
		return new PhysicsWorld(-9.8f);
	}
	        
	@Override
	public void createObjects(PhysicsWorld world) {
        // create ground plane(use top surface of box, static object)
		// You can also use StaticPlaneShape as a ground plane,
        new RigidBody(world,
        	new BoxShape(world, new Vector3(50f, 0.1f, 50f)),		// collision shpae = box(100x0.2x100)
   			0f,														// mass(0 means static object)
    		new MotionState(0f, -0.1f, 0f)							// initial position
    	);
//    	new RigidBody(new StaticPlaneShape(0f, 1f, 0f, 0f));		// static plane
        
        // create box(cube) object
        new RigidBody(world,
        	new BoxShape(world, 5f, 5f, 5f),						// collision shape = box(10x10x10)
       		5f,														// mass(0 means static object)
    		new MotionState(6f, 0f, 0f)								// initial position
        );
        // create embedded sphere
		new RigidBody(world,
			new SphereShape(world, 5f),								// collision shape = sphere(radius=5)
        	0.f,													// mass(0 means static object)
        	new MotionState(-5f, 0f, -3f)							// initial position
        );
		// create small pillar
		new RigidBody(world,
			new CylinderShapeY(world, 1f, 2f, 0.2f),				// collision shape = cylinder(2x4, radius=0.2)
	       	0.f,													// mass(0 means static object)
	       	new MotionState(5f, 2f, -5f)							// initial position
		);
		
        // create stacked falling box(cube) objects 
        // Sharing and re-useing collision shape are better for performance.
        final CollisionShape colShape
    		= new BoxShape(world, BOX_SIZE * 1f, BOX_SIZE * 1f, BOX_SIZE * 1f);	// collision shape = box

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
