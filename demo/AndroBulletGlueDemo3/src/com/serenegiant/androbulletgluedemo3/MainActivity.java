/*
 * AndroBulletGlueDemo3
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: MainActivity.java
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
package com.serenegiant.androbulletgluedemo3;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.serenegiant.androbulletglue.BulletManager;
import com.serenegiant.androbulletglue.MotionState;
import com.serenegiant.androbulletglue.PhysicsWorld;
import com.serenegiant.androbulletglue.RigidBody;
import com.serenegiant.androbulletglue.shape.BoxShape;
import com.serenegiant.androbulletglue.shape.StaticPlaneShape;

import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.os.Bundle;
import android.app.Activity;
import android.view.MotionEvent;

public class MainActivity extends Activity {

	private PhysicsWorld mWorld;
	private RigidBody mCubeBody;
    private Cube mCube;
    
    private GLSurfaceView mGLSurfaceView;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
        mGLSurfaceView = new GLSurfaceView(this);
        setContentView(mGLSurfaceView);
        mGLSurfaceView.setRenderer(mRenderer);
        mGLSurfaceView.requestFocus();
        mGLSurfaceView.setFocusableInTouchMode(true);
    	mCube = new Cube();
	}

    @Override
	protected void onPause() {
        mGLSurfaceView.onPause();
        if (isFinishing()) {
        	// terminate physics engine when activity is disposing
    		freePhysics();
        }
        super.onPause();
	}

	@Override
	protected void onResume() {
        super.onResume();
        mGLSurfaceView.onResume();
	}

	private volatile boolean needReset;	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (!needReset && ((event.getAction() & MotionEvent.ACTION_UP) != 0)) {
			// set reset frag on
			needReset = true;
		}
		return super.onTouchEvent(event);
	}

    private final GLSurfaceView.Renderer mRenderer = new GLSurfaceView.Renderer() {
		@Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	        gl.glDisable(GL10.GL_DITHER);
            gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_FASTEST);
            gl.glClearColor(1f, 1f, 1f, 1f);
            gl.glEnable(GL10.GL_CULL_FACE);
            gl.glShadeModel(GL10.GL_SMOOTH);
            gl.glEnable(GL10.GL_DEPTH_TEST);
            // initialize physics engine
            initPhysics();
        }

		@Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
			gl.glViewport(0, 0, width, height);
			final float aspect = (float) width / height;
			gl.glMatrixMode(GL10.GL_PROJECTION);
            gl.glLoadIdentity();
            GLU.gluPerspective(gl, 67f, aspect, 0.1f, 20f);
            gl.glMatrixMode(GL10.GL_MODELVIEW);
            gl.glLoadIdentity();
            gl.glTranslatef(0f, -0.5f, -3f);
            // create object and initialize
            mCube.init(gl);
	    	createObjects(mWorld);
        }
		
		@Override
        public void onDrawFrame(GL10 gl) {
			gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
       
            if (needReset) {
            	// when reset request. remove all objects and re-create
            	needReset = false;
            	freePhysics();
            	initPhysics();
		    	createObjects(mWorld);
            }
			// simulate
            mWorld.doSimulation(1f / 60f, 10);
			// draw
            mCube.draw(gl, mCubeBody.getOrigin());
		}
    };

    /**
     * initialize physics engine
     */
    private final void initPhysics() {
    	BulletManager bulletManager = BulletManager.getBulletManager();
    	if (bulletManager == null) {
    		bulletManager = BulletManager.initialize(true);
    	}
    	if (mWorld == null)
    		mWorld = new PhysicsWorld(-9.8f);
    }
    
    /**
     * create object
     * @param world
     */
    private final void createObjects(PhysicsWorld world) {
    	if (mCubeBody == null) {
	        // create ground
	    	new RigidBody(world, new StaticPlaneShape(world, 0f, 1f, 0f, -56f));
	        // create cube
	    	mCubeBody = new RigidBody(world,
	    		new BoxShape(world, 1f, 1f, 1f),	// collision shape
	   			1f,									// mass
	    		new MotionState(2f, 10f, 0f)		// initial position
	    	);
    	}
    }
    
    /**
     * dispose objects and terminate physics engine
     */
    private final void freePhysics() {
		if (mCubeBody != null) {
			mCubeBody.dispose();
			mCubeBody = null;				
		}
		if (mWorld != null) {
			mWorld.dispose();
			mWorld = null;
		}
		BulletManager.destroy();
    }
}
