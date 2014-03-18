/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: shape.cpp
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

#include "shape.h"

#include "btHeightfieldTerrainShape.h"
#include "btMultimaterialTriangleMeshShape.h"
#include "btTriangleShape.h"
#include "btBox2dShape.h"

#include "variables.h"

#define LOCAL_DEBUG 0

//**********************************************************************
//
//**********************************************************************
void clearCollisionShape() {
	// delete Collisions.
	for (int i = 0; i < gCollisionShapes.size(); i++ ) {
		btCollisionShape *shape = gCollisionShapes[i];
		gCollisionShapes[i] = 0;
		SAFE_DELETE(shape);
	}
	gCollisionShapes.clear();
}

//**********************************************************************
//
//**********************************************************************
static btCollisionShape *createTerrainShape(JNIEnv *env, jobject shape_obj) {
	// shape_objはHeightfieldTerrainShape

#if LOCAL_DEBUG
	LOGV("createTerrainShape:");
	LOGV("createTerrainShape:load parameters from Java object");
#endif
	jint heightStickWidth = getField_int(env, shape_obj, "mHeightStickWidth");
	jint heightStickLength = getField_int(env, shape_obj, "mHeightStickLength");
	jfloat maxHeight = getField_float(env, shape_obj, "mMaxHeight");
	jint upAxis = getField_int(env, shape_obj, "mUpAxis");
	jboolean useFloatData = getField_bool(env, shape_obj, "mUseFloatData");
	jboolean flipQuadEdges = getField_bool(env, shape_obj, "mFlipQuadEdges");
	jboolean useDiamondSubdivision = getField_bool(env, shape_obj, "mUseDiamondSubdivision");

	// create native object and save its pointer value into Java object
	TerrainShapeRec *rec = new TerrainShapeRec();
	setField_nativePtr(env, shape_obj, "mNativePtr", rec);
	// get mHeightfieldData array as a Object
	jobject heightfieldDataArray_obj = getField_obj(env, shape_obj, "mHeightfieldData", "[B");
	// convert it to jbyteArray
#if LOCAL_DEBUG
	LOGV("createTerrainShape:reinterpret_cast:heightfieldData");
#endif
	jbyteArray *heightfieldDataArray = reinterpret_cast<jbyteArray *>( &heightfieldDataArray_obj );
	// copy jbyte array into native array
	jsize datalen = env->GetArrayLength(*heightfieldDataArray);	// indexlen == numTriangles * 3のはず
	rec->heightfieldData = new unsigned char[datalen];
	env->GetByteArrayRegion(*heightfieldDataArray, 0, datalen, (jbyte *)rec->heightfieldData);

#if LOCAL_DEBUG
	LOGV("createTerrainShape: create btHeightfieldTerrainShape");
#endif
	btHeightfieldTerrainShape *heightFieldShape = new btHeightfieldTerrainShape(
		heightStickWidth, heightStickLength, rec->heightfieldData, maxHeight,
		upAxis, useFloatData, flipQuadEdges);
	heightFieldShape->setUseDiamondSubdivision(useDiamondSubdivision);

	// release jbyte array
//	env->ReleaseByteArrayElements(*heightfieldDataArray, heightfieldData, ARRAYELEMENTS_ABORT_AND_RELEASE);
	env->DeleteLocalRef(heightfieldDataArray_obj);
	return heightFieldShape;
}

