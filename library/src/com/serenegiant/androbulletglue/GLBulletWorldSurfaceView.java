/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: GLBulletWorldSurfaceView.java
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

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.serenegiant.androbulletglue.math.Vector3;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;
import android.view.ScaleGestureDetector;
import android.view.ScaleGestureDetector.OnScaleGestureListener;

public class GLBulletWorldSurfaceView extends GLSurfaceView {
	static {
		BulletManager.loadBulletLib();
	};

	private final String TAG = getClass().getSimpleName();
	
	public static final int DBG_NODEBUG = 0;
	public static final int DBG_DRAWWIREFRAME = 1;
	public static final int DBG_DRAWAABB = 2;
	public static final int DBG_DRAWFEATURESTEXT = 4;
	public static final int DBG_DRAWCONTACTPOINTS = 8;
	public static final int DBG_NODEACTIVATION = 16;
	public static final int DBG_NOHELPTEXT = 32;
	public static final int DBG_DRAWTEXT = 64;
	public static final int DBG_PROFILETIMINGS = 128;
	public static final int DBG_ENABLESATCOMPARISON = 256;
	public static final int DBG_DISABLEBULLERLCP = 512;
	public static final int DBG_ENABLECCD = 1024;
	public static final int DBG_DRAWCONSTRAINTS = (1 << 11);
	public static final int DBG_DRAWCONSTRAINTLIMITS = (1 << 12);
	public static final int DBG_FASTWIREFRAME = (1<<13);
	public static final int DBG_DRAWNORMALS = (1<<14);

	public static final int TOUCH_UP = 0;
	public static final int TOUCH_DOWN = 1;
	public static final int TOUCH_MOVE = 2;

// flags for object picking mode when calls #setPickMode
	public static final int PICKMODE_HANG = 1;		// pick object
	public static final int PICKMODE_TRACKING = 2;	// track picking object with camera
// internal request flags to renderer thread
	private static final int RENDERER_MODIFY_TEXTURING = 0x01;
	private static final int RENDERER_MODIFY_SHADOWS = 0x02;
	private static final int RENDERER_MODIFY_CLUSTERS = 0x04;
	private static final int RENDERER_MODIFY_DEBUGMODE_TOGGLE = 0x08;
	private static final int RENDERER_MODIFY_DEBUGMODE_SET = 0x10;
	private static final int RENDERER_MODIFY_CAMERA_MOVE = 0x20;
	private static final int RENDERER_MODIFY_CAMERA_DISTANCE = 0x40;
	private static final int RENDERER_MODIFY_CAMERA_DISTANCE3 = 0x80;
	private static final int RENDERER_MODIFY_CAMERA_HEIGHT = 0x100;
	private static final int RENDERER_MODIFY_CAMERA_POSITION = 0x200;
	private static final int RENDERER_MODIFY_CAMERA_TRACKING = 0x400;
	
	private static final int SIMULATE_CLIENTRESETSCENE = 0x01;
	private static final int SIMULATE_SHOOT = 0x02;
	private static final int SIMULATE_SET_SHOOTSPEED = 0x04;
	
    private GLBulletWorldSurfaceView.GLBulletWorldSurfaceViewCallback mCallback;
    private GestureDetector mSingleTouchGestureDetector;
    private ScaleGestureDetector mMultiTouchGestureDetector;
//	private BulletManager mBulletManager;
	private PhysicsWorld mWorld;
	private long mPhysicsWorldId;
	private int mViewWidth, mViewHeight;
	
	private final Object mSyncObj = new Object();
	private float mExecTime = 0.f;	// 1 / 60f;
	private int mSubSteps = 10;
	private boolean mIsEnabled;
	private boolean mIsActive;
	private boolean mNeedDraw;
	private boolean mHasObjects;
// RENDERER	
	private int mRendererFlag;
	private int mMoveX, mMoveY, mMoveZ;
	private float mCameraDistance, mCameraDistanceMin, mCameraDistanceMax;
	private float mCameraHeight;
	private float mCameraX, mCameraY, mCameraZ;
	private float mTargetX, mTargetY, mTargetZ;
	private long mTrackingBodyID;
	private boolean mIsTexturing;
	private boolean mDrawShadows;
	private boolean mDrawClusters;
	private int mDebugMode;
// SIMULATE	
	private int mSimulateFlag;
	private boolean mNeedReset;
	private int mShootX, mShootY;
	private float mShootSpeed;
	private int mTouchX, mTouchY; 

	public interface GLBulletWorldSurfaceViewCallback {
		/**
		 * you will create and initialize physicsWorld in this method</br>
		 * if you return null, automatically create default PhysicsWorld with -9.8 of gravity
		 * @param manager
		 * @param view
		 */
		public PhysicsWorld initPhysics(GLBulletWorldSurfaceView view);
		/**
		 * destroy physics world(normally you don't need to do anything)
		 * @param manager
		 * @param view
		 * @return PhysicsWorld
		 */
		public void freePhysics(GLBulletWorldSurfaceView view);
		/**
		 * you will create your physics objects in this method
		 * @param world
		 */
		public void createObjects(PhysicsWorld world);
		/**
		 * free your physics objects(normally you don't need to do anything, automatically free objects)
		 */
		public void destroyObjects(PhysicsWorld world);
		/**
		 * additional rendering by yourself after standard rendering</br>
		 * this method is called by GL thread and direct UI access is not allowed
		 */
		public void renderFrame(GL10 gl);
		/**
		 * callback when long tapping
		 * @param screen_x
		 * @param screen_y
		 * @return
		 */
		public boolean onLongTap(int screen_x, int screen_y);
		/**
		 * callback when double tapping
		 * @param screen_x
		 * @param screen_y
		 * @return
		 */
		public boolean onDoubleTap(int screen_x, int screen_y);
	}
	
    private final GLBulletWorldRenderer mRenderer;
    private SimulationThread mSimulationThread;

    /**
     * default constructor
     * @param context
     */
	public GLBulletWorldSurfaceView(Context context) {
		this(context, null);
	}

	public GLBulletWorldSurfaceView(Context context, GLBulletWorldSurfaceViewCallback callback) {
		super(context);
		mCallback = callback;
		mRenderer = new GLBulletWorldRenderer();
		setRenderer(mRenderer);
		// set render mode to RENDERMODE_WHEN_DIRTY (only render when anything are modified)
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		setFocusableInTouchMode(true);
		requestFocus();
		final OnSingleTouchListener singleTouchlistener = new OnSingleTouchListener();
		mSingleTouchGestureDetector = new GestureDetector(context, singleTouchlistener);
     	mSingleTouchGestureDetector.setOnDoubleTapListener(singleTouchlistener);
     	final OnMultiTouchListener multiTouchListener = new OnMultiTouchListener();
     	mMultiTouchGestureDetector = new ScaleGestureDetector(context, multiTouchListener);
	}

	@Override
    public void onPause() {
    	synchronized (mSyncObj) {
//    		mIsActive = false;
    		mViewWidth = mViewHeight = 0;
    	}
		if (mSimulationThread != null) {
			mSimulationThread.terminate();
			mSimulationThread = null;
		}
		super.onPause();		// stop GL Renderer thread 
    }

	public void onDispose() {
		callDestroyObjects();
		callFreePhysics();
	}
	
