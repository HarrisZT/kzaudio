/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzmusicstream.cpp 											          |
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
#include <al/al.h> 
#include "kzaudiodevice.h"
#include "kzmusicstream.h"
namespace kz {



	MusicStream::MusicStream() {
		m_gain        = 1.f;
		m_volume      = 1.f;
		m_loopEnabled = true;
		alGenSources(1, &m_alsource);
		alGenBuffers(STREAMFRAGMENTS, m_buffers);
	}



	MusicStream::~MusicStream() {
		Stop();
		alDeleteBuffers(STREAMFRAGMENTS, m_buffers);
		alDeleteSources(1, &m_alsource);
	}



	Void MusicStream::Play() {
		alSourcePlay(m_alsource);
	}

	Void MusicStream::Pause() {
		alSourcePause(m_alsource);
	}

	Void MusicStream::Resume() {
		if (IsPaused()) { Play(); }
	}

	Void MusicStream::Stop() {
		alSourceStop(m_alsource);
		alSourcei(m_alsource, AL_BUFFER, AL_NONE);
	}



	Bool MusicStream::IsPlaying() const {
		Int32 state = AL_PLAYING;
		alGetSourcei(m_alsource, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}


	Bool MusicStream::IsPaused() const {
		Int32 state = AL_PAUSED;
		alGetSourcei(m_alsource, AL_SOURCE_STATE, &state);
		return state == AL_PAUSED;
	}



	Void MusicStream::SetVolume(Float volume) {
		m_volume = volume;
		alSourcef(m_alsource, AL_GAIN, m_gain * m_volume);
	}

	Float MusicStream::GetVolume() const {
		return m_volume;
	}



	Void MusicStream::SetLoopEnabled(Bool loop) {
		m_loopEnabled = loop;
	}
	Bool MusicStream::IsLoopEnabled() const {
		return m_loopEnabled;
	}



	Bool MusicStream::Load(const String& filename) {
		AudioDesc desc;
		Int32     i, queued;

		if (!m_file.Load(filename)) {
			return false;
		}
		m_file.GetDesc(&desc);

		m_format     = AudioDevice::GetFormat(desc.nchannels);
		m_sampleRate = desc.sampleRate;
		m_buffersize = m_sampleRate * desc.nchannels;
		m_bufferdata.resize(m_buffersize);

		if (m_bufferdata.empty()) {
			return false;
		}
		alGetSourcei(m_alsource, AL_BUFFERS_QUEUED, &queued);
		for (i = 0; i < STREAMFRAGMENTS - queued; ++i) {
			if (FillBufferQueue(m_buffers[i]) == false)
				break;
		}
		return true;
	}



	Void MusicStream::Update() {
		Uint32 buffer;
		Int32  i, processed = 0;

		alGetSourcei(m_alsource, AL_BUFFERS_PROCESSED, &processed);

		for (i = 0; i < processed; ++i) {
			alSourceUnqueueBuffers(m_alsource, 1, &buffer);
			if (FillBufferQueue(buffer) == false)
				break;
		}
		if (!IsPlaying() && !IsPaused()) {
			if (processed == 0 || !m_loopEnabled) {
				return;
			}//may have to restart source if there was a buffer underrun 
			Play();
		}
	}



	Bool MusicStream::FillBufferQueue(Uint32 buffer) {
		Int32  size;
		Uint64 bytesread = 0;
		Int16* data = m_bufferdata.data();

		do {
			bytesread += m_file.Read(
				data + bytesread,
				m_buffersize - bytesread);

			if (bytesread < m_buffersize) {
				if (m_loopEnabled)
					m_file.Seek(0);
				else break;
			}
		} while (bytesread < m_buffersize);

		if (bytesread > 0) {
			size = (Int32)(bytesread * sizeof(Int16));
			alBufferData(buffer, m_format, data, size, m_sampleRate);
			alSourceQueueBuffers(m_alsource, 1, &buffer);
		}
		return bytesread >= m_buffersize;
	}
};
/*****************************************************************************/  
//EOF                                                                         |
/*****************************************************************************/