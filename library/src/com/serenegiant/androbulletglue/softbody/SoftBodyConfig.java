/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: SoftBodyConfig.java
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

public class SoftBodyConfig {
	// native code access these fields and you should not remove and rename.
//	eAeroModel::_aeromodel;		// Aerodynamic model (default: V_Point)
	public float kVCF;			// Velocities correction factor (Baumgarte)
	public float kDP;			// Damping coefficient [0,1]
	public float kDG;			// Drag coefficient [0,+inf]
	public float kLF;			// Lift coefficient [0,+inf]
	public float kPR;			// Pressure coefficient [-inf,+inf]
	public float kVC;			// Volume conversation coefficient [0,+inf]
	public float kDF;			// Dynamic friction coefficient [0,1]
	public float kMT;			// Pose matching coefficient [0,1]		
	public float kCHR;			// Rigid contacts hardness [0,1]
	public float kKHR;			// Kinetic contacts hardness [0,1]
	public float kSHR;			// Soft contacts hardness [0,1]
	public float kAHR;			// Anchors hardness [0,1]
	public float kSRHR_CL;		// Soft vs rigid hardness [0,1] (cluster only)
	public float kSKHR_CL;		// Soft vs kinetic hardness [0,1] (cluster only)
	public float kSSHR_CL;		// Soft vs soft hardness [0,1] (cluster only)
	public float kSR_SPLT_CL;	// Soft vs rigid impulse split [0,1] (cluster only)
	public float kSK_SPLT_CL;	// Soft vs rigid impulse split [0,1] (cluster only)
	public float kSS_SPLT_CL;	// Soft vs rigid impulse split [0,1] (cluster only)
	public float maxvolume;		// Maximum volume ratio for pose
	public float timescale;		// Time scale
	public int   viterations;	// Velocities solver iterations
	public int   piterations;	// Positions solver iterations
	public int   diterations;	// Drift solver iterations
	public int   citerations;	// Cluster solver iterations
	public int   collisions;	// Collisions flags
	public float[] m_vsequence;	// Velocity solvers sequence(not supported now)
	public float[] m_psequence;	// Position solvers sequence(not supported now)
	public float[] m_dsequence;	// Drift solvers sequence(not supported now)
}
