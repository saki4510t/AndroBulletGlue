/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Matrix3x3.java
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

public class Matrix3x3 extends NativeObject {
	
	/**
	 * internal constructor to asign native btMatrix3x3 object
	 * @param native_id
	 */
	protected Matrix3x3(long id_mat) {
		mID = id_mat;
		isAsigned = true;
	}
	/**
	 * Constructor without initialization(set all values to zero)
	 */
	public Matrix3x3() {
		mID = nativeCreate(0);
	}

	/**
	 * copy constructor(deep copy)
	 * @param src
	 */
	public Matrix3x3(Matrix3x3 src) {
		mID = nativeCreate(src != null ? src.mID : 0);
	}
	private static final native long nativeCreate(long id_src_mat);
	
	/**
	 * Constructor with each element value
	 * @param xx
	 * @param xy
	 * @param xz
	 * @param yx
	 * @param yy
	 * @param yz
	 * @param zx
	 * @param zy
	 * @param zz
	 */
	public Matrix3x3(float xx, float xy, float xz,
					 float yx, float yy, float yz,
					 float zx, float zy, float zz) {
		mID = nativeCreate(0);
		set(xx, xy, xz, yx, yy, yz, zx, zy, zz);
	}


	/**
	 * Constractor with matrix
	 * @param mat
	 */
	public Matrix3x3(float[] mat) {
		mID = nativeCreate(0);
		set(mat);
	}
	
	/**
	 * Constractor with rotating quaternion
	 * @param quat
	 */
	public Matrix3x3(Quaternion quat) {
		mID = nativeCreate(0);
		setRotation(quat);
	}
	
	protected final native void nativeDestroy(long id_map);
	
	/**
	 * asign other Matrix3x3 object to this
	 * @param other
	 * @return
	 */
	public Matrix3x3 asign(Matrix3x3 other) {
		dispose();
		mID = other.mID;
		isAsigned = true;
		return this;
	}
	
	/**
	 * setter with each element value
	 * @param xx
	 * @param xy
	 * @param xz
	 * @param yx
	 * @param yy
	 * @param yz
	 * @param zx
	 * @param zy
	 * @param zz
	 * @return
	 */
	public Matrix3x3 set(float xx, float xy, float xz,
						 float yx, float yy, float yz,
						 float zx, float zy, float zz) {
		nativeSet(mID, xx, xy, xz, yx, yy, yz, zx, zy, zz);
		return this;
	}
	private static final native int nativeSet(long native_mat,
		float xx, float xy, float xz,
		float yx, float yy, float yz,
		float zx, float zy, float zz);
	
	/**
	 * Setter with other Matrix3x3 object(deep copy)
	 * @param src
	 * @return
	 */
	public Matrix3x3 set(Matrix3x3 src) {
		if (src != null) {
			nativeCopy(mID, src.mID);
		}
		return this;
	}
	private static final native int nativeCopy(long id_dest_mat, long id_src_mat);
	
	/**
	 * Setter with matrix(float array)
	 * @param mat
	 * @return
	 */
	public Matrix3x3 set(float[] mat) {
		if ((mat != null) && (mat.length >= 9)) {
			nativeSet(mID,
				mat[0], mat[1], mat[2],
				mat[3], mat[4], mat[5],
				mat[6], mat[7], mat[8]);
		}
		return this;
	}

	/**
	 * get this Matrix3x3 object as Quaternion object
	 * @param quat
	 * @return quat
	 */
	public Quaternion getRotation(Quaternion quat) {
		if (quat != null)
			nativeGetRotation(mID, quat.getID());
		return quat;
	}
	private static final native int nativeGetRotation(long id_mat, long id_quat);

	/**
	 * set this Matrix3x3 object from Quaternion object
	 * @param quat
	 * @return
	 */
	public Quaternion setRotation(Quaternion quat) {
		if (quat != null)
			nativeSetRotation(mID, quat.getID());
		return quat;
	}
	private static final native int nativeSetRotation(long id_mat, long id_quat);

	
	/**
	 * get this Matrix3x3 object as Vector3 object
	 * @param euler
	 * @param euler x shows yaw/around X axis, y shows pitch/around Y axis, z shows roll/around Z axis
	 */
	public Vector3 getEulerZYX(Vector3 euler) {
		if (euler != null)
			nativeGetEulerZYX(mID, euler.getID());
		return euler;
	}
	private static final native int nativeGetEulerZYX(long id_mat, long id_vec);

	/**
	 * set this Matrix3x3 object from Vector3 object
	 * @param euler x is yaw/around X axis, y is pitch/around Y axis, z is roll/around Z axis
	 */
	public void setEulerZYX(Vector3 euler) {
		nativeSetEulerZYX(mID, euler.x(), euler.y(), euler.z());
	}
	/**
	 * 
	* @param yaw Yaw around X axis
	* @param pitch Pitch around Y axis
	* @param roll around X axis 
	 * @return
	 */
	public Matrix3x3 setEulerZYX(float yaw, float pitch, float roll) {
		nativeSetEulerZYX(mID, yaw, pitch, roll);
		return this;
	}
	private static final native int nativeSetEulerZYX(long id_mat, float yaw, float pitch, float roll);
	private static final native int nativeSetEulerZYXVec(long id_mat, long id_vec);

	/**
	 * set zoom in/out or revese
	 * @param scaleX
	 * @param scaleY
	 * @param scaleZ
	 * @return
	 */
	public Matrix3x3 setScale(float scaleX, float scaleY, float scaleZ) {
		set(scaleX, 0f, 0f,
			0f, scaleY, 0f,
			0f, 0f,scaleZ);
		return this;
	}
	/**
	 * set zoom in/out or revese
	 * @param scale
	 * @return
	 */
	public Matrix3x3 setScale(Vector3 scale) {
		return setScale(scale.x(), scale.y(), scale.z());
	}
	
	/**
	 * multiply Matrix3x3 with other Matrix3x3 object 
	 * @param mat
	 * @return
	 */
	public Matrix3x3 mult(Matrix3x3 mat) {
		nativeMultMat(0, mID, mat.mID);
		return this;
	}
	private static final native int nativeMultMat(long id_mat_result, long id_mat1, long id_mat2);

	/**
	 * return multiplied matrix as a new Matrix3x3 object
	 * @param mat
	 * @return
	 */
	public Matrix3x3 multNew(Matrix3x3 mat) {
		final Matrix3x3 result = new Matrix3x3();
		nativeMultMat(result.mID, mID, mat.mID);
		return result;
	}

	/**
	 * return multiplied vector as a new Vector3 object
	 * @param vec
	 * @return
	 */
	public Vector3 mult(Vector3 vec) {
		final Vector3 result = new Vector3();
		nativeMultMatVec(result.getID(), mID, vec.getID());
		return result;
	}
	private static final native int nativeMultMatVec(long id_vec_result, long id_mat, long id_vec);
	
	/**
	 * scale this Matrix3x3 with Vector3
	 * @param vec
	 * @return
	 */
	public Matrix3x3 scaled(Vector3 vec) {
		nativeScaledMat(0, mID, vec.getID());
		return this;
	}
	/**
	 * return scaled matrix as a new Matrix3x3 object
	 * @param vec
	 * @return
	 */
	public Matrix3x3 scaledNew(Vector3 vec) {
		final Matrix3x3 result = new Matrix3x3();
		nativeScaledMat(result.mID, mID, vec.getID());
		return result;
	}
	private static final native int nativeScaledMat(long id_mat_result, long id_mat, long id_vec);
	
	public Matrix3x3 setIdentity() {
		return set(1f, 0f, 0f,
				   0f, 1f, 0f,
				   0f, 0f, 1f); 
	}

	/**
	 * Convert this Matrix3x3 object to transposed matrix
	 * @return
	 */
	public Matrix3x3 transpose() {
		nativeTranspose(mID, mID);
		return this;
	}
	/**
	 * return transposed matrix as a new Matrix3x3 object
	 * @return
	 */
	public Matrix3x3 transposeNew() {
		final Matrix3x3 result = new Matrix3x3();
		nativeTranspose(result.mID, mID);
		return result;
	}
	private static final native int nativeTranspose(long id_mat_result, long id_mat);

	/**
	 * Convert this Matrix3x3 object to inversed matrix
	 * @return
	 */
	public Matrix3x3 inverse() {
		nativeInverse(mID, mID);
		return this;
	}
	/**
	 * return inversed matrix as a new Matrix3x3 object
	 * @return
	 */
	public Matrix3x3 inverseNew() {
		final Matrix3x3 result = new Matrix3x3();
		nativeInverse(result.mID, mID);
		return result;
	}
	private static final native int nativeInverse(long id_mat_result, long id_mat);

	/**
	 * Calculate absolute value of each element
	 * @return
	 */
	public Matrix3x3 absolute() {
		nativeAbsolute(mID, mID);
		return this;
	}
	/**
	 * Calculate absolute value of each element　and return new object
	 * @return
	 */
	public Matrix3x3 absoluteNew() {
		final Matrix3x3 result = new Matrix3x3();
		nativeAbsolute(result.mID, mID);
		return result;
	}
	private static final native int nativeAbsolute(long id_mat_result, long id_mat);

	/**
	 * Get OpenGL sub matrix into float array
	 * @param result
	 */
	public void getOpenGLSubMatrix(float[] result) {
		if (result == null)
			throw new NullPointerException();
		if (result.length <= 12)
			throw new ArrayIndexOutOfBoundsException();
		nativeGetOpenGLSubMatrix(mID, result);
	}
	private static final native int nativeGetOpenGLSubMatrix(long id_mat, float[] result);

	/**
	 * set OpenGL sub matrix from float array values to this Matrix3x3 object 
	 * @param glmat
	 * @return
	 */
	public Matrix3x3 setFromOpenGLSubMatrix(float[] glmat) {
		if (glmat == null)
			throw new NullPointerException();
		if (glmat.length <= 12)
			throw new ArrayIndexOutOfBoundsException();
		nativeSetFromOpenGLSubMatrix(mID, glmat);
		return this;
	}
	private static final native int nativeSetFromOpenGLSubMatrix(long id_mat, float[] glmat);
	
	/**
	 * get specified row of Matrix3x3 object as a Vecotr3
	 * @param i
	 * @return
	 */
	public Vector3 getRow(int row) {
		final Vector3 result = new Vector3();
		nativeGetRow(result.getID(), mID, row);
		return result;
	}
	private static final native int nativeGetRow(long id_vec_result, long id_mat, int row);
	
	/**
	 * get specified column of Matrix3x3 object as a Vecotr3
	 * @param i
	 * @return
	 */
	public Vector3 getColumn(int col) {
		final Vector3 result = new Vector3();
		nativeGetColumn(result.getID(), mID, col);
		return result;
	}
	private static final native int nativeGetColumn(long id_vec_result, long id_mat, int col);

}
