/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: _onload.cpp
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

#include "_onload.h"
#include "utils.h"

#define LOCAL_DEBUG 0

extern int register_Vector3(JNIEnv *env);
extern int register_Quaternion(JNIEnv *env);
extern int register_Matrix3x3(JNIEnv *env);
extern int register_Transform(JNIEnv *env);

extern int register_BulletManager(JNIEnv *env);
extern int register_PhysicsWorld(JNIEnv *env);

extern int register_CollisionShape(JNIEnv *env);
extern int register_CompoundShape(JNIEnv *env);
extern int register_ConvexHullShape(JNIEnv *env);
extern int register_MultimaterialTriangleMeshShape(JNIEnv *env);

extern int register_Constraint(JNIEnv *env);
extern int register_HingeConstraint(JNIEnv *env);
extern int register_ConeTwistConstraint(JNIEnv *env);
extern int register_GearConstraint(JNIEnv *env);
extern int register_Generic6DofConstraint(JNIEnv *env);
extern int register_Generic6DofSpringConstraint(JNIEnv *env);
extern int register_Point2PointConstraint(JNIEnv *env);

extern int register_RigidBody(JNIEnv *env);

extern int register_SoftBody(JNIEnv *env);
extern int register_MotorControl(JNIEnv *env);
extern int register_SteerControl(JNIEnv *env);
extern int register_SoftBodyMaterial(JNIEnv *env);
extern int register_VehicleBody(JNIEnv *env);

extern int register_GLBulletWorldSurfaceView(JNIEnv *env);

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
#if LOCAL_DEBUG
    LOGD("JNI_OnLoad");
#endif
    // register native methods
    register_Vector3(env);
    register_Quaternion(env);
    register_Matrix3x3(env);
    register_Transform(env);

    register_BulletManager(env);
    register_PhysicsWorld(env);

    register_CollisionShape(env);
    register_CompoundShape(env);
    register_ConvexHullShape(env);
    register_MultimaterialTriangleMeshShape(env);

    register_Constraint(env);
    register_HingeConstraint(env);
    register_ConeTwistConstraint(env);
    register_GearConstraint(env);
    register_Generic6DofConstraint(env);
    register_Generic6DofSpringConstraint(env);
    register_Point2PointConstraint(env);

    register_RigidBody(env);

    register_SoftBody(env);
    register_MotorControl(env);
    register_SteerControl(env);
    register_SoftBodyMaterial(env);
    register_VehicleBody(env);

    register_GLBulletWorldSurfaceView(env);

    return JNI_VERSION_1_6;
}

