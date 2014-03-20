/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: bulletWorldDraw.cpp
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

#include "bulletWorldDraw.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"	// for picking
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"	// for picking
#include "BulletDynamics/ConstraintSolver/btConstraintSolver.h"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.h"

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include "LinearMath/btIDebugDraw.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btSerializer.h"

#include "GLDebugFont.h"
#include "glues/glues.h"

#include "variables.h"
#include "bulletmanager.h"
#include "physicsWorld.h"

#define LOCAL_DEBUG 0

#define SHOW_NUM_DEEP_PENETRATIONS 1

#ifdef SHOW_NUM_DEEP_PENETRATIONS
extern int gNumDeepPenetrationChecks;
extern int gNumGjkChecks;

#endif // #ifdef SHOW_NUM_DEEP_PENETRATIONS

#include "GLDebugDrawer.h"
static GLDebugDrawer gDebugDrawer;

BulletWorldDraw::BulletWorldDraw(btDynamicsWorld *dynamicsWorld, btSoftBodyWorldInfo *softBodyWorldInfo)
:
m_dynamicsWorld(dynamicsWorld),
m_softBodyWorldInfo(softBodyWorldInfo),
m_debugFont(NULL),
m_pickConstraint(NULL),
m_shootBoxShape(NULL),
m_debugMode(0),
m_pickMode(0),
m_pickedBody(NULL),
m_ele(20.f),
m_azi(0.f),
m_cameraDistance(15.f),
m_minCameraDistance(-BT_LARGE_FLOAT),
m_maxCameraDistance(BT_LARGE_FLOAT),
m_cameraHeight(0.f),	//
m_cameraPosition(0.f, 0.f, 0.f),
m_cameraTargetPosition(0.f, 0.f, 0.f),
m_cameraMode(0),
m_trackingBody(NULL),
m_cameraUp(0, 1, 0),
m_frustumZNear(1.f),
m_frustumZFar(10000.f),
m_scaleBottom(0.5f),
m_scaleFactor(2.f),
m_zoomStepSize(0.4),
m_forwardAxis(2),
m_ortho(0),
m_mouseOldX(0),
m_mouseOldY(0),
m_mouseButtons(0),
m_modifierKeys(0),
m_glutScreenWidth(0),
m_glutScreenHeight(0),
m_ShootBoxInitialSpeed(40.f),
m_stepping(true),
m_singleStep(false),
m_idle(false),
m_drawShadows(false),
m_drawClusters(false),
m_sundirection(btVector3(1, -2, 1) * 1000),
m_defaultContactProcessingThreshold(BT_LARGE_FLOAT),
m_raycast(false),	// switch rayCast mode enable/disable(enable leads simulation very very slow)
m_autocam(false) {
	// constructor
#ifndef BT_NO_PROFILE
	m_profileIterator = CProfileManager::Get_Iterator();
#endif //BT_NO_PROFILE
#if LOCAL_DEBUG
	LOGI("BulletWorldDraw::BulletWorldDraw");
	LOGI("BulletWorldDraw::BulletWorldDraw:m_cameraTargetPosition=%f,%f,%f", m_cameraTargetPosition.getX(), m_cameraTargetPosition.getY(), m_cameraTargetPosition.getZ());
#endif

	m_shapeDrawer = new GLShapeDrawer();
	m_shapeDrawer->enableTexture(true);
	m_drawShadows = false;
	m_dynamicsWorld->setDebugDrawer(&gDebugDrawer);
}

BulletWorldDraw::~BulletWorldDraw() {
	// destructor
#ifndef BT_NO_PROFILE
	CProfileManager::Release_Iterator(m_profileIterator);
#endif //BT_NO_PROFILE
	SAFE_DELETE(m_debugFont);
	SAFE_DELETE(m_shootBoxShape);
	SAFE_DELETE(m_shapeDrawer);
}

void BulletWorldDraw::overrideGLShapeDrawer(GLShapeDrawer *shapeDrawer) {
	shapeDrawer->enableTexture(m_shapeDrawer->hasTextureEnabled());
	SAFE_DELETE(m_shapeDrawer);
	m_shapeDrawer = shapeDrawer;
}

void BulletWorldDraw::toggleIdle() {
	m_idle = !m_idle;
/*	if (m_idle) {
		m_idle = false;
	} else {
		m_idle = true;
	} */
}

/**
 * set object that camera should track
 */
void BulletWorldDraw::setTracking(
	btRigidBody *trackingBody, btScalar minDistance, btScalar maxDistance) {

	if (trackingBody) {
		m_trackingBody = trackingBody;
		m_minCameraDistance = minDistance;
		m_maxCameraDistance = maxDistance;
		m_cameraMode |= CAMERAMODE_TRACKING;
	} else {
		m_cameraMode &= ~CAMERAMODE_TRACKING;
		m_trackingBody = NULL;
	}
}

const float STEPSIZE = 5;

void BulletWorldDraw::stepLeft(void) {
	m_azi -= STEPSIZE; if (m_azi < 0) m_azi += 360; updateCamera();
}

void BulletWorldDraw::stepRight(void) {
	m_azi += STEPSIZE; if (m_azi >= 360) m_azi -= 360; updateCamera();
}

void BulletWorldDraw::stepFront(void) {
	m_ele += STEPSIZE; if (m_ele >= 360) m_ele -= 360; updateCamera();
}

void BulletWorldDraw::stepBack(void) {
	m_ele -= STEPSIZE; if (m_ele < 0) m_ele += 360; updateCamera();
}

void BulletWorldDraw::zoomIn(void) {
	m_cameraDistance -= btScalar(m_zoomStepSize); updateCamera();
	if (m_cameraDistance < btScalar(0.1))
		m_cameraDistance = btScalar(0.1);
}

void BulletWorldDraw::zoomOut(void) {
	m_cameraDistance += btScalar(m_zoomStepSize); updateCamera();
}

void BulletWorldDraw::lostGLContext() {
	if (m_debugFont)
		m_debugFont->releaseFont();
	m_shapeDrawer->releaseTexture();
}

/**
 * method when screen size changed
 */
void BulletWorldDraw::resize(int width, int height) {
#if LOCAL_DEBUG
	LOGI("BulletWorldDraw#resize:");
#endif
	if (m_debugFont) {
		m_debugFont->resetFont(width, height);
	} else {
		m_debugFont = new GLDebugFont(width, height);
	}

	m_glutScreenWidth = width;
	m_glutScreenHeight = height;

	glViewport(0, 0, width, height);
	updateCamera();
}

void BulletWorldDraw::toggleDebugMode(int mode)
{
	switch (mode) {
	case btIDebugDraw::DBG_NoHelpText:
		if (m_debugMode & btIDebugDraw::DBG_NoHelpText)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_NoHelpText);
		else
			m_debugMode |= btIDebugDraw::DBG_NoHelpText;
		break;

	case btIDebugDraw::DBG_DrawWireframe:
		if (m_debugMode & btIDebugDraw::DBG_DrawWireframe)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DrawWireframe);
		else
			m_debugMode |= btIDebugDraw::DBG_DrawWireframe;
		break;

	case btIDebugDraw::DBG_ProfileTimings:
		if (m_debugMode & btIDebugDraw::DBG_ProfileTimings)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_ProfileTimings);
		else
			m_debugMode |= btIDebugDraw::DBG_ProfileTimings;
		break;
	case btIDebugDraw::DBG_EnableSatComparison:
		if (m_debugMode & btIDebugDraw::DBG_EnableSatComparison)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_EnableSatComparison);
		else
			m_debugMode |= btIDebugDraw::DBG_EnableSatComparison;
		break;

	case btIDebugDraw::DBG_DisableBulletLCP:
		if (m_debugMode & btIDebugDraw::DBG_DisableBulletLCP)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DisableBulletLCP);
		else
			m_debugMode |= btIDebugDraw::DBG_DisableBulletLCP;
		break;
    case btIDebugDraw::DBG_DrawNormals:
		if (m_debugMode & btIDebugDraw::DBG_DrawNormals)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DrawNormals);
		else
			m_debugMode |= btIDebugDraw::DBG_DrawNormals;
		break;

	case btIDebugDraw::DBG_DrawText:
		if (m_debugMode & btIDebugDraw::DBG_DrawText)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DrawText);
		else
			m_debugMode |= btIDebugDraw::DBG_DrawText;
		break;
	case btIDebugDraw::DBG_DrawFeaturesText:
		if (m_debugMode & btIDebugDraw::DBG_DrawFeaturesText)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DrawFeaturesText);
		else
			m_debugMode |= btIDebugDraw::DBG_DrawFeaturesText;
		break;
	case btIDebugDraw::DBG_DrawAabb:
		if (m_debugMode & btIDebugDraw::DBG_DrawAabb)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DrawAabb);
		else
			m_debugMode |= btIDebugDraw::DBG_DrawAabb;
		break;
	case btIDebugDraw::DBG_DrawContactPoints:
		if (m_debugMode & btIDebugDraw::DBG_DrawContactPoints)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DrawContactPoints);
		else
			m_debugMode |= btIDebugDraw::DBG_DrawContactPoints;
		break;
	case btIDebugDraw::DBG_DrawConstraints:
		if (m_debugMode & btIDebugDraw::DBG_DrawConstraints)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DrawConstraints);
		else
			m_debugMode |= btIDebugDraw::DBG_DrawConstraints;
		break;
	case btIDebugDraw::DBG_DrawConstraintLimits:
		if (m_debugMode & btIDebugDraw::DBG_DrawConstraintLimits)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_DrawConstraintLimits);
		else
			m_debugMode |= btIDebugDraw::DBG_DrawConstraintLimits;
		break;

	case btIDebugDraw::DBG_NoDeactivation:
		if (m_debugMode & btIDebugDraw::DBG_NoDeactivation)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_NoDeactivation);
		else
			m_debugMode |= btIDebugDraw::DBG_NoDeactivation;
		if (m_debugMode & btIDebugDraw::DBG_NoDeactivation) {
			gDisableDeactivation = true;
		} else {
			gDisableDeactivation = false;
		}
		break;
	case btIDebugDraw::DBG_EnableCCD:
		if (m_debugMode & btIDebugDraw::DBG_EnableCCD)
			m_debugMode = m_debugMode & (~btIDebugDraw::DBG_EnableCCD);
		else
			m_debugMode |= btIDebugDraw::DBG_EnableCCD;
		break;
	default:
		return;
	}
	if (getDynamicsWorld() && getDynamicsWorld()->getDebugDrawer())
		getDynamicsWorld()->getDebugDrawer()->setDebugMode(m_debugMode);
}

void BulletWorldDraw::setDebugMode(int mode) {
	m_debugMode = mode;
	if (getDynamicsWorld() && getDynamicsWorld()->getDebugDrawer())
		getDynamicsWorld()->getDebugDrawer()->setDebugMode(m_debugMode);
}

#define NUM_SPHERES_ON_DIAGONAL 9

void BulletWorldDraw::setShootBoxShape(void) {
	if (UNLIKELY(!m_shootBoxShape)) {
		btBoxShape *box = new btBoxShape(btVector3(0.5, 0.5, 0.5));
		//  box->initializePolyhedralFeatures();
		m_shootBoxShape = box;
	}
}

void BulletWorldDraw::setShootBoxInitialSpeed(float speed) {
	if (LIKELY(speed > 0))
		m_ShootBoxInitialSpeed = speed;
}

void BulletWorldDraw::shootBox(const btVector3 &destination) {

	if (LIKELY(m_dynamicsWorld)) {
		float mass = 1.f;
		btTransform startTransform;
		startTransform.setIdentity();
		btVector3 camPos = getCameraPosition();
		startTransform.setOrigin(camPos);

		setShootBoxShape();

		btRigidBody *body = this->localCreateRigidBody(mass, startTransform, m_shootBoxShape);
		body->setLinearFactor(btVector3(1, 1, 1));
		//body->setRestitution(1);

		btVector3 linVel(destination[0] - camPos[0], destination[1] - camPos[1], destination[2] - camPos[2]);
		linVel.normalize();
		linVel *= m_ShootBoxInitialSpeed;

		body->getWorldTransform().setOrigin(camPos);
		body->getWorldTransform().setRotation(btQuaternion(0, 0, 0, 1));
		body->setLinearVelocity(linVel);
		body->setAngularVelocity(btVector3(0,0,0));
		body->setCcdMotionThreshold(0.5);
		body->setCcdSweptSphereRadius(0.4f);//value should be smaller (embedded) than the half extends of the box (see ::setShootBoxShape)
//		printf("shootBox uid=%d\n", body->getBroadphaseHandle()->getUid());
//		printf("camPos=%f,%f,%f\n",camPos.getX(),camPos.getY(),camPos.getZ());
//		printf("destination=%f,%f,%f\n",destination.getX(),destination.getY(),destination.getZ());

	}
}


int gPickingConstraintId = 0;
btVector3 gOldPickingPos;
btVector3 gHitPos(-1,-1,-1);
float gOldPickingDist  = 0.f;


/**
 * @param x screen position x (upper left is 0)
 * @param y screen position y (upper left is 0)
 */
btVector3 BulletWorldDraw::getRayTo(int x, int y) {

	if (m_ortho) {

		btScalar aspect;
		btVector3 extents;
		aspect = m_glutScreenWidth / (btScalar)m_glutScreenHeight;
		extents.setValue(aspect * 1.0f, 1.0f, 0);

		extents *= m_cameraDistance;
		btVector3 lower = m_cameraTargetPosition - extents;
		btVector3 upper = m_cameraTargetPosition + extents;

		btScalar u = x / btScalar(m_glutScreenWidth);
		btScalar v = (m_glutScreenHeight - y) / btScalar(m_glutScreenHeight);

		btVector3	p(0, 0, 0);
		p.setValue((1.0f - u) * lower.getX() + u * upper.getX(),(1.0f - v) * lower.getY() + v * upper.getY(),m_cameraTargetPosition.getZ());
		return p;
	}

	float top = 1.f;
	float bottom = -1.f;
	float nearPlane = 1.f;
	float tanFov = (top - bottom) * 0.5f / nearPlane;
	float fov = btScalar(2.0) * btAtan(tanFov);

	btVector3 rayFrom = getCameraPosition();
	btVector3 rayForward = (getCameraTargetPosition()-getCameraPosition());
	rayForward.normalize();
	float farPlane = 10000.f;
	rayForward *= farPlane;

	btVector3 rightOffset;
	btVector3 vertical = m_cameraUp;

	btVector3 hor;
	hor = rayForward.cross(vertical);
	hor.normalize();
	vertical = hor.cross(rayForward);
	vertical.normalize();

	float tanfov = tanf(0.5f * fov);


	hor *= 2.f * farPlane * tanfov;
	vertical *= 2.f * farPlane * tanfov;

	btScalar aspect;

	aspect = m_glutScreenWidth / (btScalar)m_glutScreenHeight;

	hor *= aspect;

	btVector3 rayToCenter = rayFrom + rayForward;
	btVector3 dHor = hor * 1.f / float(m_glutScreenWidth);
	btVector3 dVert = vertical * 1.f / float(m_glutScreenHeight);

	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
	rayTo += btScalar(x) * dHor;
	rayTo -= btScalar(y) * dVert;
	return rayTo;
}

btScalar mousePickClamping = 30.f;

/**
 * method when user touched screen
 * @param state 0: TOUCH_DOWN, other than 0: TOUCH_UP
 * @param screen_x screen coordinate x
 * @param screen_y screen coordinate y
 */
bool BulletWorldDraw::processTouch(bool touched, int screen_x, int screen_y) {
	if (touched) {	// when touching
        m_mouseButtons |= 1;	// sete mouse state to pressing left button bit0: left, bit1:middile, bit2:right
    } else {			// release finger
        m_mouseButtons = 0;
    }
	m_mouseOldX = screen_x;
    m_mouseOldY = screen_y;

	btVector3 rayTo = getRayTo(screen_x,screen_y);

	if (touched) {	// TOUCH_DOWN
		//add a point to point constraint for picking
		if (m_dynamicsWorld) {

			btVector3 rayFrom;
			if (m_ortho) {
				rayFrom = rayTo;
				rayFrom.setZ(-100.f);
			} else {
				rayFrom = m_cameraPosition;
			}

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom,rayTo);
			m_dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);
			if (rayCallback.hasHit()) {
				btRigidBody *body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
				if (body) {
					//other exclusions?
					if (!(body->isStaticObject() || body->isKinematicObject())) {
						m_pickedBody = body;
						m_pickedBody->setActivationState(DISABLE_DEACTIVATION);

						btVector3 pickPos = rayCallback.m_hitPointWorld;
						//printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());

						btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;

						if ((m_pickMode & PICKMODE_HANG) == 0) {
							// PICKMODE_HANG bit is off = translation
							btTransform tr;
							tr.setIdentity();
							tr.setOrigin(localPivot);
							btGeneric6DofConstraint *dof6 = new btGeneric6DofConstraint(*body, tr, false);
							dof6->setLinearLowerLimit(btVector3(0, 0, 0));
							dof6->setLinearUpperLimit(btVector3(0, 0, 0));
							dof6->setAngularLowerLimit(btVector3(0, 0, 0));
							dof6->setAngularUpperLimit(btVector3(0, 0, 0));

							m_dynamicsWorld->addConstraint(dof6, true);
							m_pickConstraint = dof6;

							dof6->setParam(BT_CONSTRAINT_STOP_CFM, 0.8, 0);
							dof6->setParam(BT_CONSTRAINT_STOP_CFM, 0.8, 1);
							dof6->setParam(BT_CONSTRAINT_STOP_CFM, 0.8, 2);
							dof6->setParam(BT_CONSTRAINT_STOP_CFM, 0.8, 3);
							dof6->setParam(BT_CONSTRAINT_STOP_CFM, 0.8, 4);
							dof6->setParam(BT_CONSTRAINT_STOP_CFM, 0.8, 5);

							dof6->setParam(BT_CONSTRAINT_STOP_ERP, 0.1, 0);
							dof6->setParam(BT_CONSTRAINT_STOP_ERP, 0.1, 1);
							dof6->setParam(BT_CONSTRAINT_STOP_ERP, 0.1, 2);
							dof6->setParam(BT_CONSTRAINT_STOP_ERP, 0.1, 3);
							dof6->setParam(BT_CONSTRAINT_STOP_ERP, 0.1, 4);
							dof6->setParam(BT_CONSTRAINT_STOP_ERP, 0.1, 5);
						} else {
							// PICKMODE_HANG bit is on = hang and move objet
							btPoint2PointConstraint *p2p = new btPoint2PointConstraint(*body,localPivot);
							m_dynamicsWorld->addConstraint(p2p, true);
							m_pickConstraint = p2p;
							p2p->m_setting.m_impulseClamp = mousePickClamping;
							// very weak constraint for picking
							p2p->m_setting.m_tau = 0.001f;
/*
							p2p->setParam(BT_CONSTRAINT_CFM, 0.8, 0);
							p2p->setParam(BT_CONSTRAINT_CFM, 0.8, 1);
							p2p->setParam(BT_CONSTRAINT_CFM, 0.8, 2);
							p2p->setParam(BT_CONSTRAINT_ERP, 0.1, 0);
							p2p->setParam(BT_CONSTRAINT_ERP, 0.1, 1);
							p2p->setParam(BT_CONSTRAINT_ERP, 0.1, 2);
							*/
						}

						// save touch position for dragging
						gOldPickingPos = rayTo;
						gHitPos = pickPos;

						gOldPickingDist  = (pickPos-rayFrom).length();
					}
				}
			}
		}

	} else { // TOUCH_UP
		removePickingConstraint();
	}
	return (m_pickedBody != NULL);	// return true when picking
}

void BulletWorldDraw::removePickingConstraint() {
	if (m_pickConstraint && m_dynamicsWorld) {
		m_dynamicsWorld->removeConstraint(m_pickConstraint);
		delete m_pickConstraint;
		//printf("removed constraint %i",gPickingConstraintId);
		m_pickConstraint = 0;
		m_pickedBody->forceActivationState(ACTIVE_TAG);
		m_pickedBody->setDeactivationTime(0.f);
		m_pickedBody = 0;
	}
}

/**
 * TOUCH_MOVE
 */
bool BulletWorldDraw::touchMove(int screen_x, int screen_y) {
	LOGI("touchMove:");
	if (m_pickConstraint) {
		// move the constraint pivot

		if (m_pickConstraint->getConstraintType() == D6_CONSTRAINT_TYPE) {
			btGeneric6DofConstraint *pickCon = static_cast<btGeneric6DofConstraint*>(m_pickConstraint);
			if (pickCon) {
				//keep it at the same picking distance

				btVector3 newRayTo = getRayTo(screen_x,screen_y);
				btVector3 rayFrom;
				btVector3 oldPivotInB = pickCon->getFrameOffsetA().getOrigin();

				btVector3 newPivotB;
				if (m_ortho) {
					newPivotB = oldPivotInB;
					newPivotB.setX(newRayTo.getX());
					newPivotB.setY(newRayTo.getY());
				} else {
					rayFrom = m_cameraPosition;
					btVector3 dir = newRayTo-rayFrom;
					dir.normalize();
					dir *= gOldPickingDist;

					newPivotB = rayFrom + dir;
				}
				pickCon->getFrameOffsetA().setOrigin(newPivotB);
			}

		} else {
			btPoint2PointConstraint *pickCon = static_cast<btPoint2PointConstraint*>(m_pickConstraint);
			if (pickCon) {
				// keep it at the same picking distance

				btVector3 newRayTo = getRayTo(screen_x,screen_y);
				btVector3 rayFrom;
				btVector3 oldPivotInB = pickCon->getPivotInB();
				btVector3 newPivotB;
				if (m_ortho) {
					newPivotB = oldPivotInB;
					newPivotB.setX(newRayTo.getX());
					newPivotB.setY(newRayTo.getY());
				} else {
					rayFrom = m_cameraPosition;
					btVector3 dir = newRayTo-rayFrom;
					dir.normalize();
					dir *= gOldPickingDist;

					newPivotB = rayFrom + dir;
				}
				pickCon->setPivotB(newPivotB);
			}
		}
	}

	float dx, dy;
    dx = btScalar(screen_x) - m_mouseOldX;
    dy = btScalar(screen_y) - m_mouseOldY;

    // when camera needs to move according to the picked object position
    if (m_pickMode & PICKMODE_TRACKING) {	// when PICKMODE_TRACKING bit is on
		if (m_mouseButtons & 2) {
			btVector3 hor = getRayTo(0,0) - getRayTo(1, 0);
			btVector3 vert = getRayTo(0,0) - getRayTo(0, 1);
			btScalar multiplierX = btScalar(0.001);
			btScalar multiplierY = btScalar(0.001);
			if (m_ortho) {
				multiplierX = 1;
				multiplierY = 1;
			}

			m_cameraTargetPosition += hor * dx * multiplierX;
			m_cameraTargetPosition += vert * dy * multiplierY;
			LOGI("touchMove:m_cameraTargetPosition=%f,%f,%f", m_cameraTargetPosition.getX(), m_cameraTargetPosition.getY(), m_cameraTargetPosition.getZ());
		}

		if (m_mouseButtons & (8) && m_mouseButtons & 1) {

		} else if (m_mouseButtons & 1) {
			m_azi += dx * btScalar(0.2);
			m_azi = fmodf(m_azi, btScalar(360.f));
			m_ele += dy * btScalar(0.2);
			m_ele = fmodf(m_ele, btScalar(180.f));
		} else if (m_mouseButtons & 4) {
			m_cameraDistance -= dy * btScalar(0.02f);
			if (m_cameraDistance<btScalar(0.1))
				m_cameraDistance = btScalar(0.1);
		}
	}

	m_mouseOldX = screen_x;
    m_mouseOldY = screen_y;
	updateCamera();
	return (m_pickedBody != NULL);	// return true when picking
}


btRigidBody *BulletWorldDraw::localCreateRigidBody(
	float mass, const btTransform &startTransform, btCollisionShape *shape) {

	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#if USE_MOTIONSTATE
	btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody *body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
	btRigidBody *body = new btRigidBody(mass, 0, shape, localInertia);
	body->setWorldTransform(startTransform);
#endif

	m_dynamicsWorld->addRigidBody(body);

	return body;
}

//See http://www.lighthouse3d.com/opengl/glut/index.php?bmpfontortho
void BulletWorldDraw::setOrthographicProjection(void) {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the
	// settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
//	gluOrtho2f(0, m_glutScreenWidth, 0, m_glutScreenHeight);
	glOrthof(0, m_glutScreenWidth, 0, m_glutScreenHeight, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(btScalar(0), btScalar(-m_glutScreenHeight), btScalar(0));
}

void BulletWorldDraw::resetPerspectiveProjection(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
//	updateCamera();	// comment out to improve performance
}

#ifndef BT_NO_PROFILE
extern CProfileIterator *m_profileIterator;
#endif

void BulletWorldDraw::displayProfileString(int xOffset, int yStart, char *message)
{
//	glRasterPos3f(btScalar(xOffset),btScalar(yStart),btScalar(0));	// TODO
	glTranslatef(btScalar(xOffset),btScalar(yStart),btScalar(0));
	m_debugFont->GLDebugDrawString(xOffset, yStart, message);
}


void BulletWorldDraw::showProfileInfo(int &xOffset, int &yStart, int yIncr)
{
#ifndef BT_NO_PROFILE

	static double time_since_reset = 0.f;
	if (!m_idle) {
		time_since_reset = CProfileManager::Get_Time_Since_Reset();
	}

	{
		//recompute profiling data, and store profile strings

		char blockTime[128];

		double totalTime = 0;

		int frames_since_reset = CProfileManager::Get_Frame_Count_Since_Reset();

		m_profileIterator->First();

		double parent_time = m_profileIterator->Is_Root() ? time_since_reset : m_profileIterator->Get_Current_Parent_Total_Time();

		{
			sprintf(blockTime,"--- Profiling: %s (total running time: %8.3f ms) ---", m_profileIterator->Get_Current_Parent_Name(), parent_time );
			displayProfileString(xOffset, yStart, blockTime);
			yStart += yIncr;
//			sprintf(blockTime,"press (1,2...) to display child timings, or 0 for parent" );
//			displayProfileString(xOffset, yStart, blockTime);
//			yStart += yIncr;

		}

		double accumulated_time = 0.f;

		for (int i = 0; !m_profileIterator->Is_Done(); m_profileIterator->Next()) {
			double current_total_time = m_profileIterator->Get_Current_Total_Time();
			accumulated_time += current_total_time;
			double fraction = parent_time > SIMD_EPSILON ? (current_total_time / parent_time) * 100 : 0.f;

			sprintf(blockTime,"%d -- %s (%6.2f %%) :: %8.3f ms / frame (%2d calls)",
				++i, m_profileIterator->Get_Current_Name(), fraction,
				(current_total_time / (double)frames_since_reset),m_profileIterator->Get_Current_Total_Calls());
			displayProfileString(xOffset, yStart, blockTime);
			yStart += yIncr;
			totalTime += current_total_time;
		}

		sprintf(blockTime,"%s (%8.3f %%) :: %8.3f ms", "Unaccounted",
			// (min(0, time_since_reset - totalTime) / time_since_reset) * 100);
			parent_time > SIMD_EPSILON ? ((parent_time - accumulated_time) / parent_time) * 100 : 0.f, parent_time - accumulated_time);

		displayProfileString(xOffset, yStart, blockTime);
		yStart += yIncr;



		sprintf(blockTime,"-------------------------------------------------");
		displayProfileString(xOffset, yStart, blockTime);
		yStart += yIncr;

	}
#endif//BT_NO_PROFILE

}

void BulletWorldDraw::updateCamera(void) {
#if LOCAL_DEBUG
	LOGI("BulletWorldDraw#updateCamera:");
#endif
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (m_trackingBody && (m_cameraMode & CAMERAMODE_TRACKING)
		&& ((m_pickMode & PICKMODE_TRACKING) != PICKMODE_TRACKING)) {	// not a pick&track mode?
		// when tracking... set point of regardto object position
		btTransform trackingWorldTrans;
		m_trackingBody->getMotionState()->getWorldTransform(trackingWorldTrans);
		m_cameraTargetPosition = trackingWorldTrans.getOrigin();
		btVector3 cameraPos = m_cameraPosition;
		// set cameraheight
		if (m_cameraMode & CAMERAMODE_FORCE_ZAXIS_UP) {
			cameraPos[2] = (15.0 * cameraPos[2] + m_cameraTargetPosition[2] + m_cameraHeight) / 16.0;
		} else {
			cameraPos[1] = (15.0 * cameraPos[1] + m_cameraTargetPosition[1] + m_cameraHeight) / 16.0;
		}
		// keep distance in a constant range between camera and point of regard
		btVector3 camToObject = m_cameraTargetPosition - cameraPos;
		float cameraDistance = camToObject.length();
		if (cameraDistance) {
			float correctionFactor = 0.f;
			if (cameraDistance < m_minCameraDistance) {
				correctionFactor = 0.15 * (m_minCameraDistance - cameraDistance) / cameraDistance;
			}
			if (cameraDistance > m_maxCameraDistance) {
				correctionFactor = 0.15 * (m_maxCameraDistance - cameraDistance) / cameraDistance;
			}
			cameraPos -= correctionFactor * camToObject;
			if (LIKELY(!isnan(correctionFactor)))
				m_cameraPosition = cameraPos;
		}
	} else {
		// when not tracking... rotate and move depending on the camera rotation
		btScalar rele = m_ele * btScalar(0.01745329251994329547);// rads per deg
		btScalar razi = m_azi * btScalar(0.01745329251994329547);// rads per deg

		btQuaternion rot(m_cameraUp, razi);

		btVector3 eyePos(0, 0, 0);
		eyePos[m_forwardAxis] = -m_cameraDistance;

		btVector3 forward(eyePos[0], eyePos[1], eyePos[2]);
		if (forward.length2() < SIMD_EPSILON) {
			forward.setValue(1.f, 0.f, 0.f);
		}
		btVector3 right = m_cameraUp.cross(forward);
		btQuaternion roll(right, -rele);

		eyePos = btMatrix3x3(rot) * btMatrix3x3(roll) * eyePos;

		m_cameraPosition[0] = eyePos.getX();
		m_cameraPosition[1] = eyePos.getY();
		m_cameraPosition[2] = eyePos.getZ();
		m_cameraPosition += m_cameraTargetPosition;
	}

	if (!m_glutScreenWidth && !m_glutScreenHeight) {
		LOGI("BulletWorldDraw::updateCamera:ScreenSize not set");
		return;
	}

	btScalar aspect = m_glutScreenWidth / (btScalar)m_glutScreenHeight;
	btVector3 extents(aspect * 1.0f, 1.0f, 0);

	if (m_ortho) {
		// reset matrix
		glLoadIdentity();

		extents *= m_cameraDistance;
		btVector3 lower = m_cameraTargetPosition - extents;
		btVector3 upper = m_cameraTargetPosition + extents;
		//gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
		glOrthof(lower.getX(), upper.getX(), lower.getY(), upper.getY(), -1000, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glTranslatef(100, 210, 0);
	} else {
//		glFrustum (-aspect, aspect, -1.0, 1.0, 1.0, 10000.0);
		glFrustumf(-aspect * m_frustumZNear, aspect * m_frustumZNear, -m_frustumZNear, m_frustumZNear, m_frustumZNear, m_frustumZFar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2],
			m_cameraTargetPosition[0], m_cameraTargetPosition[1], m_cameraTargetPosition[2],
			m_cameraUp.getX(), m_cameraUp.getY(), m_cameraUp.getZ());
	}

}

void BulletWorldDraw::resetFrame(void) {
#if LOCAL_DEBUG
	LOGV("BulletWorldDraw#resetFrame:");
#endif
	overrideGLShapeDrawer(new GLShapeDrawer());
}

void BulletWorldDraw::prepareFrame(void) {
#if LOCAL_DEBUG
//	LOGD("BulletWorldDraw#prepareFrame:");
#endif

	GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
	GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
	GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
	/*	light_position is NOT default value	*/
	GLfloat light_position0[] = { btScalar(1.0), btScalar(10.0), btScalar(1.0), btScalar(0.0 )};
	GLfloat light_position1[] = { btScalar(-1.0), btScalar(-10.0), btScalar(-1.0), btScalar(0.0) };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(btScalar(0.7),btScalar(0.7),btScalar(0.7),btScalar(0));	// 背景を灰色で塗りつぶす
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

}


//
void BulletWorldDraw::renderscene(int pass)
{
	btScalar	m[16];
	btMatrix3x3	rot; rot.setIdentity();
	const int numObjects = m_dynamicsWorld->getNumCollisionObjects();
#if LOCAL_DEBUG
//	LOGD("BulletWorldDraw#renderscene:pass=%d,numObjects=%d", pass, numObjects);
#endif
	btVector3 wireColor(1, 0, 0);
	for (int i = 0; i < numObjects; i++) {
		btCollisionObject	*colObj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody			*body = btRigidBody::upcast(colObj);
		if (body && body->getMotionState()) {
			btDefaultMotionState *myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			rot = myMotionState->m_graphicsWorldTrans.getBasis();
		} else {
#if LOCAL_DEBUG
			if (body)
				LOGD("BulletWorldDraw#renderscene:getMotionState is null@%d,type=%d", i, body->getCollisionShape()->getShapeType());
#endif
			colObj->getWorldTransform().getOpenGLMatrix(m);
			rot = colObj->getWorldTransform().getBasis();
		}
		btVector3 wireColor(1.f, 1.0f, 0.5f); // WANTS_DEACTIVATION=3@btCollisionObject.h
		if (i & 1) wireColor = btVector3(0.f, 0.0f, 1.f);
		// color differently for active, sleeping, wantsdeactivation states
		if (colObj->getActivationState() == ACTIVE_TAG) { //ACTIVE_TAG=1@btCollisionObject.h
			if (i & 1) {
				wireColor += btVector3 (1.f, 0.f, 0.f);
			} else {
				wireColor += btVector3 (.5f, 0.f, 0.f);
			}
		}
		if (colObj->getActivationState() == ISLAND_SLEEPING) { // ISLAND_SLEEPING=2@btCollisionObject.h
#if LOCAL_DEBUG
//			LOGD("BulletWorldDraw#renderscene:ISLAND_SLEEPING@%d,type=%d", i, body->getCollisionShape()->getShapeType());
#endif
			if (i & 1) {
				wireColor += btVector3(0.f, 1.f, 0.f);
			} else {
				wireColor += btVector3(0.f, 0.5f, 0.f);
			}
		}

		btVector3 aabbMin, aabbMax;
		m_dynamicsWorld->getBroadphase()->getBroadphaseAabb(aabbMin, aabbMax);

		aabbMin -= btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
		aabbMax += btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
//		printf("aabbMin=(%f,%f,%f)\n", aabbMin.getX(), aabbMin.getY(), aabbMin.getZ());
//		printf("aabbMax=(%f,%f,%f)\n", aabbMax.getX(), aabbMax.getY(), aabbMax.getZ());
		m_dynamicsWorld->getDebugDrawer()->drawAabb(aabbMin, aabbMax, btVector3(1,1,1));

		if (LIKELY(!(getDebugMode() & btIDebugDraw::DBG_DrawWireframe))) {
			switch (pass) {
			case 0:	m_shapeDrawer->drawOpenGL(
						m, colObj->getCollisionShape(),
						wireColor, getDebugMode(), aabbMin, aabbMax);
					break;
			case 1:	m_shapeDrawer->drawShadow(
						m, m_sundirection * rot,
						colObj->getCollisionShape(), aabbMin, aabbMax);
					break;
			case 2:	m_shapeDrawer->drawOpenGL(
						m, colObj->getCollisionShape(),
						wireColor*btScalar(0.3), 0, aabbMin, aabbMax);
					break;
			}
		}
	}
}

void BulletWorldDraw::renderSoftbody(void) {
	btIDebugDraw *debugDrawer = m_dynamicsWorld->getDebugDrawer();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	m_dynamicsWorld->debugDrawWorld();

	// int debugMode = m_dynamicsWorld->getDebugDrawer()? m_dynamicsWorld->getDebugDrawer()->getDebugMode() : -1;

	btSoftRigidDynamicsWorld *softWorld = (btSoftRigidDynamicsWorld*)m_dynamicsWorld;
	btIDebugDraw *softDebugDrawer = softWorld->getDebugDrawer();

	btSoftBodyArray &softBodies = softWorld->getSoftBodyArray();

	if (softDebugDrawer && !(softDebugDrawer->getDebugMode() & (btIDebugDraw::DBG_DrawWireframe))) {
		for (int i = 0; i < softBodies.size(); i++) {
			btSoftBody *softBody = (btSoftBody*)softBodies[i];
			btSoftBodyHelpers::DrawFrame(softBody, softDebugDrawer);
			btSoftBodyHelpers::Draw(softBody, softDebugDrawer, softWorld->getDrawFlags());
		}
	}

	/* Bodies */
	btVector3 ps(0.f, 0.f, 0.f);
	if (m_autocam || m_raycast) {
		int nps = 0;

		for (int ib = 0; ib < softBodies.size(); ++ib) {
			btSoftBody *softBody = softBodies[ib];
			nps += softBody->m_nodes.size();
			for (int i = 0; i < softBody->m_nodes.size(); ++i) {
				ps += softBody->m_nodes[i].m_x;
			}
		}
		ps /= nps;
		if (m_autocam)
			m_cameraTargetPosition += (ps - m_cameraTargetPosition) * 0.05;
	}
	/* Ray cast		*/
	if (m_raycast) {
		/* Anm			*/
		if (!isIdle()) {
	//		m_animtime = m_clock.getTimeMilliseconds() / 1000.f;
			m_animtime = getTimeMilliseconds() / 1000.f;
		}
		/* Prepare rays	*/
		const int		res = 64;
		const btScalar	fres = res - 1;
		const btScalar	size = 8;
		const btScalar	dist = 10;
		btTransform		trs;
		trs.setOrigin(ps);
		btScalar rayLength = 1000.f;

		const btScalar angle = m_animtime * 0.2;
		trs.setRotation(btQuaternion(angle, SIMD_PI / 4,0));
		btVector3 dir = trs.getBasis() * btVector3(0, -1, 0);
		trs.setOrigin(ps - dir * dist);
		btAlignedObjectArray<btVector3>	origins;
		btAlignedObjectArray<btScalar>	fractions;
		origins.resize(res * res);
		fractions.resize(res * res, 1.f);
		for (int y = 0; y < res; ++y) {
			for (int x = 0; x < res; ++x) {
				const int	idx = y * res + x;
				origins[idx] = trs * btVector3(-size + size * 2 * x / fres, dist, -size + size * 2 * y / fres);
			}
		}
		/* Cast rays	*/
		{
//			m_clock.reset();
			if (softBodies.size()) {
				btVector3 *org = &origins[0];
				btScalar *fraction = &fractions[0];
				btSoftBody **psbs = &softBodies[0];
				btSoftBody::sRayCast results;
				for (int i = 0, ni = origins.size(), nb = softBodies.size(); i < ni; ++i) {
					for (int ib = 0; ib < nb; ++ib) {
						btVector3 rayFrom = *org;
						btVector3 rayTo = rayFrom + dir * rayLength;
						if (psbs[ib]->rayTest(rayFrom, rayTo, results)) {
							*fraction = results.fraction;
						}
					}
					++org; ++fraction;
				}
//				long ms = btMax<long>(m_clock.getTimeMilliseconds(), 1);
				long ms = btMax<long>(getTimeMilliseconds(), 1);
				long rayperseconds = (1000 * (origins.size() * softBodies.size())) / ms;
				printf("%6d ms (%6d rays/s)\r\n", int(ms), int(rayperseconds));
			}
		}
		/* Draw rays */
		const btVector3	c[] = {
			origins[0],
			origins[res - 1],
			origins[res * (res - 1)],
			origins[res * (res - 1) + res - 1] };
		debugDrawer->drawLine(c[0], c[1], btVector3(0, 0, 0));
		debugDrawer->drawLine(c[1], c[3], btVector3(0, 0, 0));
		debugDrawer->drawLine(c[3], c[2], btVector3(0, 0, 0));
		debugDrawer->drawLine(c[2], c[0], btVector3(0, 0, 0));
		for (int i = 0, ni = origins.size(); i < ni; ++i)  {
			const btScalar fraction = fractions[i];
			const btVector3 &org = origins[i];
			if (fraction < 1.f) {
				debugDrawer->drawLine(org, org + dir * rayLength * fraction, btVector3(1, 0, 0));
			} else {
				debugDrawer->drawLine(org, org - dir * rayLength * 0.1, btVector3(0, 0, 0));
			}
		}
	}
	/* Water level	*/
	if (m_softBodyWorldInfo->water_density > 0) {
		static const btVector3 axis[] = {
			btVector3(1, 0, 0),
			btVector3(0, 1, 0),
			btVector3(0, 0, 1) };
		const btVector3	c = btVector3((btScalar)0.25, (btScalar)0.25, 1);
		const btScalar	a = (btScalar)0.5;
		const btVector3	n = m_softBodyWorldInfo->water_normal;
		const btVector3	o = -n * m_softBodyWorldInfo->water_offset;
		const btVector3	x = btCross(n, axis[n.minAxis()]).normalized();
		const btVector3	y = btCross(x, n).normalized();
		const btScalar	s = 25;
		debugDrawer->drawTriangle(o - x * s - y * s, o + x * s - y * s, o + x * s + y * s, c, a);
		debugDrawer->drawTriangle(o - x * s - y * s, o + x * s + y * s, o - x * s + y * s, c, a);
	}
}

/**
 * render frame
 */
void BulletWorldDraw::renderFrame(void)
{
#if LOCAL_DEBUG
	LOGD("BulletWorldDraw#renderFrame:");
#endif
	prepareFrame();

	updateCamera();

	if (m_softBodyWorldInfo) {
		glDisable(GL_CULL_FACE);
		renderSoftbody();
	}

	if (LIKELY(m_dynamicsWorld)) {
		if (m_drawShadows) {
			glClear(GL_STENCIL_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			renderscene(0);

			glDisable(GL_LIGHTING);
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_STENCIL_TEST);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFFL);
			glFrontFace(GL_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			renderscene(1);
			glFrontFace(GL_CW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			renderscene(1);
			glFrontFace(GL_CCW);

//			glPolygonMode(GL_FRONT, GL_FILL);	// XXX this function does not exist in OpenGL/ES
//			glPolygonMode(GL_BACK, GL_FILL);	// XXX this function does not exist in OpenGL/ES
			glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
//			glEnable(GL_LIGHTING);
			glDepthMask(GL_TRUE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

			glDepthFunc(GL_LEQUAL);
			glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFFL);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glDisable(GL_LIGHTING);

			renderscene(2);

			glEnable(GL_LIGHTING);
			glDepthFunc(GL_LESS);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_CULL_FACE);
		} else {
			glDisable(GL_CULL_FACE);
			renderscene(0);
		}
		glDisable(GL_LIGHTING);
	} else {
		LOGE("m_dynamicsWorld is null");
	}
}

// draring profile text divied from renderFrame
void BulletWorldDraw::renderProfile(void) {

	if (LIKELY(m_dynamicsWorld)) {
		int	xOffset = 10;
		int yStart = 20;
		int yIncr = 20;

		glDisable(GL_LIGHTING);
		glColor4f(0, 0, 0, 1.f);

		if ((m_debugMode & btIDebugDraw::DBG_NoHelpText)==0) {
			setOrthographicProjection();
			showProfileInfo(xOffset, yStart, yIncr);
#ifdef USE_QUICKPROF
			if ( getDebugMode() & btIDebugDraw::DBG_ProfileTimings) {
				static int counter = 0;
				counter++;
				std::map<std::string, hidden::ProfileBlock*>::iterator iter;
				for (iter = btProfiler::mProfileBlocks.begin(); iter != btProfiler::mProfileBlocks.end(); ++iter) {
					char blockTime[128];
					sprintf(blockTime, "%s: %lf", &((*iter).first[0]), btProfiler::getBlockTime((*iter).first, btProfiler::BLOCK_CYCLE_SECONDS));//BLOCK_TOTAL_PERCENT));
					glRasterPos3f(xOffset,yStart,0);
					GLDebugDrawString(BMF_GetFont(BMF_kHelvetica10), blockTime);
					yStart += yIncr;
				}

			}
#endif //USE_QUICKPROF
			resetPerspectiveProjection();
		}
	} else {
		LOGE("m_dynamicsWorld is null");
	}
}

#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"

void BulletWorldDraw::clientResetScene(void)
{
	removePickingConstraint();

#ifdef SHOW_NUM_DEEP_PENETRATIONS
	gNumDeepPenetrationChecks = 0;
	gNumGjkChecks = 0;
#endif //SHOW_NUM_DEEP_PENETRATIONS

//	gNumClampedCcdMotions = 0;
	int numObjects = 0;
	int i;

	if (LIKELY(m_dynamicsWorld)) {
		int numConstraints = m_dynamicsWorld->getNumConstraints();
		for (i = 0; i < numConstraints; i++) {
			m_dynamicsWorld->getConstraint(0)->setEnabled(true);
		}
		numObjects = m_dynamicsWorld->getNumCollisionObjects();

		///create a copy of the array, not a reference!
		btCollisionObjectArray copyArray = m_dynamicsWorld->getCollisionObjectArray();

		for (i = 0; i < numObjects; i++) {
			btCollisionObject *colObj = copyArray[i];
			btRigidBody *body = btRigidBody::upcast(colObj);
			if (body) {
				if (body->getMotionState()) {
					btDefaultMotionState *myMotionState = (btDefaultMotionState*)body->getMotionState();
					myMotionState->m_graphicsWorldTrans = myMotionState->m_startWorldTrans;
					body->setCenterOfMassTransform( myMotionState->m_graphicsWorldTrans);
					colObj->setInterpolationWorldTransform( myMotionState->m_startWorldTrans);
					colObj->forceActivationState(ACTIVE_TAG);
					colObj->activate();
					colObj->setDeactivationTime(0);
					//colObj->setActivationState(WANTS_DEACTIVATION);
				}
				//removed cached contact points (this is not necessary if all objects have been removed from the dynamics world)
				if (m_dynamicsWorld->getBroadphase()->getOverlappingPairCache())
					m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(colObj->getBroadphaseHandle(),getDynamicsWorld()->getDispatcher());

				btRigidBody *body = btRigidBody::upcast(colObj);
				if (body && !body->isStaticObject()) {
					btRigidBody::upcast(colObj)->setLinearVelocity(btVector3(0, 0, 0));
					btRigidBody::upcast(colObj)->setAngularVelocity(btVector3(0, 0, 0));
				}
			}

		}

		///reset some internal cached data in the broadphase
		m_dynamicsWorld->getBroadphase()->resetPool(getDynamicsWorld()->getDispatcher());
		m_dynamicsWorld->getConstraintSolver()->reset();

	}

}
