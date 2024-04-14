/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzaudiointernal.h 												 	  |
| Desc: common headers, types, etc used by audio system	                      |
|     																		  |
| This program is free software: you can redistribute it and/or modify		  |
| it under the terms of the GNU General Public License as published by		  |
| the Free Software Foundation, either version 3 of the License, or			  |
| (at your option) any later version.										  |
| 																			  |
| This program is distributed in the hope that it will be useful,			  |
| but WITHOUT ANY WARRANTY; without even the implied warranty of			  |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				  |
| GNU General Public License for more details.								  |
| 																			  |
| You should have received a copy of the GNU General Public License			  |
| along with this program.  If not, see <http://www.gnu.org/licenses/>.		  |
******************************************************************************/
#ifndef __KZAUDIOINTERNAL_H__
#define __KZAUDIOINTERNAL_H__
 
#include <unordered_set> 
#include "kztimevalue.h"
#include "kziobuf.h" 
namespace kz {


	class AudioDecoder;
	class AudioDecoderOGG;
	class AudioDecoderWAV;
	class AudioDevice;
	class AudioFile;
	class MusicStream;
	class Sound;
	class SoundBuffer;

	typedef std::vector<Int16> SAMPLEDATA;
	typedef const Int16*       SAMPLESPTR;

	struct Chunk {
		SAMPLESPTR samples;     //Pointer to the audio samples
		SizeT      sampleCount; //Number of samples pointed by Samples
	};
	 

	/**
	contains descriptive info about audio*/
	struct AudioDesc {
		Uint32     nchannels;    //channel count. 1 (mono), 2 (stereo) 
		TimeValue  length;       //total duration of the audio
		Bool       loopEnabled;  //stream is looping?
		TimeValue  loopStart;    //The beginning offset 
		TimeValue  loopEnd;      //The length of the loop
		TimeValue  position;     //current playing position (from beginning) 
		SAMPLESPTR samples;      //array of audio samples stored in the buffer
		Uint64     sampleCount;  //total number of audio samples in the file 
		Uint64     sampleOffset; //read offset of the file in samples
		Uint32     sampleRate;   //sample rate of the stream 
	};


	/** ensures volume is within range [0-100]*/
	inline Void ClampVolume(Int32& volume) {
		if (volume < 0)
			volume = 0;
		else if (volume > 100)
			     volume = 100;
	}
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/