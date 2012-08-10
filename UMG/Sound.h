#ifndef _SOUND_H_
#define _SOUND_H_

#include <vector>
#include <string>
#include <stdlib.h>
#include "al.h"
#include "alc.h"
#include "Framework.h"
#include "CWaves.h"
#include <iostream>


using namespace std;

class Sound
{
	// Buffers hold sound data.
	ALuint uiBuffer;

	// Sources are points emitting sound.
	ALuint uiSource;

	ALint iState;

public:
	Sound(char* filename) {
		// Load wav data into a buffer.
		alGenBuffers( 1, &uiBuffer );
		if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath((ALchar*)filename), uiBuffer))
		{
			cout << "Didn't load wav file" << endl;
		}
		alGenSources( 1, &uiSource );
		alSourcei( uiSource, AL_BUFFER, uiBuffer );
	}
	~Sound(void){}

	void play() {
		alSourcePlay(uiSource);
		//alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
		//while (iState == AL_PLAYING);
		//alSourceStop(uiSource);
	}

	void stop() {
		alSourceStop(uiSource);
	}

	void loop() {
		alSourcei(uiSource, AL_LOOPING, AL_TRUE);
	}

	bool isPlaying() {
		alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		return (iState == AL_PLAYING);
	}

	void setPosition(ALfloat* pos) {
		alSourcefv(uiSource, AL_POSITION, pos);
	}

	void setVelocity(ALfloat* vel) {
		alSourcefv(uiSource, AL_VELOCITY, vel);
	}
};

class SoundContainer
{
	vector<Sound*> list;

public:
	SoundContainer()
	{
		ALFWInit();

		if (!ALFWInitOpenAL())
		{
			ALFWShutdown();
		}
		list = vector<Sound*>();
	}

	~SoundContainer(){ }

	void add(char* file)
	{
		Sound* s = new Sound(file);
		list.push_back(s);
	}

	Sound* get( int i )
	{
		if(list.size() < (unsigned int)i) return 0;
		return list.at(i);
	}

	void clean( void )
	{
				
		ALFWShutdownOpenAL();

		ALFWShutdown();
	}

};

#endif
