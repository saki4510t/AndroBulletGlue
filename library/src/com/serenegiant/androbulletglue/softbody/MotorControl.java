/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MotorControl.java
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

package com.serenegiant.androbulletglue.softbody;

import com.serenegiant.androbulletglue.interfaces.NativeObject;

public class MotorControl extends NativeObject {

	public MotorControl(float goal, float maxTorque) {
		mID = nativeCreate(goal, maxTorque);
	}
	// nativeCreate in this class can not be a static method
	// because native code access fields in this object instance.
	private final native long nativeCreate(float goal, float maxTorque);
	
	public MotorControl set(float goal, float maxTorque) {
		nativeSet(mID, goal, maxTorque);
		return this;
	}
	private static final native int nativeSet(long id_motor, float goal, float maxTorque);
	
	@Override
	protected final native void nativeDestroy(long id_motor);
	
}
