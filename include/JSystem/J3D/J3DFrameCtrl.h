#ifndef _J3DFRAMECTRL_H
#define _J3DFRAMECTRL_H

#include "types.h"

struct J3DFrameCtrl
{
	J3DFrameCtrl() { init(0); }

	virtual ~J3DFrameCtrl() {} // _08 (weak)

	void init(s16);
	void update();

	void init(int rate) { init((s16)rate); }

	void reset()
	{
		mFrame = mStartFrame;
		mRate = 1.0f;
		mState = 0;
	}

	void setRate(f32 rate) { mRate = rate; }
	void setFrame(f32 frame) { mFrame = frame; }
	f32 getFrame() const { return mFrame; }
	s16 getEnd() const { return mEndFrame; }

	void setAttribute(u8 attr) { mAttr = attr; }

	// _00 VTBL
	u8 mAttr;		 // _04
	u8 mState;		 // _05
	s16 mStartFrame; // _06
	s16 mEndFrame;	 // _08
	s16 mLoop;		 // _0A
	f32 mRate;		 // _0C
	f32 mFrame;		 // _10
};

#endif
