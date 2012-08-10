#ifndef _FRAMETIMER_H_
#define _FRAMETIMER_H_

//FrameTimer now owns frame count

class FrameTimer
{
	unsigned int startFrame;
	unsigned int framesPerSecond;
	unsigned int frame;

public:
	FrameTimer() {
		framesPerSecond = 64;
		startFrame = 0;
	}

	void startTime( void ) {
		frame = 0;
	}
	//second since start time
	float getTime( void ) {
		return (((float)frame - (float)startFrame)/(float)framesPerSecond);
	}

	void incrFrame( void ) {
		frame++;
	}
	~FrameTimer(void){}
};

#endif