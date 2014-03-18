/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MeshHelperObjLoader.java
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


package com.serenegiant.androbulletglue.mesh;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import com.serenegiant.androbulletglue.MotionState;
import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.interfaces.SoftBody;
import com.serenegiant.androbulletglue.math.Quaternion;
import com.serenegiant.androbulletglue.math.Transform;
import com.serenegiant.androbulletglue.math.Vector3;
import com.serenegiant.androbulletglue.shape.BvhTriangleMeshShape;
import com.serenegiant.androbulletglue.softbody.SoftBodyConfig;
import com.serenegiant.androbulletglue.softbody.SoftBodyMaterial;
import com.serenegiant.androbulletglue.softbody.TrimeshSoftBody;

import android.content.Context;

/**
 * load OBJ formatted file(Wavefront) from assets folder and create softbody/rigidbody
 */
public final class MeshHelperObjLoader {
	
	private final float[] vertexArray;
	private final float[] normalArray;
	private final float[] uvArray;
	
	private int numVertex = 0;
	private int numNormal = 0;
	private int numUV = 0;
	private int numFace = 0;
	
	private final int[] faceIndices;
	private final int[] faceNormals;
	private final int[] faceUV;
	
	/**
	 * constructor : load OBJ formatted file from assets folder
	 * @param context
	 * @param fileName
	 */
	public MeshHelperObjLoader(Context context, String fileName) {
		InputStream in = null;
		try {
			in = new BufferedInputStream(context.getAssets().open(fileName));
			final List<String> lines = readLines(in);
			final int n = lines.size();
			vertexArray = new float[n * 3];
			normalArray = new float[n * 3];
			uvArray = new float[n * 2];
			
			faceIndices = new int[n * 3];
			faceNormals = new int[n * 3];
			faceUV = new int[n * 3];
			
			// index of loading objects
			int vertexIndex = 0;
			int normalIndex = 0;
			int uvIndex = 0;
			int faceIndex = 0;
			
			// loading...
			for (int i = 0; i < n; i++) {
				final String line = lines.get(i);
				if (line.startsWith("v ")) {		// verteices info
					final String[] tokens = line.split("[ ]+");
					vertexArray[vertexIndex] = Float.parseFloat(tokens[1]);
					vertexArray[vertexIndex + 1] = Float.parseFloat(tokens[2]);
					vertexArray[vertexIndex + 2] = Float.parseFloat(tokens[3]);
					vertexIndex += 3;
					numVertex++;
					continue;
				} if (line.startsWith("vn ")) {		// normal vector info
					final String[] tokens = line.split("[ ]+");
					normalArray[normalIndex] = Float.parseFloat(tokens[1]);
					normalArray[normalIndex + 1] = Float.parseFloat(tokens[2]);
					normalArray[normalIndex + 2] = Float.parseFloat(tokens[3]);
					normalIndex += 3;
					numNormal++;
					continue;
				} if (line.startsWith("vt ")) {		// texture coord info
					final String[] tokens = line.split("[ ]+");
					uvArray[uvIndex] = Float.parseFloat(tokens[1]);
					uvArray[uvIndex + 1] = Float.parseFloat(tokens[2]);
					uvIndex += 2;
					numUV++;
					continue;
				} if (line.startsWith("f ")) {		// triangle face info.
					final String[] tokens = line.split("[ ]+");
					// first verteices index
					String[] parts = tokens[1].split("/");
					faceIndices[faceIndex] = getIndex(parts[0], numVertex);			// vertices coordinate index
	
					if (parts.length > 2) {
						faceNormals[faceIndex] = getIndex(parts[2], numNormal);		// normal vector no.
					}
					if (parts.length > 1)
						faceUV[faceIndex] = getIndex(parts[1], numUV);				// texture coordinate no.
					faceIndex++;
					// second verteices index
					parts = tokens[2].split("/");
					faceIndices[faceIndex] = getIndex(parts[0], numVertex);			// vertex no.
	
					if (parts.length > 2)
						faceNormals[faceIndex] = getIndex(parts[2], numNormal);		// normal vector no.
					if (parts.length > 1)
						faceUV[faceIndex] = getIndex(parts[1], numUV);				// texture coordinate no.
					faceIndex++;
					// third verteices index
					parts = tokens[3].split("/");
					faceIndices[faceIndex] = getIndex(parts[0], numVertex);			// vertex coordinate no.
	
					if (parts.length > 2)
						faceNormals[faceIndex] = getIndex(parts[2], numNormal);		// normal vector no.
					if (parts.length > 1)
						faceUV[faceIndex] = getIndex(parts[1], numUV);				// texture coordinate no.
					faceIndex++;
					
					numFace++;
					continue;
				}
			}
		} catch (Exception e) {
			throw new RuntimeException("couldn't load '" + fileName + "'");
		} finally {
			if (in != null)
			try {
				in.close();
			} catch (Exception e) {
			}
		}
	}

	private static int getIndex(String index, int size) {
		if ((index != null) && (!index.equals(""))) {
			int idx = Integer.parseInt(index);
			if (idx < 0)
				return size + idx;
			else
				return idx - 1;
		} else
			return 0;
	}