static btCollisionShape *createMultiSphereShape(JNIEnv *env, jobject shape_obj) {
	// shape_objはMultiSphereShape

#if LOCAL_DEBUG
	LOGV("createMultiSphereShape:");
#endif
	btMultiSphereShape *collisionShape = NULL;
	int numShperes = getField_int(env, shape_obj, "mNumSpheres");
	if (LIKELY(numShperes)) {
		// create native object and save its pointer value into Java object
		MultiSphereShapeRec *rec = new MultiSphereShapeRec();
		rec->positions = new btVector3[numShperes];
		rec->radii = new btScalar[numShperes];
		setField_nativePtr(env, shape_obj, "mNativePtr", rec);

		jobject positionArray_obj = getField_obj(env, shape_obj, "mRadius", "[F");
		jfloatArray *positionArray = reinterpret_cast<jfloatArray *>(&positionArray_obj);
		jfloat *position = env->GetFloatArrayElements(*positionArray, NULL);

		jobject radiusArray_obj = getField_obj(env, shape_obj, "mRadius", "[F");
		jfloatArray *radiusArray = reinterpret_cast<jfloatArray *>(&radiusArray_obj);
//		jfloat *radius = env->GetFloatArrayElements(*radiusArray, NULL);
		int radiuslen = env->GetArrayLength(*radiusArray);
		env->GetFloatArrayRegion(*radiusArray, 0, radiuslen, rec->radii);

		// copy parameters
		for (int i = 0; i < numShperes; i++) {
			rec->positions->setValue(position[i * 3], position[i * 3 + 1], position[i * 3 + 2]);
		}
		collisionShape = new btMultiSphereShape(
			rec->positions, rec->radii, numShperes);

		// release the reference of jfloat array
		env->ReleaseFloatArrayElements(*positionArray, position, ARRAYELEMENTS_ABORT_AND_RELEASE);
		env->DeleteLocalRef(positionArray_obj);
		// release jfloat array
//		env->ReleaseFloatArrayElements(*radiusArray, radius, ARRAYELEMENTS_ABORT_AND_RELEASE);
		env->DeleteLocalRef(radiusArray_obj);
	}

	return collisionShape;
}



static btTriangleIndexVertexArray *prepareTriangleMeshShape(JNIEnv *env, jobject shape_obj) {
	// shape_objはBvhTriangleMeshShape

#if LOCAL_DEBUG
	LOGV("prepareTriangleMeshShape:");
#endif
	int vertStride = sizeof(btVector3);
	int indexStride = 3 * sizeof(int);

	btTriangleIndexVertexArray *indexVertexArrays = NULL;
	int indexedMeshNum = getField_int(env, shape_obj, "mIndexedMeshNum");
	if (LIKELY(indexedMeshNum)) {

		// get IndexedMesh as a Object
#if LOCAL_DEBUG
		LOGV("prepareTriangleMeshShape:load mIndexedMeshes(indexedMeshNum=%d)", indexedMeshNum);
#endif
		jobject indexedmesharray_obj = getField_obj(env, shape_obj, "mIndexedMeshes", JTYPE_INDEXEDMESHARRAY);
		if (UNLIKELY(!indexedmesharray_obj)) {
			LOGE("prepareTriangleMeshShape:BvhTriangleMeshShape has no mIndexedMeshes field");
			return NULL;
		}
		// convert it to jobjectArray
#if LOCAL_DEBUG
		LOGV("prepareTriangleMeshShape:reinterpret_cast:indexedmesharray");
#endif
		jobjectArray *indexedmesharray = reinterpret_cast<jobjectArray *>( &indexedmesharray_obj );

		// create native object and save its pointer value into Java object
		TriangleMeshShapeRec *rec = new TriangleMeshShapeRec();
		rec->indexedMeshNum = indexedMeshNum;
		rec->indexBaseArray = new jint*[indexedMeshNum];
		rec->vertexBaseArray = new btVector3*[indexedMeshNum];
		setField_nativePtr(env, shape_obj, "mNativePtr", rec);
		for (int i = 0; i < indexedMeshNum; i++) {
			rec->indexBaseArray[i] = NULL;
			rec->vertexBaseArray[i] = NULL;
#if LOCAL_DEBUG
			LOGV("prepareTriangleMeshShape:get indexedmesharray");
#endif
			// get array elements(IndexedMesh)
			jobject indexedmesh_obj = env->GetObjectArrayElement(*indexedmesharray, i);
			if (UNLIKELY(!indexedmesh_obj)) {
				LOGE("prepareTriangleMeshShape:GetObjectArrayElements returned NULL");
				continue;
			}
			//----------------------------------------------------------------------
			int numTriangles = getField_int(env, indexedmesh_obj, "mNumTriangles");
			int numVertices = getField_int(env, indexedmesh_obj, "mNumVertices");
#if LOCAL_DEBUG
			LOGV("prepareTriangleMeshShape:numTriangles=%d,numVertices=%d", numTriangles, numVertices);
#endif
			// get mTriangleIndexBase as a Object
#if LOCAL_DEBUG
			LOGV("createTriangleMeshShape:get mTriangleIndexBase");
#endif
			jobject triangleindexbase_obj = getField_obj(env, indexedmesh_obj, "mTriangleIndexBase", "[I");
			if (UNLIKELY(!triangleindexbase_obj)) {
				LOGE("prepareTriangleMeshShape:IndexedMesh has no mTriangleIndexBase field");
				continue;
			}
			// convert it to jintArray
			jintArray *triangleindexbase = reinterpret_cast<jintArray *>( &triangleindexbase_obj );
			// copy jint array into native array
			jsize indexlen = env->GetArrayLength(*triangleindexbase);	// indexlen == numTriangles * 3のはず
			rec->indexBaseArray[i] = new int[indexlen];
			env->GetIntArrayRegion(*triangleindexbase, 0, indexlen, rec->indexBaseArray[i]);
#if LOCAL_DEBUG
			LOGV("prepareTriangleMeshShape:copy indexBaseArray(indexlen=%d)", indexlen);
#endif

			// get mVertexBase as a Object
#if LOCAL_DEBUG
			LOGV("prepareTriangleMeshShape:get mVertexBase");
#endif
			jobject verticesbase_obj = getField_obj(env, indexedmesh_obj, "mVertexBase", "[F");
			if (!verticesbase_obj) {
				LOGE("prepareTriangleMeshShape:IndexedMesh has no mVertexBase field");
				SAFE_DELETE_ARRAY(rec->indexBaseArray[i]);
				env->DeleteLocalRef(triangleindexbase_obj);
				continue;
			}
			// convert it to jFloatArray
			jfloatArray *verticesbase = reinterpret_cast<jfloatArray *>( &verticesbase_obj );
			// get as jfloat
			jfloat *vertices = env->GetFloatArrayElements(*verticesbase, NULL);
			// create btVector3 array(native array) from jfloat array
			jsize vertexlen = env->GetArrayLength(*verticesbase) / 3;	// vertexLen == numVerticesのはず
			rec->vertexBaseArray[i] = new btVector3[vertexlen];
			for (int j = 0; j < vertexlen; j++) {
				rec->vertexBaseArray[i][j].setValue(
					vertices[j * 3],
					vertices[j * 3 + 1],
					vertices[j * 3 + 2]);
			}
#if LOCAL_DEBUG
			LOGV("prepareTriangleMeshShape:copy & create vertexBaseArray(vertexlen=%d)", vertexlen);
#endif

			if (!indexVertexArrays) {
				// btTriangleIndexVertexArray can accept vertexBase as a pointer to btScalar.
				// but btTriangleIndexVertexArray internally treats vertexBase as a array of btVector3,
				// therefore we can't pass array of btScalar itself.
				// we must cast address of x filed of the top elemnt of btVector3 array as a pointer of btScalar
				// and pass it as a vertexBase.
				indexVertexArrays = new btTriangleIndexVertexArray(
					numTriangles, rec->indexBaseArray[i], indexStride,
					numVertices, (btScalar*) &rec->vertexBaseArray[i]->x(), vertStride);
			} else {
				btIndexedMesh mesh;
				mesh.m_numTriangles = numTriangles;
				mesh.m_triangleIndexBase = (const unsigned char *)rec->indexBaseArray[i];	// triangleindex;
				mesh.m_triangleIndexStride = indexStride;
				mesh.m_numVertices = numVertices;
				mesh.m_vertexBase = (const unsigned char *)&rec->vertexBaseArray[i]->x();	// vertices;
				mesh.m_vertexStride = vertStride;
				indexVertexArrays->addIndexedMesh(mesh);
			}
			env->DeleteLocalRef(triangleindexbase_obj);
			// release jfloat array
			env->ReleaseFloatArrayElements(*verticesbase, vertices, ARRAYELEMENTS_COPYBACK_AND_RELEASE);	// float[]
			env->DeleteLocalRef(verticesbase_obj);
			//----------------------------------------------------------------------
			env->DeleteLocalRef(indexedmesh_obj);	// release reference to IndexedMesh
		}
		bool hasAabb = getField_bool(env, shape_obj, "mHasAabb");
		if (hasAabb) {
			indexVertexArrays->setPremadeAabb(
				*getField_vec(env, shape_obj, "mAabbMin"),
				*getField_vec(env, shape_obj, "mAabbMax")
			);
		}
		env->DeleteLocalRef(indexedmesharray_obj);	// release reference to IndexedMesh array

		return indexVertexArrays;
	} else {
		LOGE("prepareTriangleMeshShape:BvhTriangleMeshShape has no IndexedMeshe");
		return NULL;
	}
}

static btCollisionShape *createTriangleMeshShape(JNIEnv *env, jobject shape_obj) {
	// shape_objはBvhTriangleMeshShape

#if LOCAL_DEBUG
	LOGV("createTriangleMeshShape:");
#endif
	bool useQuantizedAabbCompression = getField_bool(env, shape_obj, "mUseQuantizedAabbCompression");
	btTriangleIndexVertexArray *indexVertexArrays = prepareTriangleMeshShape(env, shape_obj);
	if (LIKELY(indexVertexArrays)) {
		return new btBvhTriangleMeshShape(indexVertexArrays, useQuantizedAabbCompression);
	} else {
		LOGE("createTriangleMeshShape:BvhTriangleMeshShape has no IndexedMeshe");
		return NULL;
	}
}

static btCollisionShape *createMultimaterialTriangleMeshShape(JNIEnv *env, jobject shape_obj) {
	// shape_objはBvhTriangleMeshShape

#if LOCAL_DEBUG
	LOGV("createMultimaterialTriangleMeshShape:");
#endif
	bool useQuantizedAabbCompression = getField_bool(env, shape_obj, "mUseQuantizedAabbCompression");
	btTriangleIndexVertexArray *indexVertexArrays = prepareTriangleMeshShape(env, shape_obj);
	if (LIKELY(indexVertexArrays)) {
		return new btMultimaterialTriangleMeshShape(indexVertexArrays, useQuantizedAabbCompression);
	} else {
		LOGE("createMultimaterialTriangleMeshShape:MultimaterialTriangleMeshShape has no IndexedMeshe");
		return NULL;
	}
}

//**********************************************************************
//
//**********************************************************************
static ID_SHAPE nativeCreate(JNIEnv *env, jobject thiz) {

	btCollisionShape *collisionShape = NULL;

#if LOCAL_DEBUG
	LOGV("createCollisionShape:");
#endif
	int shapeType = getField_int(env, thiz, "mType");
#if LOCAL_DEBUG
	LOGV("createCollisionShape:shapeType=%d", shapeType);
#endif
	switch (shapeType) {
	// polyhedral convex shapes
		case BOX_SHAPE_PROXYTYPE:					// 0
#if LOCAL_DEBUG
			LOGV("createCollisionShape:BOX_SHAPE_PROXYTYPE");
#endif
			collisionShape = new btBoxShape(
				*getField_vec(env, thiz, "boxHalfExtents"));
			break;

		case TRIANGLE_SHAPE_PROXYTYPE:				// 1
#if LOCAL_DEBUG
			LOGV("createCollisionShape:TRIANGLE_SHAPE_PROXYTYPE");
#endif
			collisionShape = new btTriangleShape(
				*getField_vec(env, thiz, "p0"),
				*getField_vec(env, thiz, "p1"),
				*getField_vec(env, thiz, "p2"));
			break;

		case TETRAHEDRAL_SHAPE_PROXYTYPE:			// 2
#if LOCAL_DEBUG
			LOGV("createCollisionShape:TETRAHEDRAL_SHAPE_PROXYTYPE");
#endif
			collisionShape = new btBU_Simplex1to4(
				*getField_vec(env, thiz, "p0"),
				*getField_vec(env, thiz, "p1"),
				*getField_vec(env, thiz, "p2"),
				*getField_vec(env, thiz, "p3"));
			break;

		case CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE:	// 3
		case CONVEX_HULL_SHAPE_PROXYTYPE:			// 4
		case CONVEX_POINT_CLOUD_SHAPE_PROXYTYPE:	// 5
		case CUSTOM_POLYHEDRAL_SHAPE_TYPE:			// 6
			break;
	// implicit convex shapes
		case SPHERE_SHAPE_PROXYTYPE:				// 8
#if LOCAL_DEBUG
			LOGV("createCollisionShape:SPHERE_SHAPE_PROXYTYPE");
#endif
			collisionShape = new btSphereShape(
				getField_float(env, thiz, "radius"));
			break;

		case MULTI_SPHERE_SHAPE_PROXYTYPE:			// 9
			collisionShape = createMultiSphereShape(env, thiz);
			break;
		case CAPSULE_SHAPE_PROXYTYPE:				// 10
		{
#if LOCAL_DEBUG
			LOGV("createCollisionShape:CAPSULE_SHAPE_PROXYTYPE");
#endif
			btScalar radius = getField_float(env, thiz, "radius");
			btScalar height = getField_float(env, thiz, "height");
			switch (getField_int(env, thiz, "upAxis")) {
			case 0:
				collisionShape = new btCapsuleShapeX(radius, height);
				break;
			case 2:
				collisionShape = new btCapsuleShapeZ(radius, height);
				break;
			default:
				collisionShape = new btCapsuleShape(radius, height);
				break;
			}
			break;
		}
		case CONE_SHAPE_PROXYTYPE:					// 11
		{
#if LOCAL_DEBUG
			LOGV("createCollisionShape:CONE_SHAPE_PROXYTYPE");
#endif
			btScalar radius = getField_float(env, thiz, "radius");
			btScalar height = getField_float(env, thiz, "height");
			switch (getField_int(env, thiz, "upAxis")) {
			case 0:
				collisionShape = new btConeShapeX(radius, height);
				break;
			case 1:
				collisionShape = new btConeShape(radius, height);
				break;
			case 2:
				collisionShape = new btConeShapeZ(radius, height);
				break;
			}
			break;
		}
		case CONVEX_SHAPE_PROXYTYPE:				// 12
			break;

		case CYLINDER_SHAPE_PROXYTYPE:				// 13
		{
#if LOCAL_DEBUG
			LOGV("createCollisionShape:CYLINDER_SHAPE_PROXYTYPE");
#endif
			btVector3 *halfExtents = getField_vec(env, thiz, "halfExtents");
			switch (getField_int(env, thiz, "upAxis")) {
			case 0:
				collisionShape = new btCylinderShapeX(*halfExtents);
				break;
			case 1:
				collisionShape = new btCylinderShape(*halfExtents);
				break;
			case 2:
				collisionShape = new btCylinderShapeZ(*halfExtents);
				break;
			}
			break;
		}
		case UNIFORM_SCALING_SHAPE_PROXYTYPE:		// 14
#if LOCAL_DEBUG
			LOGV("createCollisionShape:UNIFORM_SCALING_SHAPE_PROXYTYPE");
#endif
			collisionShape = new btUniformScalingShape(
				reinterpret_cast<btConvexShape *>(getField_NativeObj(env, thiz, "mChildConvexShape", JTYPE_CONVEXSHAPE)),
				getField_float(env, thiz, "mUniformScalingFactor")
			);
			break;
		case MINKOWSKI_SUM_SHAPE_PROXYTYPE:			// 15
		case MINKOWSKI_DIFFERENCE_SHAPE_PROXYTYPE:	// 16
			break;
		case BOX_2D_SHAPE_PROXYTYPE:				// 17
#if LOCAL_DEBUG
			LOGV("createCollisionShape:BOX_2D_SHAPE_PROXYTYPE");
#endif
			collisionShape = new btBox2dShape(
				*getField_vec(env, thiz, "boxHalfExtents"));
			break;
		case CONVEX_2D_SHAPE_PROXYTYPE:				// 18
		case CUSTOM_CONVEX_SHAPE_TYPE:				// 19
			break;

		//concave shapes
		case TRIANGLE_MESH_SHAPE_PROXYTYPE:			// 21
#if LOCAL_DEBUG
			LOGV("createCollisionShape:TRIANGLE_MESH_SHAPE_PROXYTYPE");
#endif
			collisionShape = createTriangleMeshShape(env, thiz);
			break;
		case SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE:	// 22
#if LOCAL_DEBUG
			LOGV("createCollisionShape:SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE");
#endif
			collisionShape = new btScaledBvhTriangleMeshShape(
				reinterpret_cast<btBvhTriangleMeshShape *>(getField_NativeObj(env, thiz, "mChildShape", JTYPE_BVHTRIANGLEMESHSHAPE)),
				*getField_vec(env, thiz, "mLocalScaling")
			);
			break;
			// used for demo integration FAST/Swift collision library and Bullet
		case FAST_CONCAVE_MESH_PROXYTYPE:			// 23
			// terrain
		case TERRAIN_SHAPE_PROXYTYPE:				// 24
			collisionShape = createTerrainShape(env, thiz);
			break;
			// Used for GIMPACT Trimesh integration
		case GIMPACT_SHAPE_PROXYTYPE:				// 25
			// Multimaterial mesh
		case MULTIMATERIAL_TRIANGLE_MESH_PROXYTYPE:	// 26
#if LOCAL_DEBUG
			LOGV("createCollisionShape:MULTIMATERIAL_TRIANGLE_MESH_PROXYTYPE");
#endif
			collisionShape = createMultimaterialTriangleMeshShape(env, thiz);
			break;

		case EMPTY_SHAPE_PROXYTYPE:					// 27
#if LOCAL_DEBUG
			LOGV("createCollisionShape:EMPTY_SHAPE_PROXYTYPE");
#endif
			collisionShape = new btEmptyShape();
			break;

		case STATIC_PLANE_PROXYTYPE:				// 28
#if LOCAL_DEBUG
			LOGV("createCollisionShape:STATIC_PLANE_PROXYTYPE");
#endif
			collisionShape = new btStaticPlaneShape(
				*getField_vec(env, thiz, "mPlaneNormal"),
				getField_float(env, thiz, "mPlaneConstant"));
			break;

		case CUSTOM_CONCAVE_SHAPE_TYPE:				// 29
			break;
		case COMPOUND_SHAPE_PROXYTYPE:				// 31
			collisionShape = new btCompoundShape();
			break;
		case SOFTBODY_SHAPE_PROXYTYPE:				// 32
		case HFFLUID_SHAPE_PROXYTYPE:				// 33
		case HFFLUID_BUOYANT_CONVEX_SHAPE_PROXYTYPE:// 34
			break;
		default:
			break;
	}

	if (LIKELY(collisionShape)) {
		gCollisionShapes.push_back(collisionShape);
	} else {
		LOGE("createCollisionShape:shapeType not found:type=%d", shapeType);
	}

	// return pointer value of object as a ID to Java instance
	return reinterpret_cast<ID_SHAPE>(collisionShape);
}

