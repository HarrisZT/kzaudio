/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzmusicstream.h 										              |
| Desc: interface for managing a music stream  			                      |
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
#ifndef __KZMUSICSTREAM_H__ 
#define __KZMUSICSTREAM_H__ 

#include "kzaudiofile.h"
namespace kz {


	/**
	interface for managing a music stream*/
	class MusicStream final : NonCopyable {
	public:

		/** construct music instance*/
		MusicStream();
		~MusicStream();


		/** open the given music file
			@return: true if loaded successfully, else false*/
		Bool Load(const String& filename);

		/** update the music stream. this function must
			be called once during the main program loop*/
		Void Update();


		/** start (resume) playback of music stream*/
		Void Play();

		/** pause playback of music stream at the current point*/
		Void Pause();

		/** halt playback of music stream (resets stream to beginning)*/
		Void Stop();

		/** start (resume) playback of music stream*/
		Void Resume();


		/** returns true if music is playing else false*/
		Bool IsPlaying() const;

		/** returns true if music is paused else false*/
		Bool IsPaused() const;


		/** set the music stream's volume*/
		Void SetVolume(Float volume);

		/** returns the music stream's volume*/
		Float GetVolume() const;


		/** set whether the stream loops after reaching the end
			(this is enabled by default)*/
		Void SetLoopEnabled(Bool loop);

		/** returns true if the stream loops after reaching the end*/
		Bool IsLoopEnabled() const;


	private:
		enum { STREAMFRAGMENTS = 5 };

		Bool FillBufferQueue(Uint32 buffer);

		Uint32     m_alsource;
		Float      m_gain;
		Float      m_volume;
		AudioFile  m_file;
		Uint32     m_buffers[STREAMFRAGMENTS];
		Bool       m_loopEnabled;
		Int32      m_format;
		Uint32     m_buffersize;
		SAMPLEDATA m_bufferdata;
		Uint32     m_sampleRate;
	};
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/