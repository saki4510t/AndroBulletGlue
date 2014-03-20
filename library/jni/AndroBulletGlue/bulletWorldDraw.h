/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: bulletWorldDraw.h
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

#ifndef BULLETWORLDDRAW_H_
#define BULLETWORLDDRAW_H_

#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include "GLShapeDrawer.h"
#include "GLDebugFont.h"
#include "localdefines.h"
#include "utils.h"

class	btCollisionShape;
class	btDynamicsWorld;
class	btRigidBody;
class	btTypedConstraint;

#define	CAMERAMODE_FORCE_ZAXIS_UP 1		// whether or not to kepp z axis up(bit0)
#define	CAMERAMODE_TRACKING 2			// whether or not to track object(bit1)

#define PICKMODE_HANG		1			// hang and move a object(bit0)
#define PICKMODE_TRACKING	2			// whether or not to track pickking object(bit1)

class BulletWorldDraw {
protected:
	void displayProfileString(int xOffset, int yStart, char *message);
	class CProfileIterator *m_profileIterator;

	protected:
#ifdef USE_BT_CLOCK
	btClock m_clock;
#endif //USE_BT_CLOCK

	// this is the most important class
	btDynamicsWorld *m_dynamicsWorld;
	btSoftBodyWorldInfo *m_softBodyWorldInfo;

	// constraint for picking
	btTypedConstraint *m_pickConstraint;
	virtual void removePickingConstraint();

	btCollisionShape *m_shootBoxShape;

	float m_cameraDistance;				// distance btween camera and gazing point
	float m_minCameraDistance;			// minimum distance btween camera and gazing point
	float m_maxCameraDistance;			// maximum distance btween camera and gazing point
	float m_cameraHeight;				// height of camera position
	int m_cameraMode;					// camera mode
	btRigidBody *m_trackingBody;		// the object that the camera track now

	int	m_debugMode;
	int m_pickMode;						// bit0: HANG mode, bit1: Tracking mode
	btRigidBody *m_pickedBody;			// for deactivation state

	float m_ele;
	float m_azi;
	btVector3 m_cameraPosition;
	btVector3 m_cameraTargetPosition;	// look at

	int	m_mouseOldX;
	int	m_mouseOldY;
	int	m_mouseButtons;					// bit0: left button, bit1: middle button, bit2:right button

	bool m_autocam;
	bool m_raycast;
	btScalar m_animtime;

public:
	int	m_modifierKeys;
protected:

	float m_scaleBottom;
	float m_scaleFactor;
	btVector3 m_cameraUp;
	int	m_forwardAxis;
	float m_zoomStepSize;

	int m_glutScreenWidth;
	int m_glutScreenHeight;

	float m_frustumZNear;
	float m_frustumZFar;

	int	m_ortho;

	float m_ShootBoxInitialSpeed;

	bool m_stepping;
	bool m_singleStep;
	bool m_idle;
	int m_lastKey;

	void showProfileInfo(int& xOffset,int& yStart, int yIncr);
	void renderscene(int pass);
	void renderSoftbody(void);

	GLDebugFont		*m_debugFont;
	GLShapeDrawer	*m_shapeDrawer;
	bool			m_drawShadows;
	bool			m_drawClusters;
	btVector3		m_sundirection;
	btScalar		m_defaultContactProcessingThreshold;

public:
	BulletWorldDraw(btDynamicsWorld *dynamicsWorld, btSoftBodyWorldInfo *softBodyWorldInfo);

	virtual ~BulletWorldDraw(void);

	void resetFrame(void);

	btDynamicsWorld *getDynamicsWorld(void)
	{
		return m_dynamicsWorld;
	}

	void overrideGLShapeDrawer (GLShapeDrawer *shapeDrawer);
	GLShapeDrawer *getGLShapeDrawer(void) { return m_shapeDrawer; }

	void setOrthographicProjection(void);
	void resetPerspectiveProjection(void);

