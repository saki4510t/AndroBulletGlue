/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: BulletManager.java
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

import com.serenegiant.androbulletglue.interfaces.CollisionShape;
import android.util.Log;

public class BulletManager {
    private static final boolean DEBUG = false;	// TODO set false when production
    private static final String TAG = "BulletManager";
    private static boolean isLoaded;
    /**
     * you can use only one instance of BulletManager on each app.
     */
    private static BulletManager mBulletManager;
    
	private final List<PhysicsWorld> mPhysicsWorlds;
	private final List<CollisionShape> mCollisionShapes;
	private PhysicsWorld mDefaultPhysicsWorld;	
	protected boolean mNeedJavaResult;
	
	public static void loadBulletLib() {
    	if (!isLoaded) {
    		System.loadLibrary("androbulletglue");
    		if (DEBUG) Log.v(TAG, "loadBulletLibglue:loadLibrary");
    	}
        isLoaded = true;
	}
	
	public static synchronized BulletManager initialize() {
		return initialize(true);
	}

	/**
	 * initialize Bullet
	 * @param needJavaResult whether or not you want to use the simulation result in Java code 
	 * @return
	 */
	public static synchronized BulletManager initialize(boolean needJavaResult) {
    	loadBulletLib();
		if (mBulletManager == null) {
			mBulletManager = new BulletManager(needJavaResult);
		} else {
			mBulletManager.clearObjects(true);
			mBulletManager.mNeedJavaResult = needJavaResult;
		}
		return mBulletManager;
	}

	public static synchronized BulletManager getBulletManager() {
		return mBulletManager;
	}
	
	/**
	 * destroy bullet and all related objects
	 */
	public static synchronized void destroy() {
		if (mBulletManager != null) {
			mBulletManager.dispose();
		}
		mBulletManager = null;
	}
	
	/**
	 * constructor
	 * @param needJavaResult
	 * the necessity that the simulation result need to write back into Java object
	 * when simulation execute.
	 * if true, the simulation will be slower.
	 * if you use GLBulletActivity and GLBulletWorldSufaceViewm,
	 * you will set this flag false for better performance.
	 */
	private BulletManager(boolean needJavaResult) {
		if (DEBUG) Log.v(TAG, "Constructor");
		mNeedJavaResult = needJavaResult;
		mPhysicsWorlds = new ArrayList<PhysicsWorld>();
		mCollisionShapes = new ArrayList<CollisionShape>();
	}

	/**
	 * dispose related all objects on all physics worlds
	 * but does not dispose physics worlds themselves</br>
	 * if you want to dispose all objects including physics worlds, use #dispose
	 */
	public synchronized void clearObjects(boolean needClearCollisionShape) {
		// clear all objects on all physics world
		for (PhysicsWorld world: mPhysicsWorlds) {
			world.clearObjects();
		}
		if (needClearCollisionShape) {
			// dispose CollisionShape objects if exist
			CollisionShape collisionShape;
			final int n = mCollisionShapes.size();
			for (int i = n - 1; i >= 0; i--) {
				collisionShape = mCollisionShapes.get(i);
				if (collisionShape != null) {
					collisionShape.dispose();
				}
			}
			mCollisionShapes.clear();
		}
		// clear native physics world objects if exist
		nativeClearObjects(needClearCollisionShape);
	}
	protected native int nativeClearObjects(boolean needClearCollisionShape);

	private void dispose() {
		// clear all physics world objects
		clearObjects(true);
		// dispose PhysicsWorld objects if exist
		if (mDefaultPhysicsWorld != null) {
			mDefaultPhysicsWorld.dispose();
			mDefaultPhysicsWorld = null;
		}
		PhysicsWorld physicsWorld;
		final int n = mPhysicsWorlds.size();
		for (int i = n - 1; i >= 0; i--) {
			physicsWorld = mPhysicsWorlds.get(i);
			if (physicsWorld != null) {
				physicsWorld.dispose();
			}
		}
		mPhysicsWorlds.clear();
		// delete native objects
		nativeDestroy();
	}
	private native void nativeDestroy();
	

	/**
	 * return default PhysicsWorld instance
	 * @return PhysicsWorld(return null if not exist)
	 */
	public PhysicsWorld getDefaultPhysicsWorld() {
		try {
			return this.mDefaultPhysicsWorld;
		} catch (Exception e) {
			Log.e(TAG, "BulletManager could not initialize");
			return null;
		}
	}

	/**
	 * set PhysicsWorld as a default PhysicsWorld
	 * @param defaultPhysicsWorld
	 */
	public void setDefaultPhysicsWorld(PhysicsWorld defaultPhysicsWorld) {
		mDefaultPhysicsWorld = defaultPhysicsWorld;
	}

	/**
	 * return id of default PhysicsWorld.
	 * @return PhysicsWorld(return 0 if not exist)
	 */
	public long getDefaultPhysicsWorldId() {
		try {
			return mDefaultPhysicsWorld != null ? mDefaultPhysicsWorld.getID() : 0;
		} catch (Exception e) {
			Log.e(TAG, "BulletManager does not initialize");
			return 0;
		}
	}

	/**
	 * return all instances of registered PhysicsWorld
	 * @return SparseArray<PhysicsWorld>
	 */
	public List<PhysicsWorld> getPhysicsWorlds() {
		return mPhysicsWorlds;
	}

	/**
	 * return all instances of registered CollisionShape
	 * @return SparseArray<CollisionShape>
	 */
	public List<CollisionShape> getCollisionShapes() {
		return mCollisionShapes;
	}

	/**
	 * search CollisionShape instance which has specified id
	 * @param id_shape
	 * @return
	 */
	public CollisionShape getCollisionShape(long id_shape) {
		CollisionShape shape = null;
		for (CollisionShape obj: mCollisionShapes) {
			if (obj.getID() == id_shape) {
				shape = obj;
				break;
			}
		}
		return shape;
	}

}
