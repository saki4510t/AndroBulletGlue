/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: SoftBodyMaterial.java
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

package com.serenegiant.androbulletglue.softbody;

import com.serenegiant.androbulletglue.interfaces.NativeObject;

public class SoftBodyMaterial extends NativeObject {
	// native code access these fields and you should not remove and rename these.
	public float m_kLST;			// Linear stiffness coefficient [0,1]
	public float m_kAST;			// Area/Angular stiffness coefficient [0,1]
	public float m_kVST;			// Volume stiffness coefficient [0,1]
	public int	 m_flags;			// Flags
	
	public SoftBodyMaterial(int id_native) {
		if (id_native != 0) {
			mID = id_native;
			isAsigned = true;
		}
	}
	
	public SoftBodyMaterial() {
		isAsigned = true;
	}

	protected void nativeDestroy(long id_material) { }
	
	public void setMaterial() {
		nativeSetMaterial(mID);
	}
	private static final native void nativeSetMaterial(long id_material);
}