static void nativeDestroy(JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape) {

#if LOCAL_DEBUG
	LOGD("CollisionShape#nativeDestroy:");
#endif

	btCollisionShape *shape = reinterpret_cast<btCollisionShape *>(id_shape);
	// 追加処理
	int shapeType = getField_int(env, thiz, "mType");
	switch (shapeType) {
	case MULTI_SPHERE_SHAPE_PROXYTYPE:			// 9
	{
		MultiSphereShapeRec *multiSphereShapeRec
		 	= reinterpret_cast<MultiSphereShapeRec *>(getField_nativePtr(env, thiz, "mNativePtr"));
		if (multiSphereShapeRec) {
			SAFE_DELETE_ARRAY(multiSphereShapeRec->positions);
			SAFE_DELETE_ARRAY(multiSphereShapeRec->radii);
			SAFE_DELETE(multiSphereShapeRec);
			setField_nativePtr(env, thiz, "mNativePtr", NULL);
		}
		break;
	}
	case TRIANGLE_MESH_SHAPE_PROXYTYPE:			// 21
	{
		TriangleMeshShapeRec *triangleMeshShapeRec
			= reinterpret_cast<TriangleMeshShapeRec *>(getField_nativePtr(env, thiz, "mNativePtr"));
		if (triangleMeshShapeRec) {
			if (triangleMeshShapeRec->indexedMeshNum) {
				if (triangleMeshShapeRec->indexBaseArray) {
					for (int i = 0; i < triangleMeshShapeRec->indexedMeshNum; i++) {
						SAFE_DELETE_ARRAY(triangleMeshShapeRec->indexBaseArray[i]);
					}
				}
				SAFE_DELETE_ARRAY(triangleMeshShapeRec->indexBaseArray);

				if (triangleMeshShapeRec->vertexBaseArray) {
					for (int i = 0; i < triangleMeshShapeRec->indexedMeshNum; i++) {
						SAFE_DELETE_ARRAY(triangleMeshShapeRec->vertexBaseArray[i]);
					}
				}
				SAFE_DELETE_ARRAY(triangleMeshShapeRec->vertexBaseArray);
			}
			SAFE_DELETE(triangleMeshShapeRec);
			setField_nativePtr(env, thiz, "mNativePtr", NULL);
		}
		break;
	}
	case TERRAIN_SHAPE_PROXYTYPE:			// 24
		TerrainShapeRec *terrainShapeRec
			= reinterpret_cast<TerrainShapeRec *>(getField_nativePtr(env, thiz, "mNativePtr"));
		if (terrainShapeRec) {
			SAFE_DELETE_ARRAY(terrainShapeRec->heightfieldData);
			SAFE_DELETE(terrainShapeRec);
			setField_nativePtr(env, thiz, "mNativePtr", NULL);
		}
		break;
	}
	// CollisionShapeの破棄
	gCollisionShapes.remove(shape);
	SAFE_DELETE(shape);
}