	@Override
    public void onResume() {
		if (mSimulationThread == null) {
			mSimulationThread = new SimulationThread();
			mSimulationThread.start();
		}
		mNeedDraw = true;
		super.onResume();		// start GL Renderer thread
    }

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (mInMultiTouch || (event.getPointerCount() > 1))	// multiple touch?
			return mMultiTouchGestureDetector.onTouchEvent(event);
		else
			return mSingleTouchGestureDetector.onTouchEvent(event);
	}

	private boolean mPicked = false;
	private boolean mInMultiTouch = false;
	private int mPrevX, mPrevY;
	private void pickEnd() {
		if (mPicked) {
			processTouch(TOUCH_UP, mPrevX, mPrevY);
			mPicked = false;
		}
	}
	
    private final class OnSingleTouchListener implements OnGestureListener, OnDoubleTapListener {
		@Override
		public boolean onDown(MotionEvent event) {
			// when tap occurs(called every time)
			// this method should be return true for other event call
			return true;
		}

		@Override
		public void onShowPress(MotionEvent event) {
			// when tap occurs(not performed a move or up yet)
			mPicked = (processTouch(TOUCH_DOWN, mPrevX = (int)event.getX(), mPrevY = (int)event.getY()) != 0);
		}

		@Override
		public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY) {
			// fling event
			pickEnd();
			return true;
		}

		@Override
		public void onLongPress(MotionEvent event) {
			// long tap event
			pickEnd();
			boolean b = false;
			if (mCallback != null) {
				try {
					b = mCallback.onLongTap((int)event.getX(), (int)event.getY());
				} catch (Exception e) {
					Log.w(TAG, "onLongPress:" + e);
				}
			}
			if (!b) {
//				resetWorld();	// XXX do you need default action?
			}
		}

		@Override
		public boolean onScroll(MotionEvent event1, MotionEvent event2, float distanceX, float distanceY) {
			// when scroll occurs 
			if (mPicked) {
				mPicked = processTouch(TOUCH_MOVE, mPrevX = (int)event2.getX(), mPrevY = (int)event2.getY()) != 0;
			}
			if (!mPicked) {
				float dx = distanceX / 10f;
				float dy = distanceY / 10f;
				if (Math.abs(dx) > Math.abs(dy)) dy = 0f;
				else dx = 0f;		
				moveCamera((int)dx, (int)dy, 0);
			}
			return true;
		}

		@Override
		public boolean onSingleTapUp(MotionEvent event) {
			// called when tap occurs(called when double tapped)
			pickEnd();
			return true;
		}

		@Override
		public boolean onSingleTapConfirmed(MotionEvent event) {
			// called when tap occurs(not called when double tapped)
			shootBox((int)event.getX(), (int)event.getY());
			return true;
		}

		@Override
		public boolean onDoubleTap(MotionEvent event) {
			// called when double tap occurs
			boolean b = false;
			if (mCallback != null) {
				try {
					mCallback.onDoubleTap((int)event.getX(), (int)event.getY());
				} catch (Exception e) {
					Log.w(TAG, "onDoubleTap:" + e);
				}
			}
			if (!b)
				setActive(!getActive());
			return true;
		}

		@Override
		public boolean onDoubleTapEvent(MotionEvent event) {
			// when double tap occurs
			return false;
		}
    };

    private final class OnMultiTouchListener implements OnScaleGestureListener {
		@Override
		public boolean onScaleBegin(ScaleGestureDetector detector) {
			// start scaling
			pickEnd();
			mInMultiTouch = true;
			// this method should return true when you need scaling.
			// if this method returns false, no onScale and onScaleEnd are called.
			return true;
		}

		@Override
		public boolean onScale(ScaleGestureDetector detector) {
			// when scale event occurs
			final float factor = detector.getScaleFactor();
			moveCamera(0, 0, factor > 1f ? 1 : (factor < 1f ? -1 : 0));
			return true;
		}

		@Override
		public void onScaleEnd(ScaleGestureDetector detector) {
			// terminate scaling
			mInMultiTouch = false;
		}
    };

    /**
     * get default PhysicsWorld instance
     * @return null if no instance present
     */
    protected PhysicsWorld getPhysicsWorld() {
    	return mWorld;
    }
	/**
	 * set executing interval time of current simulation
	 * @param execTime
	 */
	protected void setExecTime(float execTime) {
    	synchronized (mSyncObj) {
    		mExecTime = execTime;
    	}
    }

    /**
     * get executing interval time of current simulation
     * @return
     */
    protected float getExecTime() {
    	return mExecTime;
    }
    
    /**
     * set sub step(s) of current simulation
     * @param subSteps
     */
    protected void setSubSteps(int subSteps) {
    	synchronized (mSyncObj) {
    		mSubSteps = subSteps;
    	}
    }
    
    /**
     * get sub step(s) of current simulation
     * @return
     */
    protected int getSubSteps() {
    	return mSubSteps;
    }
    
    /**
     * activate/deactivate simulation
     * @param active
     */
	protected void setActive(boolean active) {
      	synchronized (mSyncObj) {
       		mIsActive = active;
       		mSyncObj.notifyAll();
     	}
	}
   
    /**
     * get current active state of simulation
     * @return
     */
    protected boolean getActive() {
    	return mIsActive;
    }
    
    /**
     * reset simulation
     */
    public void resetWorld() {
    	synchronized (mSyncObj) {
    		mNeedReset = true;
    	}
    }
    
    /**
     * set callback method
     * @param callBack
     */
    public void setCallBack(GLBulletWorldSurfaceViewCallback callBack) {
    	synchronized (mSyncObj) {
    		mCallback = callBack;
    	}
    }
    
    /**
     * move camera
     * @param x turn left(-)/right(+)
     * @param y turn down(-)/up(+)
     * @param z zoom out(-)/in(+)
     */
    protected void moveCamera(int x, int y, int z) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_CAMERA_MOVE;
    		mMoveX = x;
    		mMoveY = y;
    		mMoveZ = z;
    	}
    	requestRender();
    }
    
    /**
     * set distance between camera and target position
     * @param distance
     */
    protected void setCamDistance(float distance) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_CAMERA_DISTANCE;
    		mCameraDistance = distance;
    	}
    	requestRender();
    }

    /**
     * set distance between camera and target position
     * @param distance
     * @param min_distance
     * @param max_distance
     */
    protected void setCamDistance(float distance, float min_distance, float max_distance) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_CAMERA_DISTANCE3;
    		mCameraDistance = distance;
    		mCameraDistanceMin = min_distance;
    		mCameraDistanceMax = max_distance;
    	}
    }
    
    /**
     * set camera height
     * @param height
     */
    protected void setCameraHeight(float height) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_CAMERA_HEIGHT;
    		mCameraHeight = height;
    	}
    }
    
    /**
     * set camera position
     * @param x
     * @param y
     * @param z
     */
    protected void setCameraPosition(float x, float y, float z) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_CAMERA_POSITION;
    		mCameraX = x;
    		mCameraY = y;
    		mCameraZ = z;
    	}
    }
    
    /**
     * set camera position
     * @param position
     */
    protected void setCameraPosition(Vector3 position) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_CAMERA_POSITION;
    		mCameraX = position.x();
    		mCameraY = position.y();
    		mCameraZ = position.z();
    	}
    }
    
    /**
     * set object tracking mode on/off</br>
     * if tracking mode is on(rigidBody not null), you cannot move camera by yourself
     * @param rigidBody tracking object, tracking mode is off when this parameter is null
     * @param minDistance minimum distance between camera and object
     * @param maxDistance maximum distance between camera and object
     */
    protected void setTracking(RigidBody rigidBody, float minDistance, float maxDistance) {
    	if (rigidBody != null && rigidBody.getID() > 0) {
	    	synchronized (mSyncObj) {
	    		mRendererFlag |= RENDERER_MODIFY_CAMERA_TRACKING;
	    		mTrackingBodyID = rigidBody.getID();
	    		mCameraDistanceMin = minDistance;
	    		mCameraDistanceMax = maxDistance;
	    	}
    	}
    }


    /**
     * set texturing mode enabled/disabled
     * @param enable
     */
    protected void setTexturing(boolean enable) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_TEXTURING;
        	mIsTexturing = enable;
    	}
    	requestRender();    	
    }
    
    /**
     * set visivility of shadows 
     * @param drawShadows
     */
    protected void setDrawShadows(boolean drawShadows) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_SHADOWS;
        	mDrawShadows = drawShadows;
    	}
    	requestRender();    	
    }

    /**
     * set visivility of clusters 
     * @param visible
     */
    protected void setDrawClusters(boolean drawClusters) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_CLUSTERS;
        	mDrawClusters = drawClusters;
    	}
    	requestRender();    	
    }
    
    /**
     * toggle debugmode
     * @param mode use GLBulletWorldSurfaceView#DBG_xxx constants
     */
    protected void toggleDebugMode(int mode) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_DEBUGMODE_TOGGLE;
    		mDebugMode = mode;
    	}
    	requestRender();    	
    }

    /**
     * set debugmode
     * @param mode use GLBulletWorldSurfaceView#DBG_xxx constants
     */
    protected void setDebugMode(int mode) {
    	synchronized (mSyncObj) {
    		mRendererFlag |= RENDERER_MODIFY_DEBUGMODE_SET;
    		mDebugMode = mode;
    	}
    	requestRender();    	
    }
    
    /**
     * get current debugmode
     * @return
     */
    protected int getDebugMode() {
    	synchronized (mSyncObj) {
    		return nativeGetDebugMode(mPhysicsWorldId);
    	}
    }
    
    /**
     * shoot box from camera position to the specific position
     * @param screen_x destination position to shoot
     * @param screen_y destination position to shoot
     */
    protected void shootBox(int screen_x, int screen_y) {
    	synchronized (mSyncObj) {
  			mSimulateFlag |= SIMULATE_SHOOT;
 			mShootX = screen_x;
 			mShootY = screen_y;
    	}
    }
    
    /**
     * set speed of shooting box
     * @param speed shoot speed(initial:40f)
     */
    protected void setShootSpeed(float speed) {
    	if (speed > 0.f)
    	synchronized (mSyncObj) {
    		mSimulateFlag |= SIMULATE_SET_SHOOTSPEED;
    		mShootSpeed = speed;
    	}
    }
    
    protected void clientResetScene() {
    	synchronized (mSyncObj) {
    		mSimulateFlag |= SIMULATE_CLIENTRESETSCENE;
    	}
    }
    
    /**
     * notify touch event to GLBulletWorldSurfaceView/simulating framework
     * @param state		TOUCH_UP/TOUCH_DOWN_TOUCH_MOVE
     * @param screen_x	screen position x
     * @param screen_y	screen position y
     */
    protected int processTouch(int state, int screen_x, int screen_y) {
    	synchronized (mSyncObj) {
    		return nativeProcessTouch(mPhysicsWorldId, state, screen_x, screen_y);
    	}
    }
    
    /**
     * set picking mode of object
     * @param mode PICKMODE_HANG/PICKMODE_TRACKING
     */
    protected void setPickMode(int mode) {
    	synchronized (mSyncObj) {
    		nativeSetPickMode(mPhysicsWorldId, mode);
    	}
    }
    
    /**
     * returns whether or not to pick object now
     * @return true: picking object now
     */
    protected boolean isPicking() {
    	synchronized (mSyncObj) {
    		return nativeIsPicking(mPhysicsWorldId);
    	}
    }
    
	/**
	 * internal initialization of physicsworld
	 */
	private void callInitPhysics() {
		synchronized (mSyncObj) {
			BulletManager bulletManager = BulletManager.getBulletManager();
			if (bulletManager == null) {
				mIsActive = mIsEnabled = false;
				// create and initializae physics engine
				// the simulation result does not need to set to Java object
				bulletManager = BulletManager.initialize(false);
			}
			mWorld = bulletManager.getDefaultPhysicsWorld();
			if (mWorld == null) {
				if (mCallback != null) {
					try {
						mWorld = mCallback.initPhysics(this);
					} catch (Exception e) {
						Log.w("GLBulletWorldRenderer#callInitPhysics:", e);
					}
				}
				if (mWorld == null) {
					mWorld = new PhysicsWorld(-9.8f);
				}
			}
			mPhysicsWorldId = mWorld.getID(); 
			mIsEnabled = (mPhysicsWorldId != 0);
		}
	}
	
	/**
	 * create physics world objects
	 */
	private void callCreateObjects() {
		if (!mHasObjects) {
			mHasObjects = true;
			if (mCallback != null) {
				try {
					mCallback.createObjects(mWorld);
				} catch (Exception e) {
					Log.w("GLBulletWorldRenderer#callCreateObjects:", e);
				}
			}
		}
	}
	
	/**
	 * destroy physics world objects
	 */
	private void callDestroyObjects() {
		if (mHasObjects) {
			if (mCallback != null) {
				try {
					mCallback.destroyObjects(mWorld);
				} catch (Exception e) {
					Log.w("GLBulletWorldRenderer#callDestroyObjects:", e);
				}
			}
		}
		mWorld.clearObjects();
		mHasObjects = false;
	}
	
	/**
	 * dispose physics world
	 */
	private void callFreePhysics() {
		synchronized (mSyncObj) {
			mHasObjects = mIsActive = mIsEnabled = false;
		}
		if (mCallback != null) {
			try {
				mCallback.freePhysics(this);
			} catch (Exception e) {
				Log.w("GLBulletWorldRenderer#callFreePhysics:", e);
			}
		}
		mWorld.dispose();
		mWorld = null;
		mPhysicsWorldId = 0;
	}

	/**
	 * reset
	 */
	private void reset() {
		synchronized (mSyncObj) {
			final boolean b = mIsActive;
			callDestroyObjects();
//			callFreePhysics();	// release physics
//			callInitPhysics();	// then restart again
//			nativeResize(mPhysicsWorldId, mViewWidth, mViewHeight);
			callCreateObjects();
			setDebugMode(mDebugMode);
			setActive(b);
		}
	}

    private static final native void nativeResize(long physicsWorldId, int w, int h);
    private static final native void nativeLostGLContext(long physicsWorldId);
    private static final native void nativeRender(long physicsWorldId);
    private static final native void nativeMoveCamera(long physicsWorldId, int x, int y, int z);
    private static final native void nativeSetCameraPosition(long physicsWorldId, float x, float y, float z);
    private static final native void nativeSetCameraDistance(long physicsWorldId, float distance);
    private static final native void nativeSetCameraDistanceMinMax(long physicsWorldId, float distance, float min_distance, float max_distance);
    private static final native void nativeSetCameraHeight(long physicsWorldId, float height);
    private static final native void nativeShootBox(long physicsWorldId, int screen_x, int screen_y);
    private static final native void nativeSetShootSpeed(long physicsWorldId, float speed);
    private static final native void nativeClientResetScene(long physicsWorldId);
    private static final native void nativeSetTexturing(long physicsWorldId, boolean enable);
    private static final native void nativeSetDrawShadows(long physicsWorldId, boolean drawShadows);
    private static final native void nativeSetDrawClusters(long physicsWorldId, boolean drawClusters);
    private static final native int nativeToggleDebugMode(long physicsWorldId, int mode);
    private static final native int nativeSetDebugMode(long physicsWorldId, int mode);
    private static final native int nativeGetDebugMode(long physicsWorldId);
    private static final native int nativeProcessTouch(long physicsWorldId, int state, int screen_x, int screen_y);
    private static final native void nativeSetPickMode(long physicsWorldId, int mode);
    private static final native boolean nativeIsPicking(long physicsWorldId);
    private static final native void nativeSetTracking(long physicsWorldId, long rigidBodyId, float minDistance, float maxDistance);
    
    // callbacks of GLSurfaceView
    private final class GLBulletWorldRenderer implements GLSurfaceView.Renderer {
    	@Override
    	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
//    		Log.v(TAG, "onSurfaceCreated:mIsEnabled=" + mIsEnabled +", mIsActive=" + mIsActive + ", mPhysicsWorldId=" + mPhysicsWorldId);
    		if (mPhysicsWorldId != 0)
    			nativeLostGLContext(mPhysicsWorldId);
    		mPhysicsWorldId = 0;
	    	callInitPhysics();
        }

    	@Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
/*    		Log.v(TAG, "onSurfaceChanged:mIsEnabled=" + mIsEnabled
    			+ ", mIsActive=" + mIsActive + ", mPhysicsWorldId=" + mPhysicsWorldId
    			+ ", height=" + height + " ,width=" + width); */
    		if ((width == 0) || (height == 0)) return;
    		mViewWidth = width;
    		mViewHeight = height;
			nativeResize(mPhysicsWorldId, width, height);
			synchronized (mSyncObj) {
				if (mIsEnabled && !mHasObjects && (mPhysicsWorldId != 0)) {
	   				callCreateObjects();
	   				mIsActive = true;
				}
			}
        }

    	@Override
        public void onDrawFrame(GL10 gl) {
    		synchronized (mSyncObj) {
    			if (mIsEnabled) {
    				// these methods affect to the current rendering
	    			if ((mRendererFlag & RENDERER_MODIFY_CAMERA_MOVE) != 0)
	    				nativeMoveCamera(mPhysicsWorldId, mMoveX, mMoveY, mMoveZ);
	    			if ((mRendererFlag & RENDERER_MODIFY_CAMERA_DISTANCE) != 0)
	    				nativeSetCameraDistance(mPhysicsWorldId, mCameraDistance); 
	    			if ((mRendererFlag & RENDERER_MODIFY_CAMERA_DISTANCE3) != 0)
	    				nativeSetCameraDistanceMinMax(mPhysicsWorldId, mCameraDistance, mCameraDistanceMin, mCameraDistanceMax); 
	    			if ((mRendererFlag & RENDERER_MODIFY_CAMERA_HEIGHT) != 0)
	    				nativeSetCameraHeight(mPhysicsWorldId, mCameraHeight);
	    			if ((mRendererFlag & RENDERER_MODIFY_CAMERA_POSITION) != 0)
	    				nativeSetCameraPosition(mPhysicsWorldId, mCameraX, mCameraY, mCameraZ);
	    			if ((mRendererFlag & RENDERER_MODIFY_CAMERA_TRACKING) != 0)
		    			nativeSetTracking(mPhysicsWorldId, mTrackingBodyID, mCameraDistanceMin, mCameraDistanceMax);
	    			
	    			if ((mRendererFlag & RENDERER_MODIFY_TEXTURING) != 0)
	    				nativeSetTexturing(mPhysicsWorldId, mIsTexturing);
	    			if ((mRendererFlag & RENDERER_MODIFY_SHADOWS) != 0)
	    				nativeSetDrawShadows(mPhysicsWorldId, mDrawShadows);
	    			if ((mRendererFlag & RENDERER_MODIFY_CLUSTERS) != 0)
	    				nativeSetDrawClusters(mPhysicsWorldId, mDrawClusters);
	    			if ((mRendererFlag & RENDERER_MODIFY_DEBUGMODE_TOGGLE) != 0)
	    				nativeToggleDebugMode(mPhysicsWorldId, mDebugMode);
	    			if ((mRendererFlag & RENDERER_MODIFY_DEBUGMODE_SET) != 0)
	    				nativeSetDebugMode(mPhysicsWorldId, mDebugMode);
	    			mRendererFlag = 0;
	    			// rendering
	    			nativeRender(mPhysicsWorldId);
	    			if (mCallback != null) {
	    				try {
	    					mCallback.renderFrame(gl);
	    				} catch (Exception e) {
	    					Log.w("GLBulletWorldRenderer#onDrawFrame:renderFrame:", e);
	    				}
	    			}
    			}
    			mNeedDraw = false;
    			mSyncObj.notifyAll();
    		}
        }       
    }

    /**
     * physics simulation thread
     */
    private final class SimulationThread extends Thread {
    	private boolean isRunning;
    	@Override
    	public void run() {
    		isRunning = true;
    		while (isRunning) {
    			if (mNeedReset) {
    				mNeedReset = false;
    				reset();
    			}
    			if (mIsActive) {	// only execute when active
            		synchronized (mSyncObj) {
            			// these methods do not affect to the calculated simulation result
            			// affect to the next calculation 
	        			if ((mSimulateFlag & SIMULATE_SET_SHOOTSPEED) != 0)
	        				nativeSetShootSpeed(mPhysicsWorldId, mShootSpeed);
	        			if ((mSimulateFlag & SIMULATE_SHOOT) != 0)
	        				nativeShootBox(mPhysicsWorldId, mShootX, mShootY);
	        			if ((mSimulateFlag & SIMULATE_CLIENTRESETSCENE) != 0)
	        				nativeClientResetScene(mPhysicsWorldId);
	        			mSimulateFlag = 0;
            		}
               		synchronized (mSyncObj) {
               			if (mNeedDraw) {
                   			// Previous drawing is not finished / still progress
   	            			try {
   	            				mSyncObj.wait();
   	    					} catch (InterruptedException e) {
   	    					}
   	   						if (!isRunning) break;
               			}
               			// simulate
   	        			if (mExecTime <= 0f) mWorld.doSimulation();
   	        			else				 mWorld.doSimulation(mExecTime, mSubSteps);
   	        			// request to render
            			mNeedDraw = true;
	    				GLBulletWorldSurfaceView.this.requestRender();
            		}
    			} else {
            		synchronized (mSyncObj) {
	    				try {				// wait when not active
	    					mSyncObj.wait(25);
						} catch (InterruptedException e) {
						}
            		}
					if (!isRunning) break;
    			}
    		}
    	}
    	
    	public void terminate() {
			isRunning = false;
			synchronized (mSyncObj) {
				mSyncObj.notifyAll();
			}
			while (true) {
				try {
					join();
					break;
				} catch (InterruptedException e) {
					// retry
				}
			}
    	}
    }    
}

