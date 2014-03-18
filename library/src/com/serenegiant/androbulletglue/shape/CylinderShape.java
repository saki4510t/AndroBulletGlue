/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: CylinderShape.java
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
import com.serenegiant.androbulletglue.interfaces.ConvexInternalShape;

public class CylinderShape extends ConvexInternalShape {
	
	public final Vector3 halfExtents;
	public final int upAxis;
	
	/**
	 * constructor
	 * @param halfExtents_x half size of cylinder
	 * @param halfExtents_y half size of cylinder
	 * @param halfExtents_z half size of cylinder
	 * @param upAxis axis (UPAXIS_X/UPAXIS_Y/UPAXIS_Z)
	 */
	public CylinderShape(PhysicsWorld world, float halfExtent_x, float halfExtent_y, float halfExtent_z, int upAxis) {
		this(world, new Vector3(halfExtent_x, halfExtent_y, halfExtent_z), upAxis);
	}

	/**
	 * constructor
	 * @param halfExtents half size of cylinder
	 * @param upAxis axis (UPAXIS_X/UPAXIS_Y/UPAXIS_Z)
	 */
	public CylinderShape(PhysicsWorld world, Vector3 halfExtents, int upAxis) {
		super(world, CYLINDER_SHAPE_PROXYTYPE);
		this.halfExtents = halfExtents;
		this.upAxis = upAxis;
		createCollisionShape();
	}

}
