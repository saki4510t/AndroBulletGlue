/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: mygl.cpp
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

#include "mygl.h"

#define USE_AUTOTEXGEN 1

static bool mAutoTexGen = false;
// テクスチャ座標の自動生成時にテクスチャ座標のs値としてどの軸(x=0, y=1, z=2)を使うかを指定
static GLint mAutoTexAxisS = 0;
static GLint mAutoTexAxisT = 1;
// static GLint mAutoTexAxisR = -1;


void setAutoTexAxis(int s, int t, int r) {
	mAutoTexAxisS = s;
	mAutoTexAxisT = t;
//	mAutoTexAxisR = r;
}

void setAutoTexGenEnabled(bool enable) {
	mAutoTexGen = enable;
}
/**
 * 色指定無し法線ベクトル有りインデックス付きで描画(3D)
 */
void glDrawColor(int primitiveType, const GLfloat *vertices, const GLfloat *normals, const GLfloat *colors, const GLshort *indices) {
	// 座標
	glVertexPointer(DIM_NUM, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	// 法線ベクトル
	glNormalPointer(DIM_NUM, GL_FLOAT, normals);
	glEnableClientState(GL_NORMAL_ARRAY);
	// 色情報
	glColorPointer(DIM_NUM, GL_FLOAT, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawElements(primitiveType, sizeof(indices), GL_SHORT, indices);
}

/**
 * 色指定無し法線ベクトル有りインデックス付きで描画(3D)
 */
void glDraw(int primitiveType, const GLfloat *vertices, const GLfloat *normals, const GLshort *indices) {
	// 座標
	glVertexPointer(DIM_NUM, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	// 法線ベクトル
	glNormalPointer(DIM_NUM, GL_FLOAT, normals);
	glEnableClientState(GL_NORMAL_ARRAY);
	// 色情報なし
	glDisableClientState(GL_COLOR_ARRAY);
	glDrawElements(primitiveType, sizeof(indices), GL_SHORT, indices);
}

/**
 * 色指定付き法線ベクトル無しインデックス付きで描画(3D)
 */
void glDrawColor(int primitiveType, const GLfloat *vertices, const GLfloat *colors, const GLshort *indices) {
	// 座標
	glVertexPointer(DIM_NUM, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	// 色情報
	glColorPointer(DIM_NUM, GL_FLOAT, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);
	// 法線ベクトル無し
	glDisableClientState(GL_NORMAL_ARRAY);
	glDrawElements(primitiveType, sizeof(indices), GL_SHORT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * draw primitive without color but with index
 */
void glDraw(int primitiveType, const GLfloat *vertices, const GLshort *indices) {
	glVertexPointer(DIM_NUM, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(primitiveType, sizeof(indices), GL_SHORT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * draw primitive without color and without index
 */
void glDraw(int primitiveType, const GLfloat *vertices) {
	glVertexPointer(DIM_NUM, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(primitiveType, 0, sizeof(vertices) / 3);
}

/**
 * draw line without color
 */
void glDrawLine(const btVector3 &p1, const btVector3 &p2) {
	GLfloat vertices[6];

	for (int i = 0; i < 3; i++) {
		vertices[i] = p1[i];
		vertices[i+3] = p2[i];
	}
	glDraw(GL_LINES, vertices);
}

/**
 * draw triangle without color
 */
void glDrawTriangle(const btVector3 &p1, const btVector3 &p2, const btVector3 &p3) {
	GLfloat vertices[9];
	GLshort indices[6] = {0, 1, 1, 2, 2, 0};

	for (int i = 0; i < 3; i++) {
		vertices[i] = p1[i];
		vertices[i+3] = p2[i];
		vertices[i+6] = p3[i];
	}
	glDraw(GL_TRIANGLES, vertices, indices);
}

void glSetVector(float *vertices, const btVector3 v) {
	vertices[0] = v.x();
	vertices[1] = v.y();
	vertices[2] = v.z();
}

void glCylinder(int ndiv, GLfloat radius, GLfloat halfHeight) {
	Mygl gl(ndiv * 2, ndiv * 2, false, false, true);
	int i;
	float ang;
	int vx = 0, nx = 0, ix = 0;
	// bottom
	gl.glBegin(GL_TRIANGLE_FAN);
	gl.glNormal(0.f, -1.f, 0.f);
	gl.glVertex(0.f, -radius / 2.f, 0.f);
	gl.glIndex(0);
	for (i = 0; i <= ndiv; i++) {
		ang = 2.f * M_PI / ndiv * i;
		gl.glNormal(0.f, -1.f, 0.f);
		gl.glVertex((float)cos(ang), -radius / 2.f, (float)sin(ang));
		gl.glIndex(ix);
	}
	gl.glEnd();
	// side wall
	vx = nx = ix = 0;
	gl.glBegin(GL_TRIANGLE_FAN);	//	glBegin(GL_QUAD_STRIP);
	for (i = 0 ;i <= ndiv; i++) {
		ang = 2.f * M_PI / ndiv * i;
		gl.glNormal((float)cos(ang), 0.f, (float)sin(ang));
		gl.glVertex((float)cos(ang), halfHeight, (float)sin(ang));
		gl.glIndex(ix);
		gl.glVertex((float)cos(ang), -halfHeight, (float)sin(ang));
		gl.glIndex(ix);
	}
	gl.glEnd();
}

void glCube(btVector3 &halfExtent, GLfloat texMapNum) {
	float *vertices;
	short *indices;
	int vertex_size;	// size of vertex for calculation of normal vector
	int norm_offset;	// offset to indicate the position of normal vector for calculation

	GLfloat x = halfExtent[0];
	GLfloat y = halfExtent[1];
	GLfloat z = halfExtent[2];

	bool hasTexture = (texMapNum > 0);
	if (hasTexture) {	// when there is texture
		GLfloat v[] = {
			// vertex,  texture coordinates,    normal vector(dummy)
			-x, -y,  z, 0, texMapNum,			0, 0, 0,
			 x, -y,  z, texMapNum, texMapNum,	0, 0, 0,
			 x,  y,  z, texMapNum, 0,			0, 0, 0,
			-x,  y,  z, 0, 0,					0, 0, 0,

			 x, -y,  z, 0, texMapNum,			0, 0, 0,
			 x, -y, -z, texMapNum, texMapNum,	0, 0, 0,
			 x,  y, -z, texMapNum, 0,			0, 0, 0,
			 x,  y,  z, 0, 0,					0, 0, 0,

			 x, -y, -z, 0, texMapNum,			0, 0, 0,
			-x, -y, -z, texMapNum, texMapNum,	0, 0, 0,
			-x,  y, -z, texMapNum, 0,			0, 0, 0,
			 x,  y, -z, 0, 0,					0, 0, 0,

			-x, -y, -z, 0, texMapNum,			0, 0, 0,
			-x, -y,  z, texMapNum, texMapNum,	0, 0, 0,
			-x,  y,  z, texMapNum, 0,			0, 0, 0,
			-x,  y, -z, 0, 0,					0, 0, 0,

			-x,  y,  z, 0, texMapNum,			0, 0, 0,
			 x,  y,  z, texMapNum, texMapNum,	0, 0, 0,
			 x,  y, -z, texMapNum, 0,			0, 0, 0,
			-x,  y, -z, 0, 0,					0, 0, 0,

			-x, -y,  z, 0, texMapNum,			0, 0, 0,
			 x, -y,  z, texMapNum, texMapNum,	0, 0, 0,
			 x, -y, -z, texMapNum, 0,			0, 0, 0,
			-x, -y, -z, 0, 0,					0, 0, 0,
		};
		vertices = &v[0];
		vertex_size = 8;	// for calculation of normal vector
		norm_offset = 5;	// for calculation of normal vector
		short in[] = {
			0, 1, 3, 1, 2, 3,
			4, 5, 7, 5, 6, 7,
			8, 9, 11, 9, 10, 11,
			12, 13, 15, 13, 14, 15,
			16, 17, 19, 17, 18, 19,
			20, 21, 23, 21, 22, 23
		};
		indices = &in[0];
	} else {	// when there is no texture
		GLfloat v[] = {
			// vertex	normal vector(dummy)
			-x, -y, -z, 0, 0, 0,
			 x, -y, -z, 0, 0, 0,
			 x,  y, -z, 0, 0, 0,
			-x,  y, -z, 0, 0, 0,
			-x, -y,  z, 0, 0, 0,
			 x, -y,  z, 0, 0, 0,
			 x,  y,  z, 0, 0, 0,
			-x,  y,  z, 0, 0, 0,
		};
		vertices = &v[0];
		vertex_size = 6;	// for calculation of normal vector
		norm_offset = 3;	// for calculation of normal vector
		short in[] = {
			0, 4, 5, 0, 5, 1,
			1, 5, 6, 1, 6, 2,
			2, 6, 7, 2, 7, 3,
			3, 7, 4, 3, 4, 0,
			4, 7, 6, 4, 6, 5,
			3, 0, 1, 3, 1, 2,
		};
		indices = &in[0];
	}

	int n = sizeof(vertices);
	int m = sizeof(indices);
	Mygl gl(n, m, false, hasTexture, true);
	// calculate normal verctor(=normalized vertex coordinate as a vector)
	for (int ix = 0; ix < n; ix += vertex_size) {
		btVector3 normal(vertices[ix], vertices[ix + 1], vertices[ix + 2]);
		normal.normalize();
		vertices[ix + norm_offset    ] = normal[0];
		vertices[ix + norm_offset + 1] = normal[1];
		vertices[ix + norm_offset + 2] = normal[2];
	}
	gl.glBegin(GL_TRIANGLES);
	gl.glVertex(vertices, 0, n);
	gl.glIndex(indices, 0, m);
	gl.glEnd();
}

Mygl::Mygl(int maxVertices, int maxIndices, bool hasColor, bool hasTexCoord, bool hasNormal) {
//	LOGI("Mygl:autoTexGen=%d", autoTexGen);
	mHasColor = hasColor;
#if USE_AUTOTEXGEN
	mHasTexCoord = hasTexCoord | mAutoTexGen;
#else
	mHasTexCoord = hasTexCoord;
#endif
	mHasNormal = hasNormal;
	mVertexSize = (DIM_NUM
		+ (hasColor ? COLOR_NUM : 0)
		+ (mHasTexCoord ? COORD_NUM : 0)
		+ (hasNormal ? NORMAL_NUM : 0) );			// number of GLfloat for each vertex
	mVertexStride = mVertexSize * sizeof(GLfloat);	// stride should be bytes
	mMaxVertices = mVertexSize * maxVertices;		// number of floats
	mVertices = new GLfloat[mMaxVertices];
	mMaxIndices = maxIndices;
	if (maxIndices) {
		mIndices = new GLshort[maxIndices];
	} else {
		mIndices = NULL;
	}
	mVertexIx = mIndexIx = 0;
	mTexIx = DIM_NUM + (hasColor ? COLOR_NUM : 0);
	int mIndexIx;
}

Mygl::~Mygl() {
//	LOGD("Mygl:delete mVertices");
	SAFE_DELETE_ARRAY(mVertices);
//	LOGD("Mygl:delete mIndices");
	SAFE_DELETE_ARRAY(mIndices);
}

void Mygl::glBegin(int primitiveType) {
	mPrimitiveType = primitiveType;
	mVertexIx = mVertexNum = mIndexIx = 0;
	current_r = current_g = current_b = 0.f;
	current_a = 1.f;
	current_nx = current_ny = current_nz = 0.f;
}

void Mygl::glEnd() {
	// setup vertices
    glEnableClientState(GL_VERTEX_ARRAY);
   	glVertexPointer(DIM_NUM, GL_FLOAT, mVertexStride, mVertices);
	// setup colors
    if (mHasColor) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(COLOR_NUM, GL_FLOAT, mVertexStride, &mVertices[DIM_NUM]);
    } else {
        glDisableClientState(GL_COLOR_ARRAY);
    }
	// setup texture coordinates
    if (mHasTexCoord) {
    	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    	int offset_num = DIM_NUM + (mHasColor ? COLOR_NUM : 0);
    	glTexCoordPointer(COORD_NUM, GL_FLOAT, mVertexStride, &mVertices[offset_num]);
    } else {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    // setup normals
    if (mHasNormal) {
    	glEnableClientState(GL_NORMAL_ARRAY);
    	int offset_num = DIM_NUM + (mHasColor ? COLOR_NUM : 0) + (mHasTexCoord ? COORD_NUM : 0);
    	glNormalPointer(GL_FLOAT, mVertexStride, &mVertices[offset_num]);
    } else {
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    if (mIndices && mIndexIx) {
    	// draw using vertex index
    	glDrawElements(mPrimitiveType, mIndexIx, GL_SHORT, mIndices);
    } else if (mVertexNum) {
    	// draw vertices array
		glDrawArrays(mPrimitiveType, 0, mVertexNum);
    }

	// 後始末
    if (mHasTexCoord)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (mHasColor)
        glDisableClientState(GL_COLOR_ARRAY);
    if (mHasNormal)
        glDisableClientState(GL_NORMAL_ARRAY);

}

void Mygl::glVertex(const GLfloat *vertices, int offset, int num) {
	if (UNLIKELY(mVertexIx + num > mMaxVertices)) {
		LOGE("Mygl#glVertex: mVertexIx overflow");
		return;
	}
/*	for (int i = 0; i < num; i++) {
		mVertices[mVertexIx++] = vertices[offset + i];
	} */
	memcpy(&mVertices[mVertexIx], &vertices[offset], num);
	mVertexIx += num;
	mVertexNum += num / mVertexSize;
}

void Mygl::glVertex(GLfloat x, GLfloat y, GLfloat z) {
	if (mVertexIx + mVertexSize > mMaxVertices) {
		LOGE("Mygl#glVertex: mVertexIx overflow");
		return;
	}
	// set vertices coordinates
	mVertices[mVertexIx    ] = x;
	mVertices[mVertexIx + 1] = y;
	mVertices[mVertexIx + 2] = z;

	if (mHasColor) {
		// set colors
		mVertices[mVertexIx + DIM_NUM    ] = current_r;
		mVertices[mVertexIx + DIM_NUM + 1] = current_g;
		mVertices[mVertexIx + DIM_NUM + 2] = current_b;
		mVertices[mVertexIx + DIM_NUM + 3] = current_a;
	}
#if USE_AUTOTEXGEN
	if (mAutoTexGen && mHasTexCoord) {
		// calculate texture coordinates
    	int offset_num = mVertexIx + DIM_NUM + (mHasColor ? COLOR_NUM : 0);
    	mVertices[offset_num    ]
    		= (mAutoTexAxisS == 0 ? x
    		: (mAutoTexAxisS == 1 ? y
    		: (mAutoTexAxisS == 2 ? z
    		: 0.f)) );
    	mVertices[offset_num + 1]
    		= (mAutoTexAxisT == 0 ? x
    		: (mAutoTexAxisT == 1 ? y
    		: (mAutoTexAxisT == 2 ? z
    		: 0.f)) );
/*    	mVertices[ix + 2] = mAutoTexScaleR	// texturing on OpenGL|ES is only available for 2D coordiantes
    		* (mAutoTexAxisR == 0 ? x
    		: (mAutoTexAxisR == 1 ? y
    		: (mAutoTexAxisR == 2 ? z
    		: 0.f)) ); */
	}
#endif
    if (mHasNormal) {
    	// set normal vector
    	int offset_num = mVertexIx + DIM_NUM + (mHasColor ? COLOR_NUM : 0) + (mHasTexCoord ? COORD_NUM : 0);
    	mVertices[offset_num    ] = current_nx;
    	mVertices[offset_num + 1] = current_ny;
    	mVertices[offset_num + 2] = current_nz;
    }
	mVertexIx += mVertexSize;	// move to position of next vertex info record
	mVertexNum++;				// add num of vertices
}

void Mygl::glVertex(const btVector3 &p) {
	glVertex((GLfloat)p.getX(), (GLfloat)p.getY(), (GLfloat)p.getZ());
}

void Mygl::glVertex(const btVector3 &p1, const btVector3 &p2) {
	if (UNLIKELY(mVertexIx + mVertexSize * 2 > mMaxVertices)) {
		LOGE("Mygl#glVertex: mVertexIx overflow");
		return;
	}
	glVertex((GLfloat)p1.getX(), (GLfloat)p1.getY(), (GLfloat)p1.getZ());
	glVertex((GLfloat)p2.getX(), (GLfloat)p2.getY(), (GLfloat)p2.getZ());
}

void Mygl::glVertex(const btVector3 &p1, const btVector3 &p2, const btVector3 &p3) {
	if (UNLIKELY(mVertexIx + mVertexSize * 3 > mMaxVertices)) {
		LOGE("Mygl#glVertex: mVertexIx overflow");
		return;
	}
	glVertex((GLfloat)p1.getX(), (GLfloat)p1.getY(), (GLfloat)p1.getZ());
	glVertex((GLfloat)p2.getX(), (GLfloat)p2.getY(), (GLfloat)p2.getZ());
	glVertex((GLfloat)p3.getX(), (GLfloat)p3.getY(), (GLfloat)p3.getZ());
}

void Mygl::glVertex(const btVector3 &p1, const btVector3 &p2, const btVector3 &p3, const btVector3 &p4) {
	if (UNLIKELY(mVertexIx + mVertexSize * 4 > mMaxVertices)) {
		LOGE("Mygl#glVertex: mVertexIx overflow");
		return;
	}
	glVertex((GLfloat)p1.getX(), (GLfloat)p1.getY(), (GLfloat)p1.getZ());
	glVertex((GLfloat)p2.getX(), (GLfloat)p2.getY(), (GLfloat)p2.getZ());
	glVertex((GLfloat)p3.getX(), (GLfloat)p3.getY(), (GLfloat)p3.getZ());
	glVertex((GLfloat)p4.getX(), (GLfloat)p4.getY(), (GLfloat)p4.getZ());
}

void Mygl::glNormal(float x, float y, float z) {
	current_nx = x;
	current_ny = y;
	current_nz = z;
}

void Mygl::glNormal(const btVector3 &p) {
	glNormal((GLfloat)p.getX(), (GLfloat)p.getY(), (GLfloat)p.getZ());
}

void Mygl::glColor(const btVector3 &cl) {
	current_r = cl.getX();
	current_g = cl.getY();
	current_b = cl.getZ();
	current_a = 1.f;
}

void Mygl::glColor(const btVector3 &cl, GLfloat a) {
	current_r = cl.getX();
	current_g = cl.getY();
	current_b = cl.getZ();
	current_a = a;
}

void Mygl::glColor(GLfloat r, GLfloat g, GLfloat b) {
	current_r = r;
	current_g = g;
	current_b = b;
	current_a = 1.f;
}

void Mygl::glColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	current_r = r;
	current_g = g;
	current_b = b;
	current_a = a;
}

void Mygl::glIndex(const GLshort *indices, int offset, int num) {
	if (UNLIKELY(mIndexIx + num > mMaxIndices)) {
		LOGE("Mygl#glIndex: mIndexIx overflow");
		return;
	}
/*	for (int i = 0; i < num; i++) {
		mIndices[mIndexIx++] = indices[offset + i];
	} */
	memcpy(&mIndices[mIndexIx], &indices[offset], num);
	mIndexIx += num;
}

void Mygl::glIndex(GLshort ix1) {
	if (UNLIKELY(mIndexIx + 1 > mMaxIndices)) {
		LOGE("Mygl#glIndex: mIndexIx overflow");
		return;
	}
	mIndices[mIndexIx++] = ix1;
}

void Mygl::glIndex(GLshort ix1, GLshort ix2) {
	if (UNLIKELY(mIndexIx + 2 > mMaxIndices)) {
		LOGE("Mygl#glIndex: mIndexIx overflow");
		return;
	}
	mIndices[mIndexIx++] = ix1;
	mIndices[mIndexIx++] = ix2;
}

void Mygl::glIndex(GLshort ix1, GLshort ix2, GLshort ix3) {
	if (UNLIKELY(mIndexIx + 3 > mMaxIndices)) {
		LOGE("Mygl#glIndex: mIndexIx overflow");
		return;
	}
	mIndices[mIndexIx++] = ix1;
	mIndices[mIndexIx++] = ix2;
	mIndices[mIndexIx++] = ix3;
}

void Mygl::glIndex(GLshort ix1, GLshort ix2, GLshort ix3, GLshort ix4) {
	if (UNLIKELY(mIndexIx + 4 > mMaxIndices)) {
		LOGE("Mygl#glIndex: mIndexIx overflow");
		return;
	}
	mIndices[mIndexIx++] = ix1;
	mIndices[mIndexIx++] = ix2;
	mIndices[mIndexIx++] = ix3;
	mIndices[mIndexIx++] = ix4;
}

