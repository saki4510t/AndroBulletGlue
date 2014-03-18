/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 *
 * File name: mygl.h
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

#ifndef MYGL_H_
#define MYGL_H_

#include "btBulletCollisionCommon.h"
#include "GLES/gl.h"
#include "btVector3.h"

#include "localdefines.h"
#include "utils.h"

#define	DIM_NUM		3		// 頂点座標の次元

#define COORD_NUM	2		// テクスチャ情報のサイズ[float x 個]
#define COLOR_NUM	4		// 色情報のサイズ[float x 個]
#define NORMAL_NUM	3		// 頂点法線情報のサイズ[float x 個]

void
setAutoTexAxis(int s, int t, int r);

void
setAutoTexGenEnabled(bool enable);

class Mygl {
private:
	GLfloat current_r;
	GLfloat current_g;
	GLfloat current_b;
	GLfloat current_a;
	GLfloat current_nx;
	GLfloat current_ny;
	GLfloat current_nz;

protected:
	GLfloat *mVertices;		// 頂点情報配列
	int mVertexSize;		// 頂点情報1つのサイズ[GLfloatの数]
	int mVertexStride;		// 頂点情報1つのサイズ[バイト]
	int mMaxVertices;		// 頂点情報配列のサイズ
	int mVertexIx;			// 頂点情報配列への次の書き込み位置インデックス
	int mVertexNum;			// 頂点情報の個数
	int mTexIx;				// テクスチャ情報の次の書き込み位置インデックス

	GLshort *mIndices;		// インデックス配列
	int mMaxIndices;		// インデックス配列のサイズ
	int mIndexIx;			// インデックス配列への次の書き込み位置

	bool mHasColor;			// 色情報有り？
	bool mHasTexCoord;		// テクスチャ情報有り？
	bool mHasNormal;		// 法線ベクトル有り？

	int mPrimitiveType;		// 描画する要素の種類

public:
	Mygl(int maxVertices, int maxIndices, bool hasColor, bool hasTexCoord, bool hasNormal);
	~Mygl();
	void glBegin(int primitiveType);
	void glEnd();

	void glVertex(const GLfloat *vertices, int offset, int num);
	void glVertex(GLfloat x, GLfloat y, GLfloat z);
	void glVertex(const btVector3& p);
	void glVertex(const btVector3& p1, const btVector3& p2);
	void glVertex(const btVector3& p1, const btVector3& p2, const btVector3& p3);
	void glVertex(const btVector3& p1, const btVector3& p2, const btVector3& p3, const btVector3& p4);

	void glNormal(GLfloat x, GLfloat y, GLfloat z);
	void glNormal(const btVector3& p);

	void glColor(const btVector3& cl);
	void glColor(const btVector3& cl, GLfloat a);
	void glColor(GLfloat r, GLfloat g, GLfloat b);
	void glColor(GLfloat r, GLfloat g, GLfloat b, float a);

	void glIndex(const GLshort *indices, int offset, int num);
	void glIndex(GLshort ix1, GLshort ix2);
	void glIndex(GLshort ix1, GLshort ix2, GLshort ix3);
	void glIndex(GLshort ix1, GLshort ix2, GLshort ix3, GLshort ix4);
	void glIndex(GLshort ix1);
};

class PixelStoreState {
public:
	int unpackRowLength;
	int unpackAlignment;
	int unpackSkipRows;
	int unpackSkipPixels;
	int packRowLength;
	int packAlignment;
	int packSkipRows;
	int packSkipPixels;

	PixelStoreState() {
		unpackRowLength = unpackSkipRows = unpackSkipPixels = 0;
		packRowLength = packAlignment = packSkipRows = packSkipPixels = 0;
		unpackAlignment = 1;
		load();
	}

	void load(void) {
//		glGetIntegerv(GL_UNPACK_ROW_LENGTH, &unpackRowLength);
//		glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlignment);
//		glGetIntegerv(GL_UNPACK_SKIP_ROWS, &unpackSkipRows);
//		glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &unpackSkipPixels);
//		glGetIntegerv(GL_PACK_ROW_LENGTH, &packRowLength);
		glGetIntegerv(GL_PACK_ALIGNMENT, &packAlignment);
//		glGetIntegerv(GL_PACK_SKIP_ROWS, &packSkipRows);
//		glGetIntegerv(GL_PACK_SKIP_PIXELS, &packSkipPixels);
	}

	void restore() {
//		glPixelStorei(GL_UNPACK_ROW_LENGTH, unpackRowLength);
		glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment);
//		glPixelStorei(GL_UNPACK_SKIP_ROWS, unpackSkipRows);
//		glPixelStorei(GL_UNPACK_SKIP_PIXELS, unpackSkipPixels);
//		glPixelStorei(GL_PACK_ROW_LENGTH, packRowLength);
		glPixelStorei(GL_PACK_ALIGNMENT, packAlignment);
//		glPixelStorei(GL_PACK_SKIP_ROWS, packSkipRows);
//		glPixelStorei(GL_PACK_SKIP_PIXELS, packSkipPixels);
	}
};

void glDrawColor(int primitiveType, const GLfloat *vertices, const GLfloat *normals, const GLfloat *colors, const GLshort *indices);
void glDraw(int primitiveType, const GLfloat *vertices, const GLfloat *normals, const GLshort *indices);
void glDrawColor(int primitiveType, const GLfloat *vertices, const GLfloat *colors, const GLshort *indices);
void glDraw(int primitiveType, const GLfloat *vertices, const GLshort *indices);
void glDraw(int primitiveType, const GLfloat *vertices);

void glDrawLine(const btVector3& p1, const btVector3& p2);
void glDrawTriangle(const btVector3& p1, const btVector3& p2, const btVector3& p3);

void glSetVector(float *array, const btVector3 v);

void glCylinder(int ndiv, GLfloat radius, GLfloat halfHeight);
void glCube(btVector3 &halfExtent, GLfloat texMapNum);

#endif /* MYGL_H_ */
