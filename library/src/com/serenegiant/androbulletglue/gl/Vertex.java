/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: Vertex.java
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

package com.serenegiant.androbulletglue.gl;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;
import java.util.Arrays;

import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.opengles.GL11;

import com.serenegiant.androbulletglue.math.Vector3;

public class Vertex {
	public static final int DIM_2D = 2;				// for 2D coordinates
	public static final int DIM_3D = 3; 			// for 3D coordinates

	private static final int FLOAT_SZ = Float.SIZE / 8;	// bytes of float[=4 bytes]
	private static final int SHORT_SZ = Short.SIZE / 8;	// bytes of short[=2 bytes]
	private static final int INT_SZ = Integer.SIZE / 8;	// bytes of int[=4 bytes]
	
	private static final int COORD_NUM = 2;			// number of float for texture coordinates
	private static final int COLOR_NUM = 4;			// number of float for color
	private static final int NORMAL_NUM = 3;		// number of float for normal coordinates
	
	private final GL10 gl;
	private final boolean hasColor;					// whether or not that this instance has color information
	private final boolean hasTexCoord; 				// whether or not that this instance has texture coordinates
	private final boolean hasNormals;				// whether or not that this instance has normal coordinates
	private final int vertexSize;					// size of vertices as a bytes
	private final IntBuffer vertexArray;			// vertices array
	private final ShortBuffer indexArray;			// indices array
	private final int[] tmpBuffer;
	private final int dim_num ;
	private final int maxVertex, maxIndex;
	
	private static final int VBO_VERTEX = 0;
	private static final int VBO_INDEX = 1;
//	private static final int VBO_TEX = 2; 
//	private static final int VBO_COLOR = 3;
//	private static final int VBO_NORM = 4;
	
	private final int[] vboId = new int[2];			// VBO ID
	private boolean hasVbo = false;
	
	public Vertex(int dim_num, GL10 gl, int maxVertex, int maxIndex) {
		this(dim_num, gl, maxVertex, maxIndex, false, false, false);
	}

	public Vertex(int dim_num, GL10 gl, int maxVertex, int maxIndex,
			boolean hasColor) {
		this(dim_num, gl, maxVertex, maxIndex, hasColor, false, false);
	}

	public Vertex(int dim_num, GL10 gl, int maxVertex, int maxIndex,
			boolean hasColor, boolean hasTexCoord) {
		this(dim_num, gl, maxVertex, maxIndex, hasColor, hasTexCoord, false);
	}
	
	public Vertex(int dim_num, GL10 gl, int maxVertex, int maxIndex,
			boolean hasColor, boolean hasTexCoord, boolean hasNormals) {
		this.dim_num = dim_num;
		this.gl = gl;
		this.maxVertex = maxVertex;
		this.maxIndex = maxIndex;
		this.hasColor = hasColor;
		this.hasTexCoord = hasTexCoord;
		this.hasNormals = hasNormals;
		this.vertexSize = (dim_num
			+ (hasColor ? COLOR_NUM : 0)
			+ (hasTexCoord ? COORD_NUM : 0)
			+ (hasNormals ? NORMAL_NUM : 0) ) * FLOAT_SZ;	// [bytes]
		// preparation of vertices array
		tmpBuffer = new int[vertexSize * maxVertex / INT_SZ];	// [number of int]
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(vertexSize * maxVertex);
		byteBuffer.order(ByteOrder.nativeOrder());
		vertexArray = byteBuffer.asIntBuffer();
		// preparation of indices array
		if (maxIndex > 0) {
			byteBuffer = ByteBuffer.allocateDirect(SHORT_SZ * maxIndex);	// [bytes]
			byteBuffer.order(ByteOrder.nativeOrder());
			indexArray = byteBuffer.asShortBuffer();
		} else {
			indexArray = null;
		}
	}
	
	/**
	 * return new Vertex object with current attributes
	 * @return
	 */
	public Vertex copy() {
		final Vertex vert = new Vertex(dim_num, gl, maxVertex, maxIndex, hasColor, hasTexCoord, hasNormals);
		// copy vertexArray
		if (vertexArray != null) {
			vertexArray.position(0);
			vert.vertexArray.put(vertexArray);
		}
		// copy indexArray
		if (indexArray != null) {
			indexArray.position(0);
			vert.indexArray.put(indexArray);
		}
		return vert;
	}

	/**
	 * return new Vertex object with current attributes
	 * @param vertOffset
	 * @param vertNum
	 * @param indexOffset
	 * @param indexNum
	 * @return
	 */
	public Vertex copy(int vertOffset, int vertNum, int indexOffset, int indexNum) {
		final Vertex vert = new Vertex(dim_num, gl, vertNum, indexNum, hasColor, hasTexCoord, hasNormals);
		// copy vertexArray
		if (vertexArray != null) {
			vertexArray.position(0);
			vert.vertexArray.put(vertexArray.array(), vertOffset, vertNum);
		}
		// copy indexArray
		if (indexArray != null) {
			indexArray.position(0);
			vert.indexArray.put(indexArray.array(), indexOffset, indexNum);
		}
		return vert;
	}

	/**
	 * get vertices as float[]
	 * @return
	 */
	public float[] getAsFloat() {
		final int n = vertexArray.limit();
		final float [] tmp = new float[n];
		for (int i = 0; i < n; i++)
			tmp[i] = Float.intBitsToFloat(vertexArray.get(i));
		return tmp;
	}
	
	/**
	 * move vertices with specified offset
	 * @param offset
	 */
	public void move(Vector3 offset) {
		final float [] tmp = getAsFloat();
		final int n = tmp.length;
		final int size = vertexSize / FLOAT_SZ;	// number of float for vertices
		final float[] d = offset.get();
		for (int i = 0; i < n; i+= size) {
			tmp[i] += d[0];			// tmp[i]   += offset.x();
			tmp[i+1] += d[1];		// tmp[i+1] +=offset.y();
			if (dim_num > 2)
				tmp[i+2] += d[2];	// tmp[i+2] +=offset.z();
		}
		setVertex(tmp, 0, tmp.length);
	}
	
	/**
	 * rotate vertices
	 */
	public void rotate(Vector3 angle) {
		final float[] a = angle.get();
		rotate(a[0], a[1], a[2]);
	}
	
	/**
	 * rotate vertices
	*/
	public void rotate(float x, float y, float z) {
		final Vector3 v = new Vector3();
		final float [] tmp = getAsFloat();
		final int n = tmp.length;
		final int size = vertexSize / FLOAT_SZ;	// number of float of vertices
		final int norm_offset = dim_num + (hasColor ? COLOR_NUM : 0) + (hasTexCoord ? COORD_NUM : 0);
		for (int i = 0; i < n; i+= size) {
			// rotate vertices
			v.set(tmp[i], tmp[i+1], dim_num > 2 ? tmp[i+2] : 0);
			Vector3.rotate(v, x, y, z);
			tmp[i] = v.x();
			tmp[i+1] = v.y();
			if (dim_num > 2) tmp[i+2] = v.z();
		    // rotate normal vectors
		    if (hasNormals) {
				v.set(tmp[norm_offset + i], tmp[norm_offset + i + 1], dim_num > 2 ? tmp[norm_offset + i + 2] : 0);
				Vector3.rotate(v, x, y, z);
				tmp[norm_offset + i] = v.x();
				tmp[norm_offset + i + 1] = v.y();
				if (dim_num > 2) tmp[norm_offset + i + 2] = v.z();
		    }
		}
		setVertex(tmp, 0, tmp.length);
	}

	public void setVertex(float[] vertex, int offset, int length) {
		vertexArray.clear();
		addVertex(vertex, offset, length);
	}

	public void add(Vertex vertex) {
		if (vertex == null) return;
		final int limit = vertexArray.limit();
		vertexArray.limit(vertexArray.capacity());
		vertexArray.position(limit);	// set buffer positon to end 
		addVertex(vertex.getAsFloat());
		if ((vertex.indexArray != null) && (indexArray != null)) {	// has indices?
			final short offset = (short)indexArray.limit();
			final int n = vertex.indexArray.limit();		// number of vertices index that need to add
			final short[] ixs = vertex.indexArray.array();
			final short[] index = new short[n];
			for (int i = 0; i < n; i++) {
				index[i] = (short) (ixs[i] + offset);
			}
			addIndex(index, 0, n);
		}
	}

	public void addVertex(float[] vertex) {
		addVertex(vertex, 0, vertex.length);
	}
	
	/**
	 * add vertices
	 * @param verts
	 * @param offset
	 * @param length
	 */
	public void addVertex(float[] verts, int offset, int length) {
		destroyVBO();
		final int len = offset + length;
		for (int i = offset, j = 0; i < len; i++, j++)
			tmpBuffer[j] = Float.floatToRawIntBits(verts[i]);
		vertexArray.put(tmpBuffer, 0, length);
		vertexArray.flip();
	}

	public void setIndexs(short[] index, int offset, int length) {
		indexArray.clear();
		addIndex(index, offset, length);
	}
	
	/**
	 * add indices
	 * @param index
	 * @param offset
	 * @param length
	 */
	public void addIndex(short[] index, int offset, int length) {
		destroyVBO();
		indexArray.put(index, offset, length);
		indexArray.flip();
	}


	public void bind() {
	    GL11 gl11 = null;
	    int offset_num;
	    
		if ((gl instanceof GL11)) {
			gl11 = (GL11)gl;
			hasVbo = gl11.glIsBuffer(vboId[VBO_VERTEX]);
			if (!hasVbo)
				createVBO();	// try using VBO
		} else
			hasVbo = false;

		// enable vertices array
	    gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
	    if (hasVbo) {
			// bind vertices array
			gl11.glBindBuffer(GL11.GL_ARRAY_BUFFER, vboId[VBO_VERTEX]);
			gl11.glVertexPointer(dim_num, GL10.GL_FLOAT, vertexSize, 0);
	    } else {
	    	vertexArray.position(0);
	    	gl.glVertexPointer(dim_num, GL10.GL_FLOAT, vertexSize, vertexArray);
	    }    
		// bind color array if need
	    if (hasColor) {
	        gl.glEnableClientState(GL10.GL_COLOR_ARRAY);
		    if (hasVbo) {
		    	gl11.glColorPointer(COLOR_NUM, GL10.GL_FLOAT, vertexSize, dim_num * FLOAT_SZ);
		    } else {
		        vertexArray.position(dim_num);
		        gl.glColorPointer(COLOR_NUM, GL10.GL_FLOAT, vertexSize, vertexArray);
		    }
	    }
		// bind texture coordinates array if need
	    if (hasTexCoord) {
	    	gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
	    	offset_num = dim_num + (hasColor ? COLOR_NUM : 0);
		    if (hasVbo) {
	    		gl11.glTexCoordPointer(COORD_NUM, GL10.GL_FLOAT, vertexSize, offset_num * FLOAT_SZ);
		    } else {
		    	vertexArray.position(offset_num);
		    	gl.glTexCoordPointer(COORD_NUM, GL10.GL_FLOAT, vertexSize, vertexArray);
		    }
	    }
	    // bind normal coordinates array if need
	    if (hasNormals) {
	    	gl.glEnableClientState(GL10.GL_NORMAL_ARRAY);
	    	offset_num = dim_num + (hasColor ? COLOR_NUM : 0) + (hasTexCoord ? COORD_NUM : 0);
		    if (hasVbo) {
		    	gl11.glNormalPointer(GL10.GL_FLOAT, vertexSize, offset_num * FLOAT_SZ);
		    } else {
		    	vertexArray.position(offset_num);
		    	gl.glNormalPointer(GL10.GL_FLOAT, vertexSize, vertexArray);
		    }
	    }
	    // bind indices array if need
	    if ((indexArray != null) && (hasVbo)) {
			gl11.glBindBuffer(GL11.GL_ELEMENT_ARRAY_BUFFER, vboId[VBO_INDEX]);
	    }

	}

