// sound handler
// written by jared bruni
// www.lostsidedead.com

// open source, open mind

#include "thehead.h"


void SoundHandle::Play(int soundx)
{
	sound[soundx].Play();
}

bool SoundHandle::Status(int s)
{
	return sound[s].PlayStatus();
}

/*************************************************************************** lostsidedead.com */