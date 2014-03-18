/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Broadphase.java
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

public abstract class Broadphase extends TypedObject {

 	public static final int BROADPHASE_SIMPLE = 0;
	public static final int BROADPHASE_AXISSWEEP3 = 1;
	public static final int BROADPHASE_32AXISSWEEP3 = 2;
	public static final int BROADPHASE_DBVT = 3;
 	public static final int BROADPHASE_MULTISAP = 4;
 	public static final int BROADPHASE_CUDA = 5;
 	
	protected Broadphase(int type) {
		super(type);
	}

}
