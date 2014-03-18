/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MultimaterialTriangleMeshShape.java
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
import com.serenegiant.androbulletglue.mesh.TriangleIndexVertexArray;
import com.serenegiant.androbulletglue.mesh.TriangleMaterial;

public class MultimaterialTriangleMeshShape extends BvhTriangleMeshShape {

	public MultimaterialTriangleMeshShape(PhysicsWorld world, 
		TriangleIndexVertexArray triangleIndexVertexArray,
		boolean useQuantizedAabbCompression) {
		
		super(world, MULTIMATERIAL_TRIANGLE_MESH_PROXYTYPE,
			triangleIndexVertexArray, useQuantizedAabbCompression);
	}
	
	/**
	 * only get material information, because you can not apply its modification now.
	 * @param partID
	 * @param triIndex
	 * @return
	 */
	public TriangleMaterial getMaterialProperties(int partID, int triIndex) {
		final TriangleMaterial result = new TriangleMaterial();
		nativeGetMaterialProperties(mID, result, partID, triIndex);
		return result;
	}
	private static final native int nativeGetMaterialProperties(long id_shape, TriangleMaterial material, int partID, int triIndex);

}
