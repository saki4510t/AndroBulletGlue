#/*
# * AndroBullet
# * Bullet Continuous Collision Detection and Physics Library for Android
# * Copyright(c) 2013,2014 t_saki@serenegiant
# * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
#
#This software is provided 'as-is', without any express or implied warranty.
#In no event will the authors be held liable for any damages arising from the use of this software.
#Permission is granted to anyone to use this software for any purpose,
#including commercial applications, and to alter it and redistribute it freely,
#subject to the following restrictions:
#
#1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
#2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
#3. This notice may not be removed or altered from any source distribution.
#*/

#MY_LOCAL_PATH	:= $(call my-dir)
######################################################################
#libbullet.a
#original Bullet library itself (version:2.82-rev2704)
######################################################################
include $(CLEAR_VARS)
LOCAL_PATH		:= $(MY_LOCAL_PATH)/Bullet

CFLAGS := -Werror

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
#compile with arm mode if the target is armeabi-v7a (Default is thumb mode)
#the file size of thumb mode is smaller than arm mode
#but the performance with arm mode is better
#LOCAL_ARM_MODE := arm
#when you want to enable NEON (but it does not work well)
#LOCAL_CFLAGS += -DBT_USE_NEON
#LOCAL_ARM_NEON := true
else
#comment out next line if complie with arm mode. (Default is thumb mode)
#LOCAL_ARM_MODE := arm
endif # TARGET_ARCH_ABI == armeabi-v7a

#libbullet.a
LOCAL_MODULE    := bullet
LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/ \
        $(LOCAL_PATH)/BulletCollision/BroadphaseCollision \
        $(LOCAL_PATH)/BulletCollision/CollisionDispatch \
        $(LOCAL_PATH)/BulletCollision/CollisionShapes \
        $(LOCAL_PATH)/BulletCollision/NarrowPhaseCollision \
        $(LOCAL_PATH)/BulletDynamics/ConstraintSolver \
        $(LOCAL_PATH)/BulletDynamics/Dynamics \
        $(LOCAL_PATH)/BulletDynamics/Vehicle \
        $(LOCAL_PATH)/BulletDynamics/Character \
        $(LOCAL_PATH)/BulletSoftBody \
        $(LOCAL_PATH)/LinearMath

LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%)
LOCAL_CFLAGS += -DANDROID_NDK 
#LOCAL_CFLAGS += -DDISABLE_IMPORTGL			# when static link OpenGL|ES library

# if you want to use exception support of C++
# don't forget to use runtime library with exception support(like GNU STL)
# (you should set APP_STL in Application.mk gnustl_static or gnustl_shared)
#LOCAL_CPP_FEATURES += exceptions 

#LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl	# to avoid NDK issue(no need for static library)
#LOCAL_LDLIBS += -lm						# math...automaticalliy linked by Android NDK
#LOCAL_LDLIBS += -lpthread					# pthread...automaticalliy linked by Android NDK
#LOCAL_LDLIBS += -lz						# zlib of Android NDK
#LOCAL_LDLIBS += -llog						# log output library
#LOCAL_LDLIBS += -lGLESv1_CM				# OpenGL|ES 1.1 library
#LOCAL_LDLIBS += -lGLESv2					# OpenGL|ES 2.0 library
#LOCAL_LDLIBS += -landroid					# Android native related library(when you use nativeActivity)
#LOCAL_LDLIBS += -OpenMAXAL					# OpenMAXAL for Android

#external modules that link dynamically
#LOCAL_SHARED_LIBRARIES :=
#external modules that link statically
#LOCAL_STATIC_LIBRARIES := android_native_app_glue	# when you want to use NativeActivityGlue

# source files that we want to compile and link into current module
LOCAL_SRC_FILES := \
		LinearMath/btQuickprof.cpp \
		LinearMath/btGeometryUtil.cpp \
		LinearMath/btAlignedAllocator.cpp \
		LinearMath/btSerializer.cpp \
		LinearMath/btConvexHull.cpp \
		LinearMath/btPolarDecomposition.cpp \
		LinearMath/btVector3.cpp \
		LinearMath/btConvexHullComputer.cpp
LOCAL_SRC_FILES += \
		BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
		BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
		BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
		BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
		BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
		BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
		BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
		BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
		BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
		BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
		BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
		BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp
LOCAL_SRC_FILES += \
		BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btCollisionObject.cpp \
		BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btGhostObject.cpp \
		BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
		BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
		BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
		BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
		BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
		BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
		BulletCollision/CollisionDispatch/btManifoldResult.cpp \
		BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
		BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btUnionFind.cpp \
		BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp \
		BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp
LOCAL_SRC_FILES += \
		BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
		BulletCollision/CollisionShapes/btShapeHull.cpp \
		BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
		BulletCollision/CollisionShapes/btCompoundShape.cpp \
		BulletCollision/CollisionShapes/btConeShape.cpp \
		BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
		BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
		BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
		BulletCollision/CollisionShapes/btSphereShape.cpp \
		BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
		BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
		BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
		BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
		BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
		BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
		BulletCollision/CollisionShapes/btEmptyShape.cpp \
		BulletCollision/CollisionShapes/btCollisionShape.cpp \
		BulletCollision/CollisionShapes/btConvexShape.cpp \
		BulletCollision/CollisionShapes/btConvex2dShape.cpp \
		BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
		BulletCollision/CollisionShapes/btConvexHullShape.cpp \
		BulletCollision/CollisionShapes/btTriangleCallback.cpp \
		BulletCollision/CollisionShapes/btCapsuleShape.cpp \
		BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
		BulletCollision/CollisionShapes/btConcaveShape.cpp \
		BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
		BulletCollision/CollisionShapes/btBoxShape.cpp \
		BulletCollision/CollisionShapes/btBox2dShape.cpp \
		BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
		BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
		BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
		BulletCollision/CollisionShapes/btCylinderShape.cpp \
		BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
		BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
		BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
		BulletCollision/CollisionShapes/btTriangleMesh.cpp
LOCAL_SRC_FILES += \
		BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
		BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
		BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
		BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
		BulletCollision/BroadphaseCollision/btDispatcher.cpp \
		BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
		BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
		BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
		BulletCollision/BroadphaseCollision/btDbvt.cpp \
		BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp
LOCAL_SRC_FILES += \
		BulletDynamics/Dynamics/btRigidBody.cpp \
		BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
		BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
#		BulletDynamics/Dynamics/Bullet-C-API.cpp
LOCAL_SRC_FILES += \
		BulletDynamics/ConstraintSolver/btGearConstraint.cpp \
		BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
		BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
		BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
		BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
		BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
		BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
		BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
		BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
		BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
		BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
		BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
		BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
LOCAL_SRC_FILES += \
		BulletDynamics/Vehicle/btWheelInfo.cpp \
		BulletDynamics/Vehicle/btRaycastVehicle.cpp \
		BulletDynamics/Character/btKinematicCharacterController.cpp
LOCAL_SRC_FILES += \
		BulletSoftBody/btDefaultSoftBodySolver.cpp \
		BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp \
		BulletSoftBody/btSoftBody.cpp \
		BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp \
		BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp \
		BulletSoftBody/btSoftRigidDynamicsWorld.cpp \
		BulletSoftBody/btSoftBodyHelpers.cpp \
		BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp \
#LOCAL_SRC_FILES += \
		BulletCollision/Gimpact/btGImpactBvh.cpp\
		BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp\
		BulletCollision/Gimpact/btTriangleShapeEx.cpp\
		BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp\
		BulletCollision/Gimpact/btGImpactShape.cpp\
		BulletCollision/Gimpact/gim_box_set.cpp\
		BulletCollision/Gimpact/gim_contact.cpp\
		BulletCollision/Gimpact/gim_memory.cpp\
		BulletCollision/Gimpact/gim_tri_collision.cpp

#link as a static library
include $(BUILD_STATIC_LIBRARY)
