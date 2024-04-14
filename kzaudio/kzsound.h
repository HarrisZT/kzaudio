/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzsound.h 													          |
| Desc: interface playing sound effects      		                          |
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
#ifndef __KZSOUND_H__ 
#define __KZSOUND_H__ 
 
#include "kzbasetypes.h" 
namespace kz {

	//source data of a sound 
	class SoundBuffer;



	/**
	interface playing sound effects*/
	class Sound final {
	public:
		Sound();
		Sound(const Sound&);
		Sound& operator=(const Sound&);
		explicit Sound(const SoundBuffer*);
		explicit Sound(SoundBuffer&&) = delete;
		Void SetBuffer(SoundBuffer&&) = delete;
		~Sound();


		/**	start or resume playback*/
		Void Play();

		/**	pause the audio source*/
		Void Pause();

		/**	stop playback of the audio source*/
		Void Stop();

		/** set the volume of the sound in the range [0, 100]*/
		Void SetVolume(Int32 volume);

		/**	get the volume of the sound in the range [0, 100]*/
		Int32 GetVolume() const;

		/** returns true if sound is playing else false*/
		Bool IsPlaying() const;

		/** returns true if sound is paused else false*/
		Bool IsPaused() const;

		/** returns true if sound is stopped else false*/
		Bool IsStopped() const;

		/**	mute or unmute the audio source.
			@param mute: true to mute, false to unmute*/
		Void SetMuted(Bool mute);

		/**	set the source buffer containing the audio data to play*/
		Void SetBuffer(const SoundBuffer* buffer);

		/**	get the audio buffer attached to the sound*/
		const SoundBuffer* const GetBuffer() const;

		/**	set the initial original volume for the sound, range [0-100]*/
		Void SetInitialVolume(Int32 volume);

		/**	this function intended for internal use only*/
		Void ResetBuffer();


	private:
		const SoundBuffer* m_buffer;
		Int32              m_initialVolume;
		Uint32             m_alSourceId;
	};
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/