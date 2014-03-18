/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: GLDebugFont.h
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


#ifndef BT_DEBUG_FONT_H
#define BT_DEBUG_FONT_H

#ifdef _WIN32//for glut.h
#include <windows.h>
#endif

//think different
#if defined(__APPLE__) && !defined (VMDMESA)
	#include <TargetConditionals.h>
	#if (defined (TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined (TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
		#import <OpenGLES/ES1/gl.h>
		#define glOrtho glOrthof
	#else
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
		#include <GLUT/glut.h>
	#endif
#else
	#if defined(_WINDOWS)
		#include <windows.h>
		#include <GL/gl.h>
		#include <GL/glu.h>
	#elif defined(ANDROID_NDK)
		#include <GLES/gl.h>
	#else
		#include <GLES/egl.h>
		#include <GL/gl.h>
		#include <GL/glut.h>
	#endif
#endif

#include "LinearMath/btVector3.h"

/// OpenGL debug text rendering
class GLDebugFont
{
private:
	bool m_texturesInitialized;
	GLuint m_texture;
	int m_screenWidth;
	int m_screenHeight;

protected:
	inline void internal_clear(void) {
		m_texturesInitialized = false;
		m_texture = 0;
		m_screenWidth = m_screenHeight = -1;
	}
	void GLDebugDrawStringInternal(int x,int y,const char* string,const btVector3& rgb, bool enableBlend, int spacing);
	void GLDebugDrawStringInternal(int x,int y,const char* string,const btVector3& rgb);
public:
	GLDebugFont(int screenWidth, int screenHeight);
	virtual ~GLDebugFont(void);
	void releaseFont(void);
	void resetFont(int screenWidth, int screenHeight);
	void GLDebugDrawString(int x,int y,const char* string);
};


#endif //BT_DEBUG_FONT_H

