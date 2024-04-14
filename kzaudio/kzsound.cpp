/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzsound.cpp 												          |
| Desc: interface playing sound effects 			                          |
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
#include <al/al.h>
#include <al/alc.h>   
#include "kzsoundbuffer.h"
#include "kzsound.h" 
namespace kz {



	Sound::Sound() {
		m_buffer = NULL;
		alGenSources(1, &m_alSourceId);
		alSourcei(m_alSourceId, AL_BUFFER, 0);
	}

	Sound::Sound(const SoundBuffer* buffer) {
		SetBuffer(buffer);
	}



	Sound::Sound(const Sound& copy) {
		alGenSources(1, &m_alSourceId);
		alSourcei(m_alSourceId, AL_BUFFER, 0);
		SetVolume(copy.GetVolume());
		if (copy.m_buffer) {
			SetBuffer(copy.m_buffer);
		}
	}



	Sound& Sound::operator=(const Sound& copy) {
		if (this == &copy) {
			return *this;
		}
		SetVolume(copy.GetVolume());

		if (m_buffer) {
			Stop();
			m_buffer->m_registeredSounds.erase(this);
			m_buffer = NULL;
		}
		if (copy.m_buffer) {
			SetBuffer(copy.m_buffer);
		}
		return *this;
	}



	Sound::~Sound() {
		Stop();
		if (m_buffer) {
			m_buffer->m_registeredSounds.erase(this);
		}
		alSourcei(m_alSourceId, AL_BUFFER, 0);
		alDeleteSources(1, &m_alSourceId);
	}



	Void Sound::Play() {
		alSourcePlay(m_alSourceId);
	}

	Void Sound::Pause() {
		alSourcePause(m_alSourceId);
	}

	Void Sound::Stop() {
		alSourceStop(m_alSourceId);
	}



	Bool Sound::IsPlaying() const {
		Int32 status;
		alGetSourcei(m_alSourceId, AL_SOURCE_STATE, &status);
		return status == AL_PLAYING;
	}


	Bool Sound::IsPaused() const {
		Int32 status;
		alGetSourcei(m_alSourceId, AL_SOURCE_STATE, &status);
		return status == AL_PAUSED;
	}


	Bool Sound::IsStopped() const {
		Int32 status;
		alGetSourcei(m_alSourceId, AL_SOURCE_STATE, &status);
		return status == AL_STOPPED || status == AL_INITIAL;
	}



	Void Sound::SetMuted(Bool mute) {
		SetVolume(mute ? 0 : m_initialVolume);
	}


	Void Sound::SetVolume(Int32 volume) {
		ClampVolume(volume);
		alSourcef(m_alSourceId, AL_GAIN, (Float)volume * 0.01f);
	}


	Void Sound::SetInitialVolume(Int32 volume) {
		ClampVolume(volume);
		m_initialVolume = volume;
	}


	Int32 Sound::GetVolume() const {
		Float gain;
		alGetSourcef(m_alSourceId, AL_GAIN, &gain);
		return (Int32)(gain * 100.f);
	}



	const SoundBuffer* const Sound::GetBuffer() const {
		return m_buffer;
	}


	Void Sound::SetBuffer(const SoundBuffer* buffer) {
		if (m_buffer) {
			Stop();
			m_buffer->m_registeredSounds.erase(this);
		}
		m_buffer = buffer;
		m_buffer->m_registeredSounds.insert(this);
		alSourcei(m_alSourceId, AL_BUFFER, (Int32)m_buffer->m_bufferId);
	}



	Void Sound::ResetBuffer() {
		Stop();
		if (m_buffer) {
			alSourcei(m_alSourceId, AL_BUFFER, 0);
			m_buffer->m_registeredSounds.erase(this);
			m_buffer = NULL;
		}
	}
};
/******************************************************************************
//EOF                                                                         |
******************************************************************************/