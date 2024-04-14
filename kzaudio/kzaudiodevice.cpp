/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzaudiodevice.cpp											          |
| Desc: manager for OpenAL audio device           			                  |
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
#include "kzaudiodecoder.h"
#include "kzaudiodevice.h"
namespace kz {



	AudioDevice::AudioDevice() {
		m_initialized  = false;
		m_alDevice     = NULL;
		m_alContext    = NULL;
		m_globalVolume = 10.f;
	}



	AudioDevice::~AudioDevice() {
		alcMakeContextCurrent(NULL);
		if (m_alContext) {
			alcDestroyContext(m_alContext);
			m_alContext = NULL;
		}
		if (m_alDevice) {
			alcCloseDevice(m_alDevice);
			m_alDevice = NULL;
		}
	}



	Bool AudioDevice::Initialize() {
		if (m_initialized) {
			return true;
		}
		m_alDevice = alcOpenDevice(NULL);
		if (m_alDevice == NULL) {
			return false;
		}
		m_alContext = alcCreateContext(m_alDevice, NULL);
		if (m_alContext == NULL) {
			return false;
		}
		if (!alcMakeContextCurrent(m_alContext)) {
			return false;
		}
		const Float orientation[] = {
			0.f, 0.f, -1.f,
			0.f, 1.f, 0.f
		};
		alListenerf (AL_GAIN, m_globalVolume * 0.01f);
		alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
		alListenerfv(AL_ORIENTATION, orientation);

		m_initialized = true;
		return m_initialized;
	}



	Void AudioDevice::SetGlobalVolume(Int32 volume) {
		ClampVolume(volume);
		if (m_alContext)
			alListenerf(AL_GAIN, (Float)volume * 0.01f);
		m_globalVolume = (Float)volume;
	}



	Int32 AudioDevice::GetGlobalVolume() const {
		return (Int32)m_globalVolume;
	}



	Bool AudioDevice::HasExtension(const String& ext) const {
		if ((ext.length() > 2) && (ext.substr(0, 3) == "ALC"))
			return alcIsExtensionPresent(m_alDevice, ext.c_str()) != 0;
		return alIsExtensionPresent(ext.c_str()) != 0;
	}



	Int32 AudioDevice::GetFormat(Uint32 nchannels) {
		Int32 format = 0;
		switch (nchannels) {
		case 1:  format = AL_FORMAT_MONO16;                    break;
		case 2:  format = AL_FORMAT_STEREO16;                  break;
		case 4:  format = alGetEnumValue("AL_FORMAT_QUAD16");  break;
		case 6:  format = alGetEnumValue("AL_FORMAT_51CHN16"); break;
		case 7:  format = alGetEnumValue("AL_FORMAT_61CHN16"); break;
		case 8:  format = alGetEnumValue("AL_FORMAT_71CHN16"); break;
		}
		return format ? format : 0;
	}
};
/*****************************************************************************/  
//EOF                                                                         |
/*****************************************************************************/ 