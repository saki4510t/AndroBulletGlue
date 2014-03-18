/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Vector3.java
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


public class Vector3 extends NativeObject {
	private static final String TAG ="Vector3";
	
	public static final float TO_RADIAN = (1 / 180.0f) * (float)Math.PI;
	public static final float TO_DEGREE = (1 / (float)Math.PI) * 180.0f;

	protected final float[] vector = new float[4];
	
	/**
	 * internal constructor to asign native btVector3 object
	 * @param native_id
	 */
	protected Vector3(long id_vec) {
		mID = id_vec;
		isAsigned = true;
	}
	
	public Vector3() {
		mID = nativeCreate(0);
	}
	private static final native long nativeCreate(long id_vec_src);
	
	/**
	 * constructor with values
	 * @param x
	 * @param y
	 * @param z
	 */
	public Vector3(float x, float y, float z) {
		mID = nativeCreate(0);
		set(x, y, z, 0f);
	}

	/**
	 * copy constructor
	 * @param vec
	 * @throws NullPointerException if vec is null
	 */
	public Vector3(Vector3 vec) {
		mID = nativeCreate(vec.mID);
	}

	/**
	 * constructor with values
	 * @param mat if mat is null, create blank Vector3 object
	 */
	public Vector3(float[] mat) {
		mID = nativeCreate(0);
		if ((mat != null) && (mat.length >= 3)) {
			set(mat[0], mat[1], mat[2], mat.length >= 4 ? mat[3] : 0f);
		}
	}
	@Override
	protected final native void nativeDestroy(long id);
	
	/**
	 * setter
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
	public Vector3 set(float x, float y, float z) {
		nativeSet3(mID, x, y, z);
		return this;
	}
	private static final native int nativeSet3(long id_vec, float x, float y, float z);

	/**
	 * setter
	 * @param x
	 * @param y
	 * @param z
	 * @param w
	 * @return
	 */
	public Vector3 set(float x, float y, float z, float w) {
		nativeSet4(mID, x, y, z, w);
		return this;
	}
	private static final native int nativeSet4(long id_vec, float x, float y, float z, float w);

	/**
	 * set value from other Vector3 object
	 * @param src
	 * @return
	 */
	public Vector3 set(Vector3 src) {
		if (src != null)
			nativeSetVec(mID, src.mID);
		return this;
	}
	private static final native int nativeSetVec(long id_vec_dest, long id_vec_src);

	/**
	 * set a value 
	 * @param axis
	 * @param v
	 * @return
	 */
	public Vector3 set(int axis, float v) {
		nativeSetA(mID, axis, v);
		return this;
	}
	private static final native int nativeSetA(long id_vec, int axis, float v);

	/**
	 * get a value
	 * @param axis
	 * @return
	 */
	public float get(int axis) {
		return nativeGetA(mID, axis);
	}
	private static final native float nativeGetA(long id_vec, int axis);
	
	/**
	 * get values array
	 * @return
	 */
	public float[] get() {
		nativeGetArray(mID, vector);
		return vector;
	}
	private static final native int nativeGetArray(long id_vec, float[] array);

	/**
	 * set values from float array
	 * @param v
	 * @return
	 */
	public Vector3 set(float[] v) {
		if (v != null && v.length >= 3)
			nativeSetArray(mID, v);
		return this;
	}
	private static final native int nativeSetArray(long id_vec, float[] array);
	
	/**
	 * get x value
	 * @return
	 */
	public float x() {
		return nativeGetA(mID, 0);
	}

	/**
	 * get y value
	 * @return
	 */
	public float y() {
		return nativeGetA(mID, 1);
	}
	
	/**
	 * get z value
	 * @return
	 */
	public float z() {
		return nativeGetA(mID, 2);
	}
	
	/**
	 * get w value
	 * @return
	 */
	public float w() {
		return nativeGetA(mID, 3);
	}

	/**
	 * set x value(this is equivalent set(0, value))
	 * @param x
	 */
	public void setX(float x) {
		nativeSetA(mID, 0, x);
	}

	/**
	 * set y value(this is equivalent set(1, value))
	 * @param y
	 */
	public void setY(float y) {
		nativeSetA(mID, 1, y);
	}

	/**
	 * set z value(this is equivalent set(2, value))
	 * @param z
	 */
	public void setZ(float z) {
		nativeSetA(mID, 2, z);
	}

	/**
	 * set w value(this is equivalent set(3, value))
	 * @param w
	 */
	public void setW(float w) {
		nativeSetA(mID, 3, w);
	}

	/**
	 * compare this and v and set this maximum Vector3 object
	 * @param v
	 * @return
	 */
	public Vector3 setMax(Vector3 v) {
		nativeSetMax(mID, v.mID);
		return this;
	}
	private static final native int nativeSetMax(long id_vec1, long id_vec2);

	/**
	 * compare this and v and set this minimum Vector3 object
	 * @param v
	 * @return
	 */
	public Vector3 setMin(Vector3 v) {
		nativeSetMin(mID, v.mID);
		return this;
	}
	private static final native int nativeSetMin(long id_vec1, long id_vec2);

	/**
	 * rotate with specifed angle on specified axis
	 * @param wAxis
	 * @param angle
	 * @return
	 */
	public Vector3 rotate(Vector3 axis, float angle) {
		final Vector3 result = new Vector3();
		nativeRotate(result.mID, mID, axis.mID, angle);
		return result;
	}
	private static final native int nativeRotate(long id_vec_result, long id_vec, long id_vec_axis, float angle);
	

	/**
	 * rotate each axis
	 * @param v
	 * @param ax
	 * @param ay
	 * @param az
	 * @return
	 */
	public static Vector3 rotate(Vector3 v, float ax, float ay, float az) {
		final Vector3 axis = new Vector3(1f, 0f, 0f);
		final Vector3 result = new Vector3();
		nativeRotate(result.mID, v.mID, axis.mID, ax);
		axis.set(0f, 1f, 0f);
		nativeRotate(result.mID, result.mID, axis.mID, ay);
		axis.set(0f, 0f, 1f);
		nativeRotate(v.mID, result.mID, axis.mID, az);
		return v;
	}
	/**
	 * get square length
	 * @return
	 */
	public float length2() {
		return nativeLength2(mID);
	}
	private static final native float nativeLength2(long id_vec);

	/**
	 * get length
	 * @return
	 */
	public float length() {
		return nativeLength(mID);
	}
	private static final native float nativeLength(long id_vec);

	/**
	 * get square distance between this and other Vector3
	 * @param v
	 * @return
	 */
	public float distance2(Vector3 v) {
		return nativeDistance2(mID, v.mID);
	}
	private static final native float nativeDistance2(long id_vec1, long id_vec2);

	/**
	 * get distance between this and other Vector3
	 * @param v
	 * @return
	 */
	public float distance(Vector3 v) {
		return nativeDistance(mID, v.mID);
	}
	private static final native float nativeDistance(long id_vec1, long id_vec2);

	/**
	 * get the linear interpolation between this and another Vector3
	 * @param v other Vector3
	 * @param t The ration of this to v (t = 0 => return this, t=1 => return other)
	 * @return
	 */
	public Vector3 lerp(Vector3 v, float t) {
		final Vector3 result = new Vector3();
		nativeLerp(result.mID, mID, v.mID, t);
		return this;
	}
	private static final native int nativeLerp(long id_vec_result, long id_vec1, long id_vec2, float t);
	
	/**
	 * get the dot product
	 * @param v
	 * @return
	 */
	public float dot(Vector3 v) {
		return nativeDot(mID, v.mID);
	}
	private static final native float nativeDot(long id_vec1, long id_vec2);
	
	
	/**
	 * get the cross product between this and another vector
	 * @param v
	 * @return
	 */
	public Vector3 cross(Vector3 v) {
		final Vector3 result = new Vector3();
		nativeCross(result.mID, mID, v.mID);
		return result;
	}

	/**
	 * get the cross product between this and another vector
	 * @param vx
	 * @param vy
	 * @param vz
	 * @return
	 */
	public Vector3 cross(float vx, float vy, float vz) {
		final Vector3 result = new Vector3();
		final Vector3 v = new Vector3(vx, vy, vz);
		nativeCross(result.mID, mID, v.mID);
		return result;
	}
	private static final native int nativeCross(long id_vec_result, long id_vec1, long id_vec2);

	/**
	 * multiply two vectors to this
	 * @param v1
	 * @param v2
	 * @return
	 */
	public float triple(Vector3 v1, Vector3 v2) {
		return nativeTriple(mID, v1.mID, v2.mID);
	}
	private static final native float nativeTriple(long id_vec, long id_vec1, long id_vec2);
	
