/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: CylinderShapeX.java
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

package com.serenegiant.androbulletglue.shape;

import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.math.Vector3;

public class CylinderShapeX extends CylinderShape {
	/**
	 * constructor(x axis)</br>
	 * the radius of this cylinder is smaller value of halfExtents#y and halfExtents#z
	 * @param halfExtents size of cylinder
	 */
	public CylinderShapeX(PhysicsWorld world, Vector3 halfExtents) {
		super(world, halfExtents, UPAXIS_X);
	}

	/**
	 * constructor(x axis)</br>
	 * the radius of this cylinder is smaller value of halfExtents_y and halfExtents_z
	 * @param halfExtents_x half size of cylinder
	 * @param halfExtents_y half size of cylinder
	 * @param halfExtents_z half size of cylinder
	 */
	public CylinderShapeX(PhysicsWorld world, float halfExtent_x, float halfExtent_y, float halfExtent_z) {
		super(world, halfExtent_x, halfExtent_y, halfExtent_z, UPAXIS_X);
	}
}
