/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: CollisionShape.java
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

package com.serenegiant.androbulletglue.interfaces;

import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.math.Vector3;


public abstract class CollisionShape extends BulletObject {

//	enum BroadphaseNativeTypes
// polyhedral convex shapes
	public static final int	BOX_SHAPE_PROXYTYPE = 0;						// 0	btBoxShape
	public static final int	TRIANGLE_SHAPE_PROXYTYPE = 1;					// 1	btTriangleShape
	public static final int	TETRAHEDRAL_SHAPE_PROXYTYPE = 2;				// 2	btBU_Simplex1to4
	public static final int	CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE = 3;		// 3
	public static final int	CONVEX_HULL_SHAPE_PROXYTYPE = 4;				// 4
	public static final int	CONVEX_POINT_CLOUD_SHAPE_PROXYTYPE = 5;			// 5
	public static final int	CUSTOM_POLYHEDRAL_SHAPE_TYPE = 6;				// 6
//implicit convex shapes
//	IMPLICIT_CONVEX_SHAPES_START_HERE,										// 7
	public static final int	SPHERE_SHAPE_PROXYTYPE = 8;						// 8	btSphereShape
	public static final int	MULTI_SPHERE_SHAPE_PROXYTYPE = 9;				// 9	btMultiSphereShape
	public static final int	CAPSULE_SHAPE_PROXYTYPE = 10;					// 10	btCapsuleShape
	public static final int	CONE_SHAPE_PROXYTYPE = 11;						// 11	btConeShape
	public static final int	CONVEX_SHAPE_PROXYTYPE = 12;					// 12	abstract
	public static final int	CYLINDER_SHAPE_PROXYTYPE = 13;					// 13	btCylinderShape
	public static final int	UNIFORM_SCALING_SHAPE_PROXYTYPE = 14;			// 14	btUniformScalingShape
	public static final int	MINKOWSKI_SUM_SHAPE_PROXYTYPE = 15;				// 15
	public static final int	MINKOWSKI_DIFFERENCE_SHAPE_PROXYTYPE = 16;		// 16
	public static final int	BOX_2D_SHAPE_PROXYTYPE = 17;					// 17	btBox2dShape
	public static final int	CONVEX_2D_SHAPE_PROXYTYPE = 18;					// 18
	public static final int	CUSTOM_CONVEX_SHAPE_TYPE = 19;					// 19
//concave shapes
//	CONCAVE_SHAPES_START_HERE,												// 20
//keep all the convex shapetype below here, for the check IsConvexShape in broadphase proxy!
	public static final int	TRIANGLE_MESH_SHAPE_PROXYTYPE = 21;				// 21	btBvhTriangleMeshShape
	public static final int	SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE = 22;		// 22	btScaledBvhTriangleMeshShape
	///used for demo integration FAST/Swift collision library and Bullet
	public static final int	FAST_CONCAVE_MESH_PROXYTYPE = 23;				// 23
	//terrain
	public static final int	TERRAIN_SHAPE_PROXYTYPE = 24;					// 24	btHeightfieldTerrarinShape
	///Used for GIMPACT Trimesh integration
	public static final int	GIMPACT_SHAPE_PROXYTYPE = 25;					// 25
	///Multimaterial mesh
	public static final int	MULTIMATERIAL_TRIANGLE_MESH_PROXYTYPE = 26;		// 26	btMultimaterialTriangleMeshShape
		
	public static final int	EMPTY_SHAPE_PROXYTYPE = 27;						// 27	btEmptyShape
	public static final int	STATIC_PLANE_PROXYTYPE = 28;					// 28	btStaticPlaneShape
	public static final int	CUSTOM_CONCAVE_SHAPE_TYPE = 29;					// 29
//	CONCAVE_SHAPES_END_HERE,												// 30

	public static final int	COMPOUND_SHAPE_PROXYTYPE = 31;					// 31	btCompoundShape

	public static final int	SOFTBODY_SHAPE_PROXYTYPE = 32;					// 32
	public static final int	HFFLUID_SHAPE_PROXYTYPE = 33;					// 33
	public static final int	HFFLUID_BUOYANT_CONVEX_SHAPE_PROXYTYPE = 34;	// 34
	public static final int	INVALID_SHAPE_PROXYTYPE = 35;					// 35

//	MAX_BROADPHASE_COLLISION_TYPES											// 36

//	upAxis constant for CAPSULE_SHAPE_PROXYTYPE/CONE_SHAPE_PROXYTYPE/CYLINDER_SHAPE_PROXYTYPE
	public static final int UPAXIS_X = 0;
	public static final int UPAXIS_Y = 1;
	public static final int UPAXIS_Z = 2;

	protected CollisionShape(PhysicsWorld world, int type) {
		super(world, type);
	}
	
	protected long createCollisionShape() {
		mID = nativeCreate();
		if (mID != 0)
			mWorld.add(this);
		return mID;
	}
	// nativeCreate in this class can not be a static method
	// because native code access fields in this object instance.
	protected final native long nativeCreate();

	/**
	 * remove this object
	 */
	@Override
	public void dispose() {
		if (mID != 0) {
			mWorld.remove(this);
		}
		super.dispose();
	}
	@Override
	protected native void nativeDestroy(long id_shape);
	
	public void setLocalScaling(Vector3 scaling) {
		nativeSetLocalScaling(mID, scaling.mID);
	}
	private static final native int nativeSetLocalScaling(long id_shape, long id_vec);
	
	public Vector3 getLocalScaling() {
		final Vector3 result = new Vector3();
		nativeGetLocalScaling(mID, result.mID);
		return result;
	}
	private static final native int nativeGetLocalScaling(long id_shape, long id_vec_result);

	public void setMargin(float margin) {
		nativeSetMargin(mID, margin);
	}
	private static final native int nativeSetMargin(long id_shape, float margin);
	
	public float getMargin() {
		return nativeGetMargin(mID);
	}
	private static final native float nativeGetMargin(long id_shape);

}