	/**
	 * add scaler value to each elemnt of vector算(this += Vector3(dx, dy, dz)) 
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
	public Vector3 add(float dx, float dy, float dz) {
		final Vector3 v = new Vector3(dx, dy, dz);
		nativeAddVec(0, mID, v.mID, 1f);
		return this;
	}

	/**
	 * return added vector as a New Vector3 object (result = this + Vector3(dx, dy, dz))
	 * @param dx
	 * @param dy
	 * @param dz
	 * @return
	 */
	public Vector3 addNew(float dx, float dy, float dz) {
		final Vector3 result = new Vector3(dx, dy, dz);
		nativeAddVec(result.mID, mID, result.mID, 1f);
		return result;
	}


	/**
	 * add other vector to this (this += v)
	 * @param v
	 * @return
	 */
	public Vector3 add(Vector3 v) {
		nativeAddVec(0, mID, v.mID, 1f);
		return this;
	}

	/**
	 * add other vector with scaler scaling to this (this += v * factor)
	 * @param v
	 * @param factor
	 * @return
	 */
	public Vector3 add(Vector3 v, float factor) {
		nativeAddVec(0, mID, v.mID, factor);
		return this;
	}

	/**
	 * return added vector as a new Vector3 object (result = this + v)
	 * @param v
	 * @return
	 */
	public Vector3 addNew(Vector3 v) {
		final Vector3 result = new Vector3();
		nativeAddVec(result.mID, mID, v.mID, 1f);
		return result;
	}

	/**
	 * return added vector as a new Vector3 object (result = this + v * factor)
	 * @param v
	 * @param factor
	 * @return
	 */
	public Vector3 addNew(Vector3 v, float factor) {
		final Vector3 result = new Vector3();
		nativeAddVec(result.mID, mID, v.mID, factor);
		return result;
	}

	private static final native int nativeAddVec(long id_vec_result, long id_vec, long id_vec_add, float factor);

	/**
	 * substract values from each element (this -= Vector3(dx, dy, dz))
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
	public Vector3 sub(float dx, float dy, float dz) {
		final Vector3 v = new Vector3(dx, dy, dz);
		nativeAddVec(0, mID, v.mID, -1f);
		return this;
	}

	/**
	 * substract other vector from this (this -= v)
	 * @param v
	 * @return
	 */
	public Vector3 sub(Vector3 v) {
		nativeAddVec(0, mID, v.mID, -1f);
		return this;
	}

	/**
	 * substract other vector with scaler scaling (this -= v * factor)
	 * @param v
	 * @param factor
	 * @return
	 */
	public Vector3 sub(Vector3 v, float factor) {
		nativeAddVec(0, mID, v.mID, -factor);
		return this;
	}


	/**
	 * return substracted vector as a new Vector3 object (result = this - v)
	 * @param v
	 * @return
	 */
	public Vector3 subNew(Vector3 v) {
		final Vector3 result = new Vector3();
		nativeAddVec(result.mID, mID, v.mID, -1f);
		return result;
	}

	/**
	 * return substracted vector as a new Vector3 object (result = this - v * factor)
	 * @param v
	 * @param factor
	 * @return
	 */
	public Vector3 subNew(Vector3 v, float factor) {
		final Vector3 result = new Vector3();
		nativeAddVec(result.mID, mID, v.mID, -factor);
		return result;
	}
	
	/**
	 * multiply scaler value to each element (this *= v)
	 * @param v
	 * @return
	 */
	public Vector3 mult(float v) {
		nativeMultScalar(0, mID, v);
		return this;
	}
	
	/**
	 * return multiplied vector with scaler value as a new Vector3 object (result = this * v)
	 * @param v
	 * @return
	 */
	public Vector3 multNew(float v) {
		final Vector3 result = new Vector3();
		nativeMultScalar(result.mID, mID, v);
		return result;
	}
	private static final native int nativeMultScalar(long id_vec_result, long id_vec, float v);

	/**
	 * divide each element of this with scaler value (this /= v)
	 * @param v
	 * @return
	 */
	public Vector3 div(float v) {
		nativeDivScalar(0, mID, v);
		return this;
	}
	private static final native int nativeDivScalar(long id_vec_result, long id_vec, float v);

	/**
	 * return divided vector with scaler value as a new Vector3 object (result = this / v)
	 * @param factor
	 * @return
	 */
	public Vector3 divNew(float v) {
		final Vector3 result = new Vector3();
		nativeDivScalar(result.mID, mID, v);
		return result;
	}

	/**
	 * normalize this vector
	 * @return
	 */
	public Vector3 normalize() {
		nativeNormalize(mID);
		return this;
	}
	private static final native int nativeNormalize(long id_vec);

