/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Transform.java
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

package com.serenegiant.androbulletglue.math;

import com.serenegiant.androbulletglue.interfaces.NativeObject;

public class Transform extends NativeObject {
	
	private final Vector3 mOrigin;		// center coordinates
	private final Matrix3x3 mBasis;		// transforming matrix
	
	/**
	 * default constructor
	 */
	public Transform() {
		mID = nativeCreate(0);
		mOrigin = new Vector3(nativeGetOrigin(mID));	// assign native object
		mBasis = new Matrix3x3(nativeGetBasis(mID));	// assign native object
		mBasis.setIdentity();
	}
	
	/**
	 * copy constructor
	 * @param src
	 */
	public Transform(Transform src) {
		mID = nativeCreate(src.mID);
		mOrigin = new Vector3(nativeGetOrigin(mID));
		mBasis = new Matrix3x3(nativeGetBasis(mID));
	}
	private static final native long nativeCreate(long id_trans_src);
	private static final native long nativeGetOrigin(long id_trans);
	private static final native long nativeGetBasis(long id_trans);
	
	/**
	 * constructor with origin values
	 * @param originX
	 * @param originY
	 * @param originZ
	 */
	public Transform(float originX, float originY, float originZ) {
		this();
		mOrigin.set(originX, originY, originZ);
	}
	
	/**
	 * constructor with origin values
	 * @param origin
	 */
	public Transform(Vector3 origin) {
		this();
		mOrigin.set(origin);
	}
	
	/**
	 * constructor with origin value and transform matrix
	 * @param origin
	 * @param basis
	 */
	public Transform(Vector3 origin, Matrix3x3 basis) {
		this();
		mOrigin.set(origin);
		mBasis.set(basis);
	}

	/**
	 * constructor with transform matrix
	 * @param basis
	 */
	public Transform(Matrix3x3 basis) {
		this();
		mBasis.set(basis);
	}

	/**
	 * constructor with rotation quaternion
	 * @param rotation
	 */
	public Transform(Quaternion rotation) {
		this();
		mBasis.setRotation(rotation);
	}

	/**
	 * constructor with origin and rotation quaternion
	 * @param origin
	 * @param rotation
	 */
	public Transform(Vector3 origin, Quaternion rotation) {
		this();
		mOrigin.set(origin);
		mBasis.setRotation(rotation);
	}

	/**
	 * constructor with origin and rotation quaternion
	 * @param origin
	 * @param rotation
	 */
	public Transform(float originX, float originY, float originZ, Quaternion rotation) {
		this();
		mOrigin.set(originX, originY, originZ);
		mBasis.setRotation(rotation);
	}

	protected native void nativeDestroy(long id_trans);
	
	/**
	 * setter
	 * @param src
	 * @return
	 */
	public Transform set(Transform src) {
		mOrigin.set(src.mOrigin);
		mBasis.set(src.mBasis);
		return this;
	}
	
	/**
	 * get OpenGL projection matrix
	 * @param m
	 */
	public void getOpenGLMatrix(float[] m) {
		mBasis.getOpenGLSubMatrix(m);
		final float[] org = mOrigin.get();
		m[12] = org[0];
		m[13] = org[1];
		m[14] = org[2];
		m[15] = 1f;
	}

	/**
	 * set identity
	 */
	public Transform setIdentity() {
		mBasis.setIdentity();
		mOrigin.set(0f, 0f, 0f);
		return this;
	}
	
	/**
	 * get object with setIdentity
	 * @return
	 */
	public static Transform getIdentity() {
		final Transform trans = new Transform();
		return trans.setIdentity();
	}
	
	/**
	 * set rotation
	 * @param rotation
	 */
	public void setRotation(Quaternion rotation) {
		mBasis.setRotation(rotation);
	}
	
	/**
	 * get origin
	 * @return
	 */
	public Vector3 getOrigin() {
		return mOrigin;
	}

	/**
	 * get copy object of origin
	 * @return
	 */
	public Vector3 copyOrigin() {
		return new Vector3(mOrigin);
	}
	
	/**
	 * set origin
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
	public Vector3 setOrigin(float x, float y, float z) {
		return mOrigin.set(x, y, z);
	}
	
	/**
	 * set origin
	 * @param origin
	 * @return
	 */
	public Vector3 setOrigin(Vector3 origin) {
		return mOrigin.set(origin);
	}
	
	/**
	 * get matrix
	 * @return
	 */
	public Matrix3x3 getBasis() {
		return mBasis;
	}

	/**
	 * get copy of matrix
	 * @return
	 */
	public Matrix3x3 copyBasis() {
		return new Matrix3x3(mBasis);
	}
	
	public Vector3 mult(Vector3 x) {
		nativeMultVec(0, mID, x.getID());
        return x;
	}

	public Vector3 multNew(Vector3 x) {
		final Vector3 result = new Vector3();
		nativeMultVec(result.getID(), mID, x.getID());
        return result;
	}
	private static final native int nativeMultVec(long id_vec_result, long id_trans, long id_vec);

	/**
	 * Multiply this Transform by another(this *= another)
	 * @param other
	 * @return
	 */
	public Transform mult(Transform other) {
		nativeMultTrans(0, mID, other.mID);
		return this;
	}

	/**
	 * Multiply this Transform by another(result = this * another)
	 * @param other
	 * @return
	 */
	public Transform multNew(Transform other) {
		final Transform result = new Transform();
		nativeMultTrans(result.mID, mID, other.mID);
		return result;
	}
	private static final native int nativeMultTrans(long id_trans_result, long id_trans1, long id_trans2);
	
	public Quaternion mult(Quaternion quat) {
		if (quat != null) {
			nativeMultQuat(0, mID, quat.getID());
		}
		return quat;
	}
	
	public Quaternion multNew(Quaternion quat) {
		if (quat != null) {
			final Quaternion result = new Quaternion();
			nativeMultQuat(result.getID(), mID, quat.getID());
			return result;
		} else
			return null;
	}
	private static final native int nativeMultQuat(long id_quat_result, long id_trans, long id_quat);
}
