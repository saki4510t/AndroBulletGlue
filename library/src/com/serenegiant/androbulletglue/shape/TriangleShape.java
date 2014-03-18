/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: TriangleShape.java
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
import com.serenegiant.androbulletglue.interfaces.PolyhedralConvexShape;

public class TriangleShape extends PolyhedralConvexShape {

	public final Vector3 p0 = new Vector3();
	public final Vector3 p1 = new Vector3();
	public final Vector3 p2 = new Vector3();
	
	protected TriangleShape(PhysicsWorld world,
		Vector3 p0, Vector3 p1, Vector3 p2) {
		
		super(world, TRIANGLE_SHAPE_PROXYTYPE);
		this.p0.set(p0);
		this.p1.set(p1);
		this.p2.set(p2);
		
		createCollisionShape();
	}

}
