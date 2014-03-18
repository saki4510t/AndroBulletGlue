/*
 * AndroBulletGlue
 * Bullet Continuous Collision Detection and Physics Library for Android
 * Copyright(c) 2013,2014 t_saki@serenegiant
 * original Bullet version is 2.81-rev2613 => changed to 2.82-rev2704
 * 
 * File name: NativeObject.java
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

package com.serenegiant.androbulletglue.interfaces;

import com.serenegiant.androbulletglue.BulletManager;

/**
 * the base class that has native pointer
 */
public abstract class NativeObject {
	static {
		BulletManager.loadBulletLib();
	};

	// pointer value for native object
	// native code access this field and you should not remove and rename
	protected long mID;
	// whether or not this object reference other native object
	// XXX it is better to change using reference counter
	protected boolean isAsigned = false;

	protected NativeObject() {
		mID = 0;
	}

	public void dispose() {
		if (mID != 0 && !isAsigned) {
			nativeDestroy(mID);
			mID = 0;
			isAsigned = false;
		}
	}
	protected abstract void nativeDestroy(long id_native);
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public long getID() {
		return mID;
	}
	
}
