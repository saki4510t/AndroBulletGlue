/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: GLBulletActivity.java
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

package com.serenegiant.androbulletglue;

import javax.microedition.khronos.opengles.GL10;

import com.serenegiant.androbulletglue.GLBulletWorldSurfaceView.GLBulletWorldSurfaceViewCallback;
import com.serenegiant.androbulletglue.math.Vector3;

import android.app.Activity;
import android.os.Bundle;

/**
 * The base activity class to demonstrate how to use the GLBulletWorldSurfaceView.</br>
 * Known issue: if this activity re-created without disposing physics objects, 
 * can not render correctly although the simulating and drawing methods are called normally.
 * maybe context lost of Open GL...</br>
 * Therefor you need to put android:configChanges="orientation"(or orientation|screenSize)
 * in your AndroidManifest and prevent re-creation of activity,
 * or you need to dispose all physics objects when screen rotation occured.
 * </br>
 * We execute simulation on simulation thread of Java and render objects
 * using renderer thread of GLSurfaceView to syncronzie.
 * But we may change this behavior or add other technique like using native activity
 * to improve performace
 */
public abstract class GLBulletActivity extends Activity implements GLBulletWorldSurfaceViewCallback {

	static {
		BulletManager.loadBulletLib();
	};
	
	protected final String TAG = getClass().getSimpleName();
    private GLBulletWorldSurfaceView mGLView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGLView = new GLBulletWorldSurfaceView(this, this);
        setContentView(mGLView);
    }

    @Override
    protected void onPause() {
        mGLView.onPause();
        if (isFinishing()) {
        	mGLView.onDispose();
			BulletManager.destroy();
        }
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.setCallBack(this);
        mGLView.onResume();
    }
       
    protected PhysicsWorld getPhysicsWorld() {
    	return mGLView.getPhysicsWorld();
    }

    /**
     * set intervals between simulation
     * @param time: the default value is 1/60f, if you set smaller than 0, execute with actual time
     */
    public void setExecTime(float time) {
    	mGLView.setExecTime(time);
    }
    
    /**
     * set sub steps of simulation
     * @param subSteps: default value is 10
     */
    public void setSubSteps(int subSteps) {
    	mGLView.setSubSteps(subSteps);
    }
    
    /**
     * set the state of simulation
     * @param active true: execute, false: stop
     */
    public void setActive(boolean active) {
    	mGLView.setActive(active);
    }
    
    /**
     * set the state of simulation
     * @return true: executing, false: stopped
     */
    public boolean getActive() {
    	return mGLView.getActive();
    }
    
    /**
     * request restart physics world
     */
    public void resetWorld() {
    	mGLView.resetWorld();
    }
    
    /**
     * move camera
     * @param x positive value moves camera turn right, negative value moves camera turn left
     * @param y positive value moves head up, negative value moves camera head down
     * @param z positive value zoom in, negative value zoom out
     */
    public void moveCamera(int x, int y, int z) {
    	if ((x != 0) || (y != 0) || (z != 0))
    		mGLView.moveCamera(x, y, z);
    }
    
    /**
     * set the distance of canmera position
     * @param distance
     */
    public void setCamDistance(float distance) {
    	mGLView.setCamDistance(distance);
    }
    public void setCamDistance(float distance, float min_distance, float max_distance) {
    	mGLView.setCamDistance(distance, min_distance, max_distance);
    }
    
    public void setCameraHeight(float height) {
    	mGLView.setCameraHeight(height);
    }
    
    public void setCameraPosition(float x, float y, float z) {
    	mGLView.setCameraPosition(x, y, z);
    }
    
    public void setCameraPosition(Vector3 position) {
    	if (position != null)
    		mGLView.setCameraPosition(position);
    }
    
    public void clientResetScene() {
    	mGLView.clientResetScene();
    }
    
    /**
     * enable/disable texturing on rendering
     * @param enable
     */
    public void setTexturing(boolean enable) {
    	mGLView.setTexturing(enable);
    }
    
    /**
     * set whether or not to render shadows</br>
     * sorry this function does not work well now.
     * @param visible
     */
    public void setDrawShadows(boolean drawShadows) {
    	mGLView.setDrawShadows(drawShadows);
    }

    /**
     * set whether or not to render clusters</br>
     * @param enable
     */
    public void setDrawClusters(boolean drawClusters) {
    	mGLView.setDrawClusters(drawClusters);
    }

    /**
     * shoot box from camera position to the specific position
     * @param screen_x destination position to shoot
     * @param screen_y destination position to shoot
     */
    public void shootBox(int screen_x, int screen_y) {
    	mGLView.shootBox(screen_x, screen_y);
    }

    /**
    * set speed of shooting box
    * @param speed shoot speed(initial:40f)
     */
   public void setShootSpeed(float speed) {
    	mGLView.setShootSpeed(speed);
    }
    /**
     * returns whether or not to pick object now
     * @return true: picking object now
     */
    public boolean isPicking() {
    	return mGLView.isPicking();
    }
    
    /**
     * set object tracking mode on/off</br>
     * if tracking mode is on(rigidBody not null), you cannot move camera by yourself
     * @param rigidBody tracking object, tracking mode is off when this parameter is null
     * @param minDistance minimum distance between camera and object
     * @param maxDistance maximum distance between camera and object
     */
    public void setTracking(RigidBody rigidBody, float minDistance, float maxDistance) {
    	mGLView.setTracking(rigidBody, minDistance, maxDistance);
    }

    /**
     * set current debugmode
     * @param mode
     */
    public void setDebugMode(int mode) {
    	mGLView.setDebugMode(mode);
    }
    
    /**
     * get current debugmode
     * @return
     */
    public int getDebugMode() {
    	return mGLView.getDebugMode();
    }   

	@Override
	public void freePhysics(GLBulletWorldSurfaceView view) {
	}

	@Override
	public void renderFrame(GL10 gl) {
	}

	@Override
	public boolean onLongTap(int screen_x, int screen_y) {
		return false;
	}

	@Override
	public boolean onDoubleTap(int screen_x, int screen_y) {
		return false;
	}

}
