/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: CompoundShape.java
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
import com.serenegiant.androbulletglue.math.Transform;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.interfaces.CollisionShape;

public class CompoundShape extends CollisionShape {

	protected Vector3 mLocalScaling;

	public CompoundShape(PhysicsWorld world) {
		super(world, COMPOUND_SHAPE_PROXYTYPE);
		createCollisionShape();
	}
	
	/**
	 * add child shape to this CompoundShape
	 * @param localTransform
	 * @param childShape
	 */
	public void addChildShape(Transform localTransform, CollisionShape childShape) {
		nativeAddChildShape(mID, localTransform.getID(), childShape.getID());
	}
	private static final native int nativeAddChildShape(long id_shape, long id_trans, long id_childShape);
	
	/**
	 * remove a child shape from this CompoundShape
	 * @param childShape
	 */
	public void removeChildShape(CollisionShape childShape) {
		nativeRemoveChildShape(mID, childShape.getID());
	}
	private static final native int nativeRemoveChildShape(long id_shape, long id_shape_child);
	
	public int getNumChildShapes() {
		return nativeGetNumChildShapes(mID);
	}
	private static final native int nativeGetNumChildShapes(long id_shape);
	
}
