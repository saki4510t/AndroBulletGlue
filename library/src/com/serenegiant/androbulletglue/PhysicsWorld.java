/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: PhysicsWorld.java
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

package com.serenegiant.androbulletglue;

import java.util.ArrayList;
import java.util.List;

import com.serenegiant.androbulletglue.broadphase.DbvtBroadphase;
import com.serenegiant.androbulletglue.collision.DefaultCollisionConfiguration;
import com.serenegiant.androbulletglue.collision.DefaultCollisionDispatcher;
import com.serenegiant.androbulletglue.dynamicsWorld.DiscreteDynamicsWorld;
import com.serenegiant.androbulletglue.interfaces.Broadphase;
import com.serenegiant.androbulletglue.interfaces.CollisionConfiguration;
import com.serenegiant.androbulletglue.interfaces.CollisionDispatcher;
import com.serenegiant.androbulletglue.interfaces.CollisionShape;
import com.serenegiant.androbulletglue.interfaces.Constraint;
import com.serenegiant.androbulletglue.interfaces.DynamicsWorld;
import com.serenegiant.androbulletglue.interfaces.NativeObject;
import com.serenegiant.androbulletglue.interfaces.SoftBody;
import com.serenegiant.androbulletglue.interfaces.Solver;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.solver.SequentialImpulseConstraintSolver;

public class PhysicsWorld extends NativeObject {
	// native code access these fields and you should not remove and rename
	private final BulletManager mBulletManager;
	public final CollisionConfiguration collisionConfiguration;
	public final CollisionDispatcher collisionDispatcher;
	public final Solver solver;
	public final Broadphase broadPhase;
	public final DynamicsWorld dynamicsWorld;
	public final Vector3 gravity;

	//
	private final List<RigidBody> mRigidBodies = new ArrayList<RigidBody>();
	private final List<SoftBody> mSoftBodies = new ArrayList<SoftBody>();
	private final List<Constraint> mConstraints = new ArrayList<Constraint>();

	/**
	 * constructor without gravity
	 * @param bullet
	 */
	public PhysicsWorld() {
		
		this(new Vector3());
	}
	
	/**
	 * constructor with gravity(only y axis), it looks like on the earth
	 * @param bullet
	 * @param gravity_y: set -9.8f for the earth
	 */
	public PhysicsWorld(float gravity_y) {
		
		this(new Vector3(0f, gravity_y, 0f));
	}

	/**
	 * constructor
	 * @param bullet
	 * @param gravity_x
	 * @param gravity_y
	 * @param gravity_z
	 */
	public PhysicsWorld(float gravity_x, float gravity_y, float gravity_z) {
		
		this(new Vector3(gravity_x, gravity_y, gravity_z));
	}
	
	/**
	 * constructor
	 * @param bullet
	 * @param gravity
	 */
	public PhysicsWorld(Vector3 gravity) {
		
		this(new DefaultCollisionConfiguration(),
			new DefaultCollisionDispatcher(),
			new SequentialImpulseConstraintSolver(),
			new DbvtBroadphase(),
			new DiscreteDynamicsWorld(),
			gravity);
	}
	
	/**
	 * constructor
	 * @param bullet
	 * @param collisionConfiguration
	 * @param collisionDispatcher
	 * @param solver: constraint solver for rigid objects
	 * @param broadPhase
	 * @param dynamicsWorld
	 * @param gravity
	 */
	public PhysicsWorld(
		CollisionConfiguration collisionConfiguration,
		CollisionDispatcher collisionDispatcher,
		Solver solver,
		Broadphase broadPhase,
		DynamicsWorld dynamicsWorld,
		Vector3 gravity) {
		
		mBulletManager = BulletManager.getBulletManager();
		
		this.collisionConfiguration = collisionConfiguration;
		this.collisionDispatcher = collisionDispatcher;
		this.solver = solver;
		this.broadPhase = broadPhase;
		this.dynamicsWorld = dynamicsWorld;
		this.gravity = gravity;

		mID = nativeCreate();
		mBulletManager.getPhysicsWorlds().add(this);
		if (mBulletManager.getDefaultPhysicsWorld() == null)
			mBulletManager.setDefaultPhysicsWorld(this);	
	}
	// nativeCreate in this class can not be a static method
	// because native code access fields in this object instance.
	protected final native long nativeCreate();

	@Override
	public void dispose() {
		if (mID != 0) {
			mBulletManager.getPhysicsWorlds().remove(this);
		}
		super.dispose();
	}
	@Override
	protected native void nativeDestroy(long id_physicsworld);
		
