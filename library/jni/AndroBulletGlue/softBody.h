/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: softbody.h
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

#ifndef SOFTBODY_H_
#define SOFTBODY_H_

#include "androbulletglue.h"

enum SOFTBODY_TYPE {
	SOFTBODY_ROPE = 0,			// 0
	SOFTBODY_PATCH,				// 1
	SOFTBODY_PATCHUV,			// 2
	SOFTBODY_ELLIPSOID,			// 3
	SOFTBODY_FROM_TRIMESH,		// 4
	SOFTBODY_FROM_CONVEXHULL,	// 5
	SOFTBODY_FROM_TETGENDATA	// 6
};

#endif /* SOFTBODY_H_ */
