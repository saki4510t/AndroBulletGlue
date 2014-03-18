/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Quaternion.java
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

public class Quaternion extends NativeObject {

	protected Quaternion(int id_src) {
		mID = id_src;
		isAsigned = true;
	}
	
	public Quaternion() {
		mID = nativeCreate(0);
	}
	private static final native long nativeCreate(long id_quat_src);
	
	public Quaternion(float x, float y, float z, float w) {
		mID = nativeCreate(0);
		nativeSet4(mID, x, y, z, w);
	}

	protected final native void nativeDestroy(long id_quat);
	
	
	/**
	 * copy constructor
	 * @param quat
	 */
	public Quaternion(Quaternion quat) {
		mID = nativeCreate(quat.mID);
	}

	public Quaternion(float[] quat) {
		mID = nativeCreate(0);
		set(quat);
	}

	public Quaternion(float yaw, float pitch, float roll) {
		mID = nativeCreate(0);
		setEuler(yaw, pitch, roll); 
	}

	public Quaternion(Vector3 axis, float angle) { 
		mID = nativeCreate(0);
		setRotation(axis, angle); 
	}

	public Quaternion set(float x, float y, float z, float w) {
		nativeSet4(mID, x, y, z, w);
		return this;
	}
	private static final native int nativeSet4(long id_quat, float x, float y, float z, float w);

	public Quaternion set(Quaternion quat) {
		if (quat != null) {
			nativeSetQuat(mID, quat.mID);
		}
		return this;
	}
	private static final native int nativeSetQuat(long id_quat, long id_quat_src);

	/**
	 * set rotation
	 * @param axis
	 * @param _angle
	 */
	public void setRotation(Vector3 axis, float angle) {
		if (axis != null) {
			nativeSetRotation(mID, axis.getID(), angle);
		}
	}
	private static final native int nativeSetRotation(long id_quat, long id_vec_axis, float angle);

	/**
	 * set euler rotation (euler YXZ) by radian
	 * @param angle y() is yaw(angle around Y), x() is pitch(angle around X), z() is roll(angle around Z)
	 */
	public void setEuler(Vector3 angle) {
		nativeSetEulerVec(mID, angle.getID());
	}
	private static final native int nativeSetEulerVec(long id_quat, long id_vec);

	/**
	 * set euler rotation (euler YXZ) by radian
	 * @param yaw Angle around Y
	 * @param pitch Angle around X
	 * @param roll Angle around Z
	 */
	public void setEuler(float yaw, float pitch, float roll) {
		nativeSetEuler(mID, yaw, pitch, roll);
	}
	private static final native int nativeSetEuler(long id_quat, float yaw, float pitch, float roll);

	/**
	 * set euler rotation (euler ZYX) by radian
	 * @param angle z() is yaw(angle around Z), y() is pitch(angle around Y), x() is roll(angle around X)
	 */
	public void setEulerZYX(Vector3 angle) {
		nativeSetEulerZYXVec(mID, angle.getID());
	}
	private static final native int nativeSetEulerZYXVec(long id_quat, long id_vec);

	/**
	 * set euler rotation (euler ZYX) by radian
	 * @param yaw Angle around Z
	 * @param pitch Angle around Y
	 * @param roll Angle around X
	*/
	public void setEulerZYX(float yaw, float pitch, float roll) {
		nativeSetEulerZYX(mID, yaw, pitch, roll);
	}
	private static final native int nativeSetEulerZYX(long id_quat, float yaw, float pitch, float roll);

	public Quaternion set(int ix, float v) {
		nativeSetA(mID, ix, v);
		return this;
	}
	private static final native int nativeSetA(long id_quat, int ix, float v);
	
	public float get(int axis) {
		return nativeGetA(mID, axis);
	}
	private static final native float nativeGetA(long id_quat, int ix);
	
	private float[] mTempArray;
	public float[] get() {
		if (mTempArray == null)
			mTempArray = new float[4];
		nativeGetArray(mID, mTempArray);
		return mTempArray;
	}
	private static final native int nativeGetArray(long id_quat, float[] array);
	
	public Quaternion set(float[] v) {
		if (v != null && v.length >= 4)
			nativeSetArray(mID, v);
		return this;
	}
	private static final native int nativeSetArray(long id_quat, float[] array);
	
	public float x() {
		return nativeGetA(mID, 0);
	}
	
	public float y() {
		return nativeGetA(mID, 1);
	}
	
	public float z() {
		return nativeGetA(mID, 2);
	}
	
	public float w() {
		return nativeGetA(mID, 3);
	}
	
	public void setX(float x) {
		nativeSetA(mID, 0, x);
	}
	
	public void setY(float x) {
		nativeSetA(mID, 0, x);
	}
	
	public void setZ(float x) {
		nativeSetA(mID, 0, x);
	}
	
	public void setW(float x) {
		nativeSetA(mID, 0, x);
	}
}