	/**
	 * clear all objects related to this PhysicsWorld instance
	 * the instances of CollisionShape and descendent class never clear with this method.
	 * insted use {@link BulletManager#clearObjects(true)}
	 */
	public void clearObjects() {
		// dispose Constraint objects if exist
		Constraint constraint;
		int n = mConstraints.size();
		for (int i = n - 1; i >= 0; i--) {
			constraint = mConstraints.get(i);
			if (constraint != null) {
				constraint.dispose();
			}
		}
		mConstraints.clear();
		
		// dispose SoftBody objects if exist
		SoftBody softBody;
		n = mSoftBodies.size();
		for (int i = n - 1; i >= 0; i--) {
			softBody = mSoftBodies.get(i);
			if (softBody != null) {
				softBody.dispose();
			}
		}
		mSoftBodies.clear();
		
		// dispose RigidBody objects if exist
		RigidBody rigidBody;
		n = mRigidBodies.size();
		for (int i = n - 1; i >= 0; i--) {
			rigidBody = mRigidBodies.get(i);
			if (rigidBody != null) {
				rigidBody.dispose();
			}
		}
		mRigidBodies.clear();
				
		nativeClearObjects(mID);
	}
	private static final native int nativeClearObjects(long id_physicsworld);
	
	public void setActive(boolean isActive) {
		nativeSetActiveAll(mID, isActive);
	}
	private native int nativeSetActiveAll(long id_physicsworld, boolean isActive);

	/**
	 * execute simulation
	 * @param execTime:	step time of calculation[seconds]
	 * @param subSteps:	division number of step
	 * @return number of objects that the value changed in this call
	 */
	public int doSimulation(float execTime, int subSteps) {
		return nativeDoSimulation(mID, execTime, subSteps);
	}

	protected final native int nativeDoSimulation(long worldId, float execTime, int subSteps); 

	/**
	 * execute variable time simulation with default PhysicsWorld
	 * @return number of objects that the value changed in this call
	 */
	public int doSimulation() {
		return nativeStepSimulation(mID);
	}
	protected final native int nativeStepSimulation(long worldId);

	public void add(Constraint constraint) {
		if ((constraint != null) && (constraint.getID() != 0)) {
			mConstraints.add(constraint);
		} else {
			throw new NullPointerException("couldn't create native object");
		}
	}
	
	public void remove(Constraint constraint) {
		if (mConstraints.contains(constraint)) {
			mConstraints.remove(constraint);
		}
	}

	public void add(RigidBody rigidBody) {
		if ((rigidBody != null) && (rigidBody.getID() != 0)) {
			mRigidBodies.add(rigidBody);
		} else {
			throw new NullPointerException("couldn't create native object");
		}
	}
	
	public void remove(RigidBody rigidBody) {
		if (mRigidBodies.contains(rigidBody)) {
			mRigidBodies.remove(rigidBody);
		}
	}

	public void add(SoftBody softBody) {
		if ((softBody != null) && (softBody.getID() != 0)) {
			mSoftBodies.add(softBody);
		} else {
			throw new NullPointerException("couldn't create native object");
		}
	}
	
	public void remove(SoftBody softBody) {
		if (mSoftBodies.contains(softBody)) {
			mSoftBodies.remove(softBody);
		}
	}

	public void add(CollisionShape shape) {
		if ((shape != null) && (shape.getID() != 0)) {
			mBulletManager.getCollisionShapes().add(shape);
		} else {
			throw new NullPointerException("couldn't create native object");
		}
	}
	
	public void remove(CollisionShape shape) {
		final List<CollisionShape> objects = mBulletManager.getCollisionShapes();
		if (objects.contains(shape)) {
			objects.remove(shape);
		}
	}

	/**
	 * return all instances of registered RigidBody
	 * @return SparseArray<RigidBody>
	 */
	public List<RigidBody> getRigidBodies() {
		return mRigidBodies;
	}

	/**
	 * search RigidBody instance which has specified id
	 * @param id_rigidbody
	 * @return
	 */
	public RigidBody getRigidBody(long id_rigidbody) {
		RigidBody body = null;
		for (RigidBody obj: mRigidBodies) {
			if (obj.getID() == id_rigidbody) {
				body = obj;
				break;
			}
		}
		return body;
	}
	
	/**
	 * return all instances of registerd SoftBody
	 * @return SparseArray<SoftBody>
	 */
	public List<SoftBody> getSoftBodies() {
		return mSoftBodies;
	}

	/**
	 * search SoftBody instance which has specified id
	 * @param id_softbody
	 * @return
	 */
	public SoftBody getSoftBody(long id_softbody) {
		SoftBody body = null;
		for (SoftBody obj: mSoftBodies) {
			if (obj.getID() == id_softbody) {
				body = obj;
				break;
			}
		}
		return body;
	}

	/**
	 * return all instances of registered Constraint
	 * @return SparseArray<Constraint>
	 */
	public List<Constraint> getConstraints() {
		return mConstraints;
	}

	/**
	 * search Constraint instance which has specified id
	 * @param id_constraint
	 * @return
	 */
	public Constraint getConstraint(long id_constraint) {
		Constraint constraint = null;
		for (Constraint obj: mConstraints) {
			if (obj.getID() == id_constraint) {
				constraint = obj;
				break;
			}
		}
		return constraint;
	}

	public BulletManager getBulletManager() {
		return mBulletManager;
	}

	public boolean needJavaResult() {
		return mBulletManager.mNeedJavaResult;
	}
}