	bool setTexturing(bool enable) { return(m_shapeDrawer->enableTexture(enable)); }
	bool setDrawShadows(bool drawShadows)	{ bool p = m_drawShadows; m_drawShadows = drawShadows; return p; }
	bool setDrawClusters(bool drawClusters)	{
		bool  p = m_drawClusters;
		m_drawClusters = drawClusters;
		if (m_softBodyWorldInfo) {
			btSoftRigidDynamicsWorld *softWorld = (btSoftRigidDynamicsWorld *)m_dynamicsWorld;
			LOGD("setDrawClusters:%d", drawClusters);
			if (drawClusters) {
				softWorld->setDrawFlags(softWorld->getDrawFlags() | fDrawFlags::Clusters);
			} else {
				softWorld->setDrawFlags(softWorld->getDrawFlags() & (~fDrawFlags::Clusters));
			}
		}
		return p;
	}

	bool getTexturing(void) const {
		return m_shapeDrawer->hasTextureEnabled();
	}

	bool getShadows(void) const { return m_drawShadows; }


	int getDebugMode(void) { return m_debugMode; }

	void setDebugMode(int mode);

	void toggleDebugMode(int mode);

	int getPickMode() { return m_pickMode; }
	void setPickMode(int mode) { m_pickMode = mode; }

	void setAzi(float azi) { m_azi = azi; }

	void setCameraUp(const btVector3& camUp) { m_cameraUp = camUp; }
	void setCameraForwardAxis(int axis) { m_forwardAxis = axis; }

	void setTracking(btRigidBody *trackingBody, btScalar minDistance, btScalar maxDistance);

	virtual void prepareFrame(void);

	void toggleIdle(void);

	virtual void updateCamera(void);

	void setCameraPosition(btScalar x, btScalar y, btScalar z) {
		m_cameraPosition.setValue(x, y, z);
	}

	void setCamaeraPosition(btVector3 &pos) {
		m_cameraPosition.setValue(pos.x(), pos.y(), pos.z());
	}

	btVector3 getCameraPosition(void) { return m_cameraPosition; }
	
	void setCameraTargetPosition(btScalar x, btScalar y, btScalar z) {
		m_cameraTargetPosition.setValue(x, y, z);
	}
	void setCameraTargetPosition(btVector3 &pos) {
		m_cameraTargetPosition.setValue(pos.x(), pos.y(), pos.z());
	}
	btVector3 getCameraTargetPosition(void) { return m_cameraTargetPosition; }

	btScalar getCameraHeight(void) { return m_cameraHeight; }
	void setCameraHeight(btScalar height) { m_cameraHeight = height; }

	void setCameraDistance(btScalar distance) { m_cameraDistance  = distance; }
	void setCameraDistance(btScalar distance, btScalar min, btScalar max) {
		m_cameraDistance  = distance;
		m_minCameraDistance = min;
		m_maxCameraDistance = max;
	}

	btScalar getCameraDistance(void) { return m_cameraDistance; }

	btScalar getDeltaTimeMicroseconds(void) {
#ifdef USE_BT_CLOCK
		btScalar dt = (btScalar)m_clock.getTimeMicroseconds();
		m_clock.reset();
		return dt;
#else
		return btScalar(16666.);
#endif
	}
	void setFrustumZPlanes(float zNear, float zFar) {
		m_frustumZNear = zNear;
		m_frustumZFar = zFar;
	}
	virtual void clientResetScene(void);

	// Demo functions
	virtual void setShootBoxInitialSpeed(float speed);
	virtual void setShootBoxShape (void);
	virtual void shootBox(const btVector3& destination);


	btVector3 getRayTo(int x,int y);

	btRigidBody *localCreateRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape);

	virtual bool processTouch(bool touched, int screen_x, int screen_y);
	virtual bool touchMove(int x, int y);

	bool isPicking(void) { return (m_pickedBody != NULL); }

	virtual void lostGLContext();
	virtual void resize(int w, int h);

	virtual void renderFrame(void);
	virtual void renderProfile(void);

	void stepLeft(void);
	void stepRight(void);
	void stepFront(void);
	void stepBack(void);
	void zoomIn(void);
	void zoomOut(void);

	bool isIdle(void) const { return m_idle; }

	void setIdle(bool idle) { m_idle = idle; }

};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* BULLETWORLDDRAW_H_ */