static jint nativeSetLocalScaling(JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape,
		ID_VEC id_vec) {

#if LOCAL_DEBUG
	LOGV("CollisionShape#nativeSetLocalScaling:");
#endif
	btCollisionShape *shape = reinterpret_cast<btCollisionShape *>(id_shape);
	btVector3 *scaling = reinterpret_cast<btVector3 *>(id_vec);
	if (LIKELY(shape && scaling)) {
		shape->setLocalScaling(*scaling);
		return JNI_OK;
	} else {
		LOGE("CollisionShape#nativeSetLocalScaling:invalid id of CollisionShape/Vector3");
		return -1000;
	}
}

static jint nativeGetLocalScaling(JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape,
		ID_VEC id_vec_result) {

#if LOCAL_DEBUG
	LOGV("CollisionShape#nativeGetLocalScaling:");
#endif
	btCollisionShape *shape = reinterpret_cast<btCollisionShape *>(id_shape);
	btVector3 *vec_result = reinterpret_cast<btVector3 *>(id_vec_result);
	if (LIKELY(shape && vec_result)) {
		*vec_result = shape->getLocalScaling();
		return JNI_OK;
	} else {
		LOGE("CollisionShape#nativeGetLocalScaling:invalid id of CollisionShape/Vector3");
		return -1000;
	}
}

static jint nativeSetMargin(JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape,
		jfloat margin) {

#if LOCAL_DEBUG
	LOGV("CollisionShape#nativeSetMargin:");
#endif
	btCollisionShape *shape = reinterpret_cast<btCollisionShape *>(id_shape);
	if (LIKELY(shape)) {
		shape->setMargin(margin);
		return JNI_OK;
	} else {
		LOGE("CollisionShape#nativeSetMargin:invalid id");
		return -1000;
	}
}

static jfloat nativeGetMargin(JNIEnv *env, jobject thiz,
		ID_SHAPE id_shape) {

#if LOCAL_DEBUG
	LOGV("CollisionShape#nativeGetMargin:");
#endif
	btCollisionShape *shape = reinterpret_cast<btCollisionShape *>(id_shape);
	if (LIKELY(shape)) {
		return shape->getMargin();
	} else {
		LOGE("CollisionShape#nativeGetMargin:invalid id");
		return 0.f;
	}
}

//**********************************************************************
//
//**********************************************************************
static JNINativeMethod methods[] = {
	{"nativeCreate", "()J", (void *)nativeCreate},
	{"nativeDestroy", "(J)V", (void *)nativeDestroy},
	{"nativeSetLocalScaling", "(JJ)I", (void *)nativeSetLocalScaling},
	{"nativeGetLocalScaling", "(JJ)I", (void *)nativeGetLocalScaling},
	{"nativeSetMargin", "(JF)I", (void *)nativeSetMargin},
	{"nativeGetMargin", "(J)F", (void *)nativeGetMargin},
};

int register_CollisionShape(JNIEnv *env) {
	return registerNativeMethods(env,
		"com/serenegiant/androbulletglue/interfaces/CollisionShape",
		methods, NUM_ARRAY_ELEMENTS(methods));
}
