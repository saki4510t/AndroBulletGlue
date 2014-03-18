/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Solver.java
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

package com.serenegiant.androbulletglue.interfaces;

public abstract class Solver extends TypedObject {
	public static final int	SEQUENTIAL_IMPULSE_CONSTRAINT_SOLVER = 1;
	public static final int	VORONOI_SIMPLEX_SOLVER = 2;
	public static final int	MINKOWSKI_PENETRATION_DEPTH_SOLVER = 3;
	public static final int	GJK_EPA_PENETRATION_DEPTH_SOLVER = 4;
	public static final int	CONVEX_PENETRATION_DEPTH_SOLVER = 5;
	public static final int SOFT_BODY_DEFAULT_SOLVER = 6;	// this is only available for SOFT_BODY
//	public static final int SOFT_BODY_CPU_SOLVER = 7;
//	public static final int SOFT_BODY_CL_SOLVER = 8;
//	public static final int SOFT_BODY_CL_SIMD_SOLVER = 9;
//	public static final int SOFT_BODY_DX_SOLVER = 10;
//	public static final int SOFT_BODY_DX_SIMD_SOLVER = 11;

	
	protected Solver(int type) {
		super(type);
	}
	
}