	/**
	 * return normalized vector as a new Vector3 object
	 * @return
	 */
	public Vector3 normalizeNew() {
		final Vector3 result = new Vector3(this);
		nativeNormalize(result.mID);
		return result;
	}

	/**
	 * normalize this vector
	 * @return
	 */
	public Vector3 safeNormalize() {
		nativeSafeNormalize(mID);
		return this;
	}
	private static final native int nativeSafeNormalize(long id_vec);

	/**
	 * return normalized vector as a new Vector3 object
	 * @return
	 */
	public Vector3 safeNormalizeNew() {
		final Vector3 result = new Vector3(this);
		nativeSafeNormalize(result.mID);
		return result;
	}

	/**
	 * calculate angle btween this and other vector
	 * @param v
	 * @return
	 */
	public float angle(Vector3 v) {
		return nativeAngle(mID, v.mID);
	}
	private static final native float nativeAngle(long id_vec1, long id_vec_2);

	/**
	 * convert to this vector to have absoluted elements
	 * @return
	 */
	public Vector3 absolute() {
		nativeAbsolute(0, mID);
		return this;
	}
	private static final native int nativeAbsolute(long id_vec_result, long id_vec);

	/**
	 * return absoluted vector as a new Vector3 object
	 * @return
	 */
	public Vector3 absoluteNew() {
		final Vector3 result = new Vector3();
		nativeAbsolute(result.mID, mID);
		return result;
	}

	/**
	 * return axis number that has minimum value
	 * @return 0:x, 1:y, 2:z
	 */
	public int minAxis() {
		return nativeMinAxis(mID);
	}
	private static final native int nativeMinAxis(long id_vec);

	/**
	 * return minimum axis value
	 * @return
	 */
	public float minAxisValue() {
		return nativeMinAxisValue(mID);
	}
	private static final native float nativeMinAxisValue(long id_vec);

	/**
	 * return axis number that has maximum value
	 * @return 0:x, 1:y, 2:z
	 */
	public int maxAxis() {
		return nativeMaxAxis(mID);
	}
	private static final native int nativeMaxAxis(long id_vec);

	/**
	 * return maximum axis value
	 * @return
	 */
	public float maxAxisValue() {
		return nativeMaxAxisValue(mID);
	}
	private static final native float nativeMaxAxisValue(long id_vec);

	/**
	 * return furthest axis number
	 * @return 0:x, 1:y, 2:z
	 */
	public int furthestAxis() {
		return nativeFurthestAxis(mID);
	}
	private static final native int nativeFurthestAxis(long id_vec);
	
	/**
	 * return furthest axis value
	 * @return
	 */
	public float furthestAxisValue() {
		return nativeFurthestAxisValue(mID);
	}
	private static final native float nativeFurthestAxisValue(long id_vec);

	/**
	 * return closest axis number
	 * @return 0:x, 1:y, 2:z
	 */
	public int closestAxis() {
		return nativeClosesttAxis(mID);
	}
	private static final native int nativeClosesttAxis(long id_vec);

	/**
	 * return closest axis value
	 * @return
	 */
	public float closestAxisValue() {
		return nativeClosesttAxisValue(mID);
	}
	private static final native float nativeClosesttAxisValue(long id_vec);

	/**
	 * return multiplied vector with Matrix3x3 as a new Vector3 object
	 * @param mat
	 * @return
	 */
	public Vector3 mult(Matrix3x3 mat) {
		final Vector3 result = new Vector3();
		nativeMultVecMat(result.mID, mID, mat.getID());
		return result;
	}
	private static final native int nativeMultVecMat(long id_vec_result, long id_vec, long id_mat);

	public static void PlaneSpace1(Vector3 n, Vector3 p, Vector3 q) {
		nativePlaneSpace1(n.mID, p.mID, q.mID);
	}
	private static final native int nativePlaneSpace1(long id_vec_n, long id_vec_p, long id_vec_q);

	/**
	 * this.dot(v0) → this.dot(v1) → this.dot(v2)
	 * @param v0
	 * @param v1
	 * @param v2
	 * @return
	 */
	public Vector3 dot3(Vector3 v0, Vector3 v1, Vector3 v2) {
		nativeDot3(mID, mID, v0.mID, v1.mID, v2.mID);
		return this;
	}
	
	private static final native int nativeDot3(long id_vec_result, long id_vec, long id_vec0, long id_vec1, long id_vec2);
}