	private static List<String> readLines(InputStream in) throws IOException {
		final List<String> lines = new ArrayList<String>();
		
		final BufferedReader reader = new BufferedReader(new InputStreamReader(in));
		String line = null;
		while ((line = reader.readLine()) != null) {
			lines.add(line);
		}
		return lines;
	}

	/**
	 * create softbody from loading OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @return softbody
	 */
	protected SoftBody createSoftBody(PhysicsWorld world) {
		return new TrimeshSoftBody(world, vertexArray, faceIndices, numFace, true);
	}
	
	/**
	 * create softbody from loading OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @return softbody
	 */
	public SoftBody createSoftBodyWithConfig(PhysicsWorld world) {
		final SoftBody torus = createSoftBody(world);

		final SoftBodyMaterial material = torus.appendMaterial();
		material.m_kLST = 1;
		material.m_flags -=	SoftBody.MATERIAL_FLAG_DEBUGDRAW;			
		torus.generateBendingConstraints(2, material);
		
		final SoftBodyConfig config = torus.getConfig();
		config.piterations = 2;
		config.kDF = 1;
		config.collisions = SoftBody.CONFIG_COLLISIONS_CL_SS | SoftBody.CONFIG_COLLISIONS_CL_RS;
		torus.setConfig(config);
		
		torus.randomizeConstraints();

		return torus;
	}
	
	/**
	 * create softbody from loaded OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @param mass:	0 means static object
	 * @param origin:	initial position
	 * @param angle:	initial rotation
	 * @param scale:	scaling factor
	 * @return softbody
	 */
	public SoftBody createSoftBodyWidthConfig(PhysicsWorld world,
		float mass, Vector3 origin, Vector3 angle, Vector3 scale) {
		
		final SoftBody torus = createSoftBodyWithConfig(world);
		torus.scale(scale);
		torus.rotate(angle);
		torus.translate(origin);
		torus.setTotalMass(mass, true);
		
		return torus;
	}

	/**
	 * create rigidbody from loaded OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @param mass:	0 means static object
	 * @param origin:	initial position
	 * @return rigidbody
	 */
	public RigidBody createRigidBody(PhysicsWorld world,
		float mass, Vector3 origin) {
		
		return createRigidBody(world, mass, new Transform(origin), null);
	}

	/**
	 * create rigidbody from loaded OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @param mass:	0 means static object
	 * @param origin:	initial position
	 * @param angle:		initial rotation
	 * @return rigidbody
	 */
	public RigidBody createRigidBody(PhysicsWorld world,
		float mass, Vector3 origin, Vector3 angle) {
		
		final Quaternion rot = new Quaternion();
		rot.setEulerZYX(angle);
		return createRigidBody(world, mass, new Transform(origin, rot), null);
	}

	/**
	 * create rigidbody from loaded OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @param mass:	0 means static object
	 * @param origin:	initial position
	 * @param angle:		initial rotation
	 * @param scale:		scaling factor
	 * @return rigidbody
	 */
	public RigidBody createRigidBody(PhysicsWorld world,
		float mass, Vector3 origin, Vector3 angle, Vector3 scale) {
		
		final Quaternion rot = new Quaternion();
		rot.setEulerZYX(angle);
		return createRigidBody(world, mass, new Transform(origin, rot), scale);
	}

	/**
	 * create rigidbody from loaded OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @param mass:	0 means static object
	 * @param origin:	initial position
	 * @param rotation:	initial rotation
	 * @return rigidbody
	 */
	public RigidBody createRigidBody(PhysicsWorld world,
		float mass, Vector3 origin, Quaternion rotation) {
		
		return createRigidBody(world, mass, new Transform(origin, rotation), null);
	}

	/**
	 * create rigidbody from loaded OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @param mass:	0 means static object
	 * @param origin:	initial position
	 * @param rotation:	initial rotation
	 * @param scale:	scaling factor
	 * @return rigidbody
	 */
	public RigidBody createRigidBody(PhysicsWorld world,
		float mass, Vector3 origin, Quaternion rotation, Vector3 scale) {
		
		return createRigidBody(world, mass, new Transform(origin, rotation), scale);
	}

	/**
	 * create rigidbody from loaded OBJ file</br>
	 * note: normal vector and texture coord are ignored now.
	 * @param mass:	0 means static object
	 * @param trans:	position and rotation
	 * @param scale:	scaling factor
	 * @return
	 */
	public RigidBody createRigidBody(PhysicsWorld world,
		float mass, Transform trans, Vector3 scale) {
		
    	final TriangleIndexVertexArray indexVertexArray = new TriangleIndexVertexArray(
    			numFace, faceIndices, vertexArray.length / 3, vertexArray);
    	final BvhTriangleMeshShape shape = new BvhTriangleMeshShape(world, indexVertexArray, true);
    	
    	if (scale != null) {
    		shape.setLocalScaling(scale);
    	}
        // create rigidbody
    	return new RigidBody(world,
    		shape,
        	mass,
        	new MotionState(trans)
        );
	}
	
}
