/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: GLDebugDrawer.cpp
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

#include "GLDebugDrawer.h"
#include "GLDebugFont.h"
#include "utils.h"
#include "mygl.h"

GLDebugDrawer::GLDebugDrawer()
  :m_debugMode(0) {

}

GLDebugDrawer::~GLDebugDrawer()
{
}

void GLDebugDrawer::drawLine(
	const btVector3 &from, const btVector3 &to,
	const btVector3 &fromColor, const btVector3 &toColor) {

	Mygl gl(2, 0, true, false, false);
	gl.glBegin(GL_LINES);
		gl.glColor(fromColor);	gl.glVertex(from);
		gl.glColor(toColor);	gl.glVertex(to);
	gl.glEnd();
}

void GLDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {

	drawLine(from, to, color, color);
}

void GLDebugDrawer::drawSphere (const btVector3 &p, btScalar radius, const btVector3 &color) {

	glColor4f(color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
	glPushMatrix();
	glTranslatef(p.getX(), p.getY(), p.getZ());

	int lats = 5;
	int longs = 5;

	int i, j;
	for (i = 0; i <= lats; i++) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
		btScalar z0  = radius * sin(lat0);
		btScalar zr0 =  radius * cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
		btScalar z1 = radius * sin(lat1);
		btScalar zr1 = radius * cos(lat1);

		Mygl gl(longs * 2, 0, false, false, true);
		gl.glBegin(GL_TRIANGLE_STRIP);	// GL_QUAD_STRIP
		for (j = 0; j <= longs; j++) {
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);

			gl.glNormal(x * zr0, y * zr0, z0);	gl.glVertex(x * zr0, y * zr0, z0);
			gl.glNormal(x * zr1, y * zr1, z1);	gl.glVertex(x * zr1, y * zr1, z1);
		}
		gl.glEnd();
	}

	glPopMatrix();
}



void GLDebugDrawer::drawTriangle(const btVector3 &a, const btVector3 &b,
		const btVector3 &c,const btVector3 &color, btScalar alpha)
{
	const btVector3	n = btCross(b - a, c - a).normalized();
	Mygl gl(3, 0, true, false, true);
	gl.glBegin(GL_TRIANGLES);
	gl.glColor(color, alpha);
	gl.glNormal(n);
	gl.glVertex(a, b, c);
	gl.glEnd();
}

void GLDebugDrawer::setDebugMode(int debugMode) {
	m_debugMode = debugMode;
}

void GLDebugDrawer::draw3dText(const btVector3 &location,const char *textString) {
//	glRasterPos3f(location.x(),  location.y(),  location.z());	// TODO OpenGL ESには存在しない
//	BMF_DrawString(BMF_GetFont(BMF_kHelvetica10), textString);
}

void GLDebugDrawer::reportErrorWarning(const char *warningString)
{
	printf("%s\n",warningString);
}

void GLDebugDrawer::drawContactPoint(
	const btVector3 &pointOnB, const btVector3 &normalOnB,
	btScalar distance, int lifeTime, const btVector3 &color) {
	
	btVector3 to = pointOnB + normalOnB * 1;//distance;
	const btVector3 &from = pointOnB;
	glColor4f(color.getX(), color.getY(), color.getZ(),1.f);
	//glColor4f(0,0,0,1.f);
	Mygl gl(2, 0, false, false, false);
	gl.glBegin(GL_LINES);
	gl.glVertex(from, to);
	gl.glEnd();

//	glRasterPos3f(from.x(),  from.y(),  from.z());
//	char buf[12];
//	sprintf(buf," %d",lifeTime);
//	BMF_DrawString(BMF_GetFont(BMF_kHelvetica10), buf);
}





