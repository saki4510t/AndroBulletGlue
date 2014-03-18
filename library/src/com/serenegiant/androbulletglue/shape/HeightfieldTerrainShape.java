/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: HeightfieldTerrainShape.java
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
import com.serenegiant.androbulletglue.interfaces.ConcaveShape;

public class HeightfieldTerrainShape extends ConcaveShape {
	protected final int mHeightStickWidth;
	protected final int mHeightStickLength;
	protected final byte[] mHeightfieldData;
	protected final float mMaxHeight;
	protected final int mUpAxis;
	protected final boolean mUseFloatData;
	protected final boolean mFlipQuadEdges;
	protected final boolean mUseDiamondSubdivision;
	// native code access this field and you should not remove and rename
	protected long mNativePtr;

	/**
	 * constructor</br>
	 * you can create this object but its calculating and drawing are too heavy to use. (;_;)
	 * @param heightStickWidth
	 * @param heightStickLength
	 * @param heightfieldData
	 * @param maxHeight
	 * @param upAxis
	 * @param useFloatData
	 * @param flipQuadEdges
	 * @param useDiamondSubdivision
	 */
	public HeightfieldTerrainShape(PhysicsWorld world, 
		int heightStickWidth, int heightStickLength, byte[] heightfieldData,
		float maxHeight, int upAxis,
		boolean useFloatData, boolean flipQuadEdges, boolean useDiamondSubdivision) {
		
		super(world, TERRAIN_SHAPE_PROXYTYPE);
		
		mHeightStickWidth = heightStickWidth;
		mHeightStickLength = heightStickLength;
		mHeightfieldData = heightfieldData;
		mMaxHeight = maxHeight;
		mUpAxis = upAxis;
		mUseFloatData = useFloatData;
		mFlipQuadEdges = flipQuadEdges;
		mUseDiamondSubdivision = useDiamondSubdivision;

		createCollisionShape();
	}

}