	/**
	 * release bindings
	 */
	public void unbind() {
	    if (hasTexCoord)
	        gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);

	    if (hasColor)
	        gl.glDisableClientState(GL10.GL_COLOR_ARRAY);

	    if (hasNormals)
	        gl.glDisableClientState(GL10.GL_NORMAL_ARRAY);

		if ((gl instanceof GL11) && hasVbo) {
			final GL11 gl11 = (GL11)gl;
    		gl11.glBindBuffer(GL11.GL_ARRAY_BUFFER, 0);
    		if (indexArray != null)
    			gl11.glBindBuffer(GL11.GL_ELEMENT_ARRAY_BUFFER, 0);
    	}
	}

	/**
	 * draw vertices
	 * @param primitiveType
	 * @param offset
	 * @param numVertex
	 */
	protected void draw(int primitiveType, int offset, int numVertex) {        
	    GL11 gl11 = null;
	    
		if ((gl instanceof GL11)) {
			gl11 = (GL11)gl;
		} else
			hasVbo = false;
	    
	    if (indexArray != null) {
	    	// bind indices if need
	    	if (hasVbo) {
				gl11.glDrawElements(primitiveType, numVertex, GL10.GL_UNSIGNED_SHORT, offset);
	    	} else {
	    		indexArray.position(offset);
	    		gl.glDrawElements(primitiveType, numVertex, GL10.GL_UNSIGNED_SHORT, indexArray);
	    	}
	    } else {
	    	if (hasVbo) {
	    		gl11.glDrawArrays(primitiveType, offset, numVertex);
	    	} else {
	    		gl.glDrawArrays(primitiveType, offset, numVertex);
	    	}
	    }        
	}

	/**
	 * draw vertices
	 * @param primitiveType
	 */
	public void draw(int primitiveType) {
	    if (indexArray != null)
	    	draw(primitiveType, 0, getNumIndex());
	    else
		   	draw(primitiveType, 0, getNumVertex());
	}

	public void resume() {
		createVBO();
	}
	
	public void pause() {
		destroyVBO();
	}

	// return number of indices
	protected int getNumIndex() {
		return indexArray.limit();
	}
	
	// return number of coordinates pair
	protected int getNumVertex() {
		return vertexArray.limit() / (vertexSize / FLOAT_SZ);
	}
	
	/**
	 * create VBO
	 */
	protected void createVBO() {

	    destroyVBO();

		if (!(gl instanceof GL11)) {
			return;
		}
		final GL11 gl11 = (GL11)gl;

		// generate VBO IDs
		gl11.glGenBuffers(vboId.length, vboId, 0);

		// assign data
		if (vboId[VBO_VERTEX] != 0) {
			// assign vertices
			vertexArray.position(0);
			gl11.glBindBuffer(GL11.GL_ARRAY_BUFFER, vboId[VBO_VERTEX]);
			gl11.glBufferData(GL11.GL_ARRAY_BUFFER, vertexArray.limit() * INT_SZ, vertexArray, GL11.GL_STATIC_DRAW);
			gl11.glBindBuffer(GL11.GL_ARRAY_BUFFER, 0);

			// assign TriangleVertexIndex
			if (indexArray != null) {
				indexArray.position(0);
				gl11.glBindBuffer(GL11.GL_ELEMENT_ARRAY_BUFFER, vboId[VBO_INDEX]);
				gl11.glBufferData(GL11.GL_ELEMENT_ARRAY_BUFFER, indexArray.limit() * SHORT_SZ, indexArray, GL11.GL_STATIC_DRAW);
				gl11.glBindBuffer(GL11.GL_ELEMENT_ARRAY_BUFFER, 0);
			}
			hasVbo = true;
		}
	}

	/**
	 * free VBO
	 */
	protected void destroyVBO() {
	    hasVbo = false;

		if (vboId[VBO_VERTEX] == 0)
			return;
		if (!(gl instanceof GL11))
			return;

		final GL11 gl11 = (GL11)gl;

		gl11.glDeleteBuffers(vboId.length, vboId, 0);
		Arrays.fill(vboId, 0);
	}

}
