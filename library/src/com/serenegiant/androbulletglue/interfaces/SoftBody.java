/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: SoftBody.java
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

package com.serenegiant.androbulletglue.interfaces;

import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.math.Quaternion;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.math.Transform;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.softbody.MotorControl;
import com.serenegiant.androbulletglue.softbody.SoftBodyConfig;
import com.serenegiant.androbulletglue.softbody.SoftBodyMaterial;

public abstract class SoftBody extends Body {

	public static final int SOFTBODY_ROPE = 0;
	public static final int SOFTBODY_PATCH = 1;
	public static final int SOFTBODY_PATCHUV = 2;
	public static final int SOFTBODY_ELLIPSOID = 3;
	public static final int SOFTBODY_FROM_TRIMESH = 4;
	public static final int SOFTBODY_FROM_CONVEXHULL = 5;
	public static final int SOFTBODY_FROM_TETGENDATA = 6;
	
	public static final int CONFIG_COLLISIONS_RVSmask	=	0x000f;	// Rigid versus soft mask
	public static final int CONFIG_COLLISIONS_SDF_RS	=	0x0001;	// SDF based rigid vs soft
	public static final int CONFIG_COLLISIONS_CL_RS		=	0x0002; // Cluster vs convex rigid vs soft
	public static final int CONFIG_COLLISIONS_SVSmask	=	0x0030;	// Rigid versus soft mask		
	public static final int CONFIG_COLLISIONS_VF_SS		=	0x0010;	// Vertex vs face soft vs soft handling
	public static final int CONFIG_COLLISIONS_CL_SS		=	0x0020; // Cluster vs cluster soft vs soft handling
	public static final int CONFIG_COLLISIONS_CL_SELF 	=	0x0040; // Cluster soft body self collision

	public static final int MATERIAL_FLAG_DEBUGDRAW = 0x0001;
	
	private final SoftBodyConfig mConfig = new SoftBodyConfig();
	public MotorControl mMotorControl;

	public SoftBody(PhysicsWorld world, int type) {
		super(world, type);
	}
	
	/**
	 * create native object and register it
	 */
	protected void createSoftBody() {
		mID = nativeCreate(mPhysicsWorldID, mType);
		mWorld.add(this);
	}
	// nativeCreate in this class can not be a static method
	// because native code access fields in this object instance.
	private final native long nativeCreate(long worldID, int type);
	
	/**
	 * dispose
	 */
	@Override
	public void dispose() {
		if (mID != 0) {
			mWorld.remove(this);
		}
		super.dispose();
	}
	@Override
	protected final native void nativeDestroy(long id_softBody);	

	public void setTotalMass(float mass) {
		nativeSetTotalMass(mID, mass, false);
	}
	public void setTotalMass(float mass, boolean fromFaces) {
		nativeSetTotalMass(mID, mass, fromFaces);
	}
	private static final native void nativeSetTotalMass(long id_softBody, float mass, boolean fromFaces);
	
	/**
	 * return number of nodes
	 * @return
	 */
	public final int getNodeNum() {
		return nativeGetNodeNum(mID);
	}
	private static final native int nativeGetNodeNum(long id_softBody);
	
	/**
	 * append an anchor of rigidBody to the tail of this softBody
	 * @param body
	 */
	public void appendAnchor(RigidBody body) {
		appendAnchor(nativeGetNodeNum(mID) - 1, body, false, 1f);
	}
	
	/**
	 * append an anchor of rigidBody to the specific node of this softBody
	 * @param node
	 * @param body
	 */
	public void appendAnchor(int node, RigidBody body) {
		appendAnchor(node, body, false, 1f);
	}

	/**
	 * append an anchor of rigidBody to the specific node of this softBody
	 * @param node
	 * @param body
	 * @param disableCollisionBetweenLinkedBodies
	 */
	public void appendAnchor(int node, RigidBody body,
		boolean disableCollisionBetweenLinkedBodies) {

		appendAnchor(node, body, null, disableCollisionBetweenLinkedBodies, 1f);
	}
	/**
	 * append an anchor of rigidBody to the specific node of this softBody
	 * @param node
	 * @param body
	 * @param disableCollisionBetweenLinkedBodies
	 * @param influence
	 */
	public void appendAnchor(int node, RigidBody body,
		boolean disableCollisionBetweenLinkedBodies, float influence) {
		
		appendAnchor(node, body, null, disableCollisionBetweenLinkedBodies, influence);
	}
	/**
	 * append an anchor of rigidBody to the specific node of this softBody
	 * @param node
	 * @param body
	 * @param localPivot
	 * @param disableCollisionBetweenLinkedBodies
	 * @param influence
	 */
	public void appendAnchor(int node, RigidBody body, Vector3 localPivot,
		boolean disableCollisionBetweenLinkedBodies, float influence) {
		
		nativeAppendAnchor(mID, body.mID, node,
			localPivot != null ? localPivot.mID : 0,
			disableCollisionBetweenLinkedBodies, influence);
	}
	private static final native void nativeAppendAnchor(
		long id_softBody, long id_rigidBody, int nodeNo,
		long id_vec_pivot, boolean disableCollisionBetweenLinkedBodies, float influence);
	
	/*
	 * constant for selecting solver iteration type
	 */
	private static final int SOLVER_ITERATIONS_VELOCITIES = 0;	// Velocities solver iterations
	private static final int SOLVER_ITERATIONS_POSITIONS = 1;	// Positions solver iterations
	private static final int SOLVER_ITERATIONS_DRIFT = 2;		// Drift solver iterations
	private static final int SOLVER_ITERATIONS_CLUSTER = 3;		// Cluster solver iterations

	public void setCfgVelocitiesIterations(int iterations) {
		nativeSetCfgSolverIterations(mID, SOLVER_ITERATIONS_VELOCITIES, iterations);
	}
	public void setCfgPositionsIterations(int iterations) {
		nativeSetCfgSolverIterations(mID, SOLVER_ITERATIONS_POSITIONS, iterations);
	}
	public void setCfgDriftIterations(int iterations) {
		nativeSetCfgSolverIterations(mID, SOLVER_ITERATIONS_DRIFT, iterations);
	}
	public void setCfgClusterIterations(int iterations) {
		nativeSetCfgSolverIterations(mID, SOLVER_ITERATIONS_CLUSTER, iterations);
	}
	private static final native void nativeSetCfgSolverIterations(long id_softbody, int type, int piterations);

	/*
	 * material
	 */
	public void setMaterialLinearStiffnessCoefficient(int ix, float klst) {
		nativeSetMaterialLinearStiffnessCoefficient(mID, ix, klst);
	}
	private static final native void nativeSetMaterialLinearStiffnessCoefficient(long id_softbody, int ix, float klst);

	public void setMaterialAngularStiffnessCoefficient(int ix, float kast) {
		nativeSetMaterialAngularStiffnessCoefficient(mID, ix, kast);
	}
	private static final native void nativeSetMaterialAngularStiffnessCoefficient(long id_softbody, int ix, float kast);
	
	public void setMaterialVolumeStiffnessCoefficient(int ix, float kvst) {
		nativeSetMaterialVolumeStiffnessCoefficient(mID, ix, kvst);
	}
	private static final native void nativeSetMaterialVolumeStiffnessCoefficient(long id_softbody, int ix, float kvst);
	
	/**
	 * Append material
	 * @return
	 */
	public SoftBodyMaterial appendMaterial() {
		final SoftBodyMaterial material = new SoftBodyMaterial();
		nativeAppendMaterial(mID, material);
		return material;
	}
	private static final native void nativeAppendMaterial(long id_softBody, SoftBodyMaterial material);
	
	/**
	 * Generate bending constraints based on distance in the adjency graph
	 * @param distance
	 * @param material
	 */
	public void generateBendingConstraints(int distance, SoftBodyMaterial material) {
		if (material != null) {
			nativeGenerateBendingConstraints(mID, distance, material);
		} else {
			throw new NullPointerException();
		}
	}
	private static final native int nativeGenerateBendingConstraints(long id_softbody, int distance, SoftBodyMaterial material);

	/*
	 * Clusters 
	 */
	public void setClustersMatching(int ix, float matching) {
		nativeSetClustersMatching(mID, ix, matching);
	}
	private static final native int nativeSetClustersMatching(long id_softbody, int ix, float matching);
	
	public void setClustersNdamping(int ix, float damping) {
		nativeSetClustersNdamping(mID, ix, damping);
	}
	private static final native int nativeSetClustersNdamping(long id_softbody, int ix, float damping);

	/**
	 * Generate clusters (K-mean)</br> 
	 * generateClusters with k=0 will create a convex cluster for each tetrahedron or triangle</br>
	 * otherwise an approximation will be used (better performance)</br>
	 * @param k
	 */
	public void generateClusters(int k) {
		nativeGenerateClusters(mID, k, 8192);
	}
	public void generateClusters(int k, int maxiterations) {
		nativeGenerateClusters(mID, k, maxiterations);
	}
	private static final native int nativeGenerateClusters(long id_softbody, int k, int maxiterations);

	/*
	 * m_cfg
	 */
	public SoftBodyConfig getConfig() {
		nativeGetConfig(mID, mConfig);
		return mConfig;
	}
	private static final native int nativeGetConfig(long id_softbody, SoftBodyConfig config);
	
	public void setConfig(SoftBodyConfig config) {
		if (config != null)
			nativeSetConfig(mID, config);
		else
			throw new NullPointerException();
	}
	private static final native int nativeSetConfig(long id_softbody, SoftBodyConfig config);
	
	/**
	 * Randomize constraints to reduce solver bias
	 */
	public void randomizeConstraints() {
		nativeRandomizeConstraints(mID);
	}
	private static final native int nativeRandomizeConstraints(long id_softbody);
	
	/**
	 * @param transform
	 */
	public void transform(Transform transform) {
		nativeTransform(mID, transform.mID);
	}
	private static final native int nativeTransform(long id_softbody, long id_trans);
	
	/**
	 * @param angle_x
	 * @param angle_y
	 * @param angle_z
	 */
	public void rotate(float angle_x, float angle_y, float angle_z) {
		nativeRotate(mID, angle_x, angle_y, angle_z);
	}
	private static final native int nativeRotate(long id_softbody, float angle_x, float angle_y, float angle_z);
	
	/**
	 * @param angle
	 */
	public void rotate(Vector3 angle) {
		nativeRotateVec(mID, angle.mID);
	}
	private static final native int nativeRotateVec(long id_softbody, long id_vec_angle); 

	/**
	 * @param rot
	 */
	public void rotate(Quaternion rot) {
		nativeRotateQuat(mID, rot.mID);
	}
	private static final native int nativeRotateQuat(long id_softbody, long id_quat_rot);
	
	/**
	 * @param dx
	 * @param dy
	 * @param dz
	 */
	public void translate(float dx, float dy, float dz) {
		nativeTranslate(mID, dx, dy, dz);
	}
	private static final native int nativeTranslate(long id_softbody, float dx, float dy, float dz);
	
	/**
	 * @param delta
	 */
	public void translate(Vector3 delta) {
		nativeTranslateVec(mID, delta.mID);
	}
	private static final native int nativeTranslateVec(long id_softbody, long id_vec);
	
	/**
	 * @param scale_x
	 * @param scale_y
	 * @param scale_z
	 */
	public void scale(float scale_x, float scale_y, float scale_z) {
		nativeScale(mID, scale_x, scale_y, scale_z);
	}
	private static final native int nativeScale(long id_softbody, float scale_x, float scale_y, float scale_z);
	/**
	 * @param scale
	 */
	public void scale(Vector3 scale) {
		nativeScaleVec(mID, scale.mID);
	}
	private static final native int nativeScaleVec(long id_softbody, long id_vec_scale);

	/**
	 * @param margin
	 */
	public void setCollisionShapeMargin(float margin) {
		nativeSetCollisionShapeMargin(mID, margin);
	}
	private static final native int nativeSetCollisionShapeMargin(long id_softbody, float margin);

	public void appendLinearJoint(Vector3 position, Body body) {
		nativeAppendLinearJoint(mID, position.mID, body.mID);
	}
	private static final native void nativeAppendLinearJoint(long id_softbody, long id_vec_pos, long bodyId);

	/**
	 * @param axis
	 * @param body
	 */
	public void appendAngularJoint(Vector3 axis, Body body) {
		nativeAppendAngularJoint(mID, axis.mID, 0, body.mID);
	}
	/**
	 * @param axis
	 * @param motorControl
	 * @param body
	 */
	public void appendAngularJoint(Vector3 axis, MotorControl motorControl, Body body) {
		nativeAppendAngularJoint(mID, axis.mID, motorControl != null ? motorControl.mID : 0, body.mID);
	}
	private static final native void nativeAppendAngularJoint(long id_softbody, long id_vec_axis, long id_motor, long id_body);

/*	public class Joint extends TypedObject {
		public int eType;
		public float specs_erp = 1f;
		public float specs_cfm = 1f;
		public float specs_split = 1f;
		
		public Vector3[] m_refs = new Vector3[2];
		public float m_cfm;
		public float m_erp;
		public float m_split;
		public float m_drift;
		public float m_sdrift;
		public float m_massmatrix;
		public boolean m_delete;
	} */
	
/*	public class LJoint extends Joint {
		public Vector3 specs_position;
		public Vector3[] m_rpos = new Vector3[2];
	} */
	
/*	public class AJoint extends Joint {
		public Vector3 specs_axis;
		public Vector3[] m_axis = new Vector3[2];
	} */
}
