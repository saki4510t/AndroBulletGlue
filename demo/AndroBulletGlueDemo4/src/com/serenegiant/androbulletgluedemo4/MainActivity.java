/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MainActivity.java
 */
/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

package com.serenegiant.androbulletgluedemo4;

import javax.microedition.khronos.opengles.GL10;

import com.serenegiant.androbulletglue.GLBulletActivity;
import com.serenegiant.androbulletglue.GLBulletWorldSurfaceView;
import com.serenegiant.androbulletglue.MotionState;
import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.softbody.SoftBodyConfig;
import com.serenegiant.androbulletglue.mesh.MeshHelperTorus;
import com.serenegiant.androbulletglue.mesh.MeshHelperTriangle;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.collision.DefaultCollisionDispatcher;
import com.serenegiant.androbulletglue.collision.SoftBodyRigidBodyCollisionConfiguration;
import com.serenegiant.androbulletglue.broadphase.AxisSweepBroadphase;
import com.serenegiant.androbulletglue.dynamicsWorld.SoftRigidDynamicsWorld;
import com.serenegiant.androbulletglue.interfaces.SoftBody;
import com.serenegiant.androbulletglue.shape.BoxShape;
import com.serenegiant.androbulletglue.softbody.EllipsoidSoftBody;
import com.serenegiant.androbulletglue.softbody.MotorControl;
import com.serenegiant.androbulletglue.softbody.PatchSoftBody;
import com.serenegiant.androbulletglue.softbody.RopeSoftBody;
import com.serenegiant.androbulletglue.solver.SequentialImpulseConstraintSolver;

public class MainActivity extends GLBulletActivity {
    
	private static final float PI = (float)Math.PI;
	/**
	 * type of demo
	 */
	private final int MAX_DEMO = 5;
	/**
	 * current demo type [0-MAX_DEMO)
	 */
	private int demoNo = 0;
	private float prevTime;
	/**
	 * intervals to change demo[seconds]</br>
	 */
	private float demoTime = 20f;
	private MotorControl mMotorControl;

	@Override
	protected void onResume() {
		super.onResume();
		setDebugMode(0);
	}
	
	@Override
	public PhysicsWorld initPhysics(GLBulletWorldSurfaceView view) {
        // set distance between camera and target position
		setCamDistance(50f);

		// create rigid & softbody physics world with -9.8 of gravity
		return new PhysicsWorld(
        	new SoftBodyRigidBodyCollisionConfiguration(),
        	new DefaultCollisionDispatcher(),
        	new SequentialImpulseConstraintSolver(),
        	new AxisSweepBroadphase(),
        	new SoftRigidDynamicsWorld(),
        	new Vector3(0f, -9.8f, 0f)
        );
	}
	
	
	@Override
	public void createObjects(PhysicsWorld world) {
    	// create a triangle-mesh ground
//		MeshHelperTriangle.createGround(world, 8f, 20, 20, 10, 0.5f);
        // create ground plane(use top surface of box, static object)
		// You can also use StaticPlaneShape as a ground plane,
        new RigidBody(world,
        	new BoxShape(world, new Vector3(50f, 0.1f, 50f)),		// collision shpae = box(100x0.2x100)
   			0f,														// mass(0 means static object)
    		new MotionState(0f, -20.f, 0f)							// initial position
    	);
        // create softbody
        switch (demoNo) {
        case 0:	initRope(world); break;
        case 1:	initRopeAttach(world); break;
        case 2:	initClothAttach(world); break;
		case 3: initPressure(world); break;
        case 4: initClusterCombine(world); break;
        }
        demoNo = (demoNo + 1) % MAX_DEMO;
        prevTime = System.nanoTime() / 1000000000.f;	// as seconds
	}

	@Override
	public void destroyObjects(PhysicsWorld world) {
		if (mMotorControl != null) {
			mMotorControl.dispose();
			mMotorControl = null;
		}
	}
	
	@Override
	public void renderFrame(GL10 gl) {
		final float t = System.nanoTime() / 1000000000.f;
		if (t - prevTime > demoTime) {	// change to next demo
			resetWorld();
		}
	}

//********************************************************************************
// demo methods
//********************************************************************************
	private static final int ROPE_NUM = 15;
	/**
	 * demo: many ropes that their both ends are fixed.
	 */
	private final void initRope(PhysicsWorld world) {
		for(int i = 0; i < ROPE_NUM; ++i) {
			final SoftBody softbody = new RopeSoftBody(world,
				new Vector3(-10f, 0f, i * 0.25f),
				new Vector3( 10f, 0f, i * 0.25f),
				16,
				1 + 2);
			softbody.setCfgPositionsIterations(4);
			softbody.setMaterialLinearStiffnessCoefficient(0, 0.1f + (i / ( ROPE_NUM - 1f)) * 0.9f);
			softbody.setTotalMass(20);
		}
		demoTime = 20f;
	}
	
	/**
	 * demo: rope attached to moving rigid body and fixed point 
	 */
	private final void initRopeAttach(PhysicsWorld world) {
		final RigidBody rigidbody = new RigidBody(world,
			new BoxShape(world, 2f, 6f, 2f), 
			50f,
			new MotionState(12f, 8f, 0f));
		final SoftBody softbody0 = createRope(world, new Vector3(0f, 8f, -1f), 50f);
		final SoftBody softbody1 = createRope(world, new Vector3(0f, 8f, +1f), 50f);
		softbody0.appendAnchor(rigidbody);
		softbody1.appendAnchor(rigidbody);		
		demoTime = 20f;
	}
	
	/**
	 * demo: cloth attached to moving rigid body and anchor
	 */
	private final void initClothAttach(PhysicsWorld world) {
		final float s = 4f;
		final float h = 6f;
		final int r = 9;
		final SoftBody softbody = new PatchSoftBody(world,
			new Vector3(-s,h,-s),
			new Vector3(+s,h,-s),
			new Vector3(-s,h,+s),
			new Vector3(+s,h,+s),r,r,4+8, true);

		final RigidBody body = new RigidBody(world,
			new BoxShape(world, s, 1, 3),
			20,
			new MotionState(0, h, -(s+3.5f)));
		softbody.appendAnchor(0, body);
		softbody.appendAnchor(r - 1, body);
		demoTime = 20f;
	}


	/**
	 * demo: soft sphere rolling down the stairs (very heavy on smartphone/tablet)
	 */
	private final void initPressure(PhysicsWorld world) {
		
		final SoftBody softbody = new EllipsoidSoftBody(world,
			new Vector3(25f, 25f, 0f),
			new Vector3(1f, 1f, 1f).mult(3f),
			512);
		softbody.setMaterialLinearStiffnessCoefficient(0, 0.1f);
		SoftBodyConfig config = softbody.getConfig();
		config.kDF = 1;
		config.kDP = 0.001f; // fun factor...
		config.kPR = 2500;
		softbody.setConfig(config);
		softbody.setTotalMass(30f, true);

		createBigPlate(world, 15f, 8f);
		createLinearStair(world, new Vector3(-10f, 0f, 0f), new Vector3(2f, 1f, 5f), 0, 10);
		demoTime = 80f;
	}

	/**
	 * demo: linked soft wheels
	 */
	private void initClusterCombine(PhysicsWorld world) {
		new RigidBody(world,
			new BoxShape(world, 2f, 2f, 2f), 
			0.f,
			new MotionState(4.f, -18.f, 0.f));
		final Vector3 sz = new Vector3(2f, 4f, 2f);		
		final SoftBody softbody0 = createClusterTorus(world, 50f,
			new Vector3(0f, 8f, 0f), new Vector3(PI / 2,0, PI / 2), sz);
		final SoftBody softbody1 = createClusterTorus(world, 50f,
			new Vector3(0f, 8f, 10f), new Vector3(PI / 2, 0f, PI / 2), sz);
		final SoftBody softBodies[] = {softbody0, softbody1};
		for (int j = 0; j < 2; ++j) {
			final SoftBodyConfig config = softBodies[j].getConfig(); 
			config.kDF = 1;
			config.kDP = 0;
			config.piterations = 1;
			softBodies[j].setConfig(config);
			
			softBodies[j].setClustersMatching(0, 0.05f);
			softBodies[j].setClustersNdamping(0, 0.05f);
		}
		mMotorControl = new MotorControl(0f, 0f);
		softbody0.appendAngularJoint(new Vector3(0f, 0f, 1f), mMotorControl, softbody1);
		softbody0.appendLinearJoint(new Vector3(0f, 8f, 5f), softbody1);
		demoTime = 40f;
	}

	
//********************************************************************************
// Helper methods
//********************************************************************************
	/**
	 * create rope
	 * @param from
	 * @param mass
	 * @return
	 */
	private final SoftBody createRope(PhysicsWorld world, Vector3 from, float mass) {
		final SoftBody softbody = new RopeSoftBody(world, from, from.addNew(10f, 0f, 0f), 8, 1);
		softbody.setTotalMass(mass);
		return softbody;
	}

	/**
	 * create rigid big plate
	 * @param mass
	 * @param height
	 * @return
	 */
	private final RigidBody createBigPlate(PhysicsWorld world, float mass, float height) {
		final RigidBody rigidbody = new RigidBody(world,
			new BoxShape(world, 5f, 1f, 5f),
			mass,
			new MotionState(0f, height, 0.5f)
		);
		rigidbody.setFriction(1f);
		return(rigidbody);	
	}
	
	/**
	 * create linear stairs
	 * @param org
	 * @param sizes
	 * @param angle
	 * @param count
	 */
	private final void createLinearStair(PhysicsWorld world,
		Vector3 org, Vector3 sizes, float angle, int count) {
		
		final BoxShape shape = new BoxShape(world, sizes);
		for (int i = 0; i < count; ++i) {
			final RigidBody body = new RigidBody(world,
				shape, 0f,
				new MotionState(org.addNew(sizes.x() * i * 2f, sizes.y() * i * 2, 0f))
			);
			body.setFriction(1f);
		}	
	}
	
	/**
	 * create cluster torus
	 * @param origin
	 * @param angle
	 * @param scale
	 * @return
	 */
	private final SoftBody createClusterTorus(PhysicsWorld world,
		float mass, Vector3 origin, Vector3 angle, Vector3 scale) {

		final SoftBody softbody = MeshHelperTorus.createSoftBodyWidthConfig(
			world, mass, origin, angle, scale);
		
		softbody.generateClusters(64);			
		return softbody;
	}

}
