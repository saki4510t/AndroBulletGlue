/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: VehicleBody.java
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

package com.serenegiant.androbulletglue;

import com.serenegiant.androbulletglue.interfaces.CollisionShape;
import com.serenegiant.androbulletglue.interfaces.NativeObject;
import com.serenegiant.androbulletglue.math.Transform;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.shape.BoxShape;
import com.serenegiant.androbulletglue.shape.CompoundShape;
import com.serenegiant.androbulletglue.shape.CylinderShape;

/**
 *  Vehiclebody object</br>
 *  <b>this object don't register to the BulletManager now
 *  and you should dispose with calling #dispse by yourself</b>.
 *  otherwise, if you set null or leave to VM, that will be cause a crash.
 *  because no one knows when finalizer is called.
 */
public class VehicleBody extends NativeObject {

	protected static float gEngineForce = 0.f;
	protected static float gBreakingForce = 0.f;

	protected static float gMaxEngineForce = 1000.f;	// this should be engine/velocity dependent
	protected static float gMaxBreakingForce = 100.f;
	protected static float gVehicleMass = 800.f;
	protected static float gVehicleSteering = 0f;
	protected static float gSteeringIncrement = 0.04f;
	protected static float gSteeringClamp = 0.3f;
	protected static float gWheelRadius = 0.5f;
	protected static float gWheelWidth = 0.4f;
	protected static float gWheelFriction = 1000;	// BT_LARGE_FLOAT;
	protected static float gSuspensionStiffness = 20.f;
	protected static float gSuspensionDamping = 2.3f;
	protected static float gSuspensionCompression = 4.4f;
	protected static float gRollInfluence = 0.1f;	// 1.0f;
	protected static float gSuspensionRestLength = 0.6f;
	
	private final PhysicsWorld mWorld;
	private final CollisionShape mWheelShape;
	// native code access this field and you should not remove and rename
	private final RigidBody mCarChassis;
	

	/**
	 * constructor
	 */
	public VehicleBody(PhysicsWorld world) {
		this(world, null, false);
	}
	
	/**
	 * constructor
	 * @param positionOffset (FIXME this parameter does not work well)
	 */
	public VehicleBody(PhysicsWorld world, Vector3 positionOffset) {
		this(world, positionOffset, false);
	}
	
	/**
	 * constructor
	 * @param positionOffset (FIXME this parameter does not work well)
	 * @param force_zaxis_up
	 */
	public VehicleBody(PhysicsWorld world, Vector3 positionOffset, boolean force_zaxis_up) {
		mWorld = world;
		// create body itself
		mCarChassis = getChassisShape(force_zaxis_up, positionOffset);
		mCarChassis.setDamping(0.2f, 0.2f);
		
		// create wheel shape(share for each wheel)
		mWheelShape = new CylinderShape(world,
			new Vector3(gWheelWidth, gWheelRadius, gWheelRadius),
			CylinderShape.UPAXIS_X);
		
		// create native object
		mID = nativeCreate(mWorld.getID(), force_zaxis_up);
	}
	// nativeCreate in this class can not be a static method
	// because native code access fields in this object instance.
	private final native long nativeCreate(long id_physicsworld, boolean force_zaxis_up);
	
	@Override
	protected native void nativeDestroy(long id_vehicle);
	
	public RigidBody getRigidBody() {
		return mCarChassis;
	}
	
	public void renderFrame() {
		nativeRender(mWorld.getID(), mID, mWheelShape.getID());
	}
	private native void nativeRender(long id_physicsworld, long id_vehicle, long id_shape_wheel); 
	
	/**
	 * create car chassis
	 * @param force_zaxis_up
	 * @param initialPosition (FIXME this parameter does not work well)
	 * @return
	 */
	protected RigidBody getChassisShape(boolean force_zaxis_up, Vector3 initialPosition) {

		final CompoundShape compound = new CompoundShape(mWorld);
		// localTrans effectively shifts the center of mass with respect to the chassis
		final Transform localTrans = new Transform();
		localTrans.setIdentity();
		
		CollisionShape chassisShape;
		if (force_zaxis_up) {
			chassisShape = new BoxShape(mWorld, 1.f, 2.f, 0.5f);	// width=1x2=2, length=2x2=4, height=0.5x2=1.2
			localTrans.setOrigin(0, 0, gWheelRadius * 2);
		} else {
			chassisShape = new BoxShape(mWorld, 1.f, 0.5f, 2.f);
			localTrans.setOrigin(0, gWheelRadius * 2, 0);
		}

		compound.addChildShape(localTrans, chassisShape);

		// FIXME can't change initial position of vehicle...
		return new RigidBody(mWorld, compound, gVehicleMass, new MotionState(initialPosition));
	}
}
