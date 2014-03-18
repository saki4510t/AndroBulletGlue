/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MotionState.java
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

import com.serenegiant.androbulletglue.math.Matrix3x3;
import com.serenegiant.androbulletglue.math.Quaternion;
import com.serenegiant.androbulletglue.math.Transform;
import com.serenegiant.androbulletglue.math.Vector3;

public class MotionState {
	
	/**
	 * gravity center (to reduce checking on native side, worldTransform should not null)
	 */
	public final Transform worldTransform;
	/**
	 * offset value between gravity center and geometrical center (optional)
	 */
	public Transform centerOfMassOffset;
	/**
	 * graphical conversion value (optional)
	 */
	public Transform graphicsWorldTransform;

	/**
	 * constructor
	 */
	public MotionState() {
		worldTransform = new Transform(0f, 0f, 0f);
	}

	/**
	 * constructor
	 * @param originX gravity center coordinate x
	 * @param originY gravity center coordinate y
	 * @param originZ gravity center coordinate z
	 */
	public MotionState(float originX, float originY, float originZ) {
		worldTransform = new Transform(originX, originY, originZ);
	}
	/**
	 * constructor
	 * @param origin
	 */
	public MotionState(Vector3 origin) {
		worldTransform = new Transform(origin);
	}
	
	/**
	 * constructor
	 * @param origin
	 * @param basis
	 */
	public MotionState(Vector3 origin, Matrix3x3 basis) {
		worldTransform = new Transform(origin, basis);
	}
	
	/**
	 * constructor
	 * @param transform
	 */
	public MotionState(Transform transform) {
		worldTransform = new Transform(transform);
	}
	/**
	 * constructor
	 * @param origin
	 * @param rotation
	 */
	public MotionState(Vector3 origin, Quaternion rotation) {
		worldTransform = new Transform(origin, rotation);
	}

}
