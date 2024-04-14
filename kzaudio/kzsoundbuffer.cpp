/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzsoundbuffer.cpp											 	      |
| Desc: source data used by Sounds             			                      |
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
#include "kzaudiodevice.h"
#include "kzaudiofile.h"
#include "kzsound.h"
#include "kzsoundbuffer.h"
namespace kz {



	SoundBuffer::SoundBuffer() {
		alGenBuffers(1, &m_bufferId);
	}



	SoundBuffer::SoundBuffer(const SoundBuffer& copy) :
		m_sampleData(copy.m_sampleData),
		m_length(copy.m_length) {

		alGenBuffers(1, &m_bufferId);
		AudioDesc desc;
		copy.GetDesc(&desc);
		Update(desc.nchannels, desc.sampleRate);
	}



	SoundBuffer& SoundBuffer::operator=(const SoundBuffer& copy) {
		SoundBuffer temp(copy);
		std::swap(m_sampleData, temp.m_sampleData);
		std::swap(m_bufferId, temp.m_bufferId);
		std::swap(m_length, temp.m_length);
		std::swap(m_registeredSounds, temp.m_registeredSounds);
		return *this;
	}



	SoundBuffer::~SoundBuffer() {
		SOUNDSET sounds;
		sounds.swap(m_registeredSounds);

		for (auto* soundPtr : sounds) {
			soundPtr->ResetBuffer();
		}
		if (m_bufferId) {
			alDeleteBuffers(1, &m_bufferId);
		}
	}



	Bool SoundBuffer::Load(const String& filename) {
		AudioFile file;
		if (file.Load(filename))
			return Initialize(&file);
		return false;
	}



	Void SoundBuffer::GetDesc(AudioDesc* desc) const {
		Int32 sampleRate, channelCount;

		desc->samples = m_sampleData.empty() ? NULL : m_sampleData.data();
		desc->sampleCount = m_sampleData.size();

		alGetBufferi(m_bufferId, AL_FREQUENCY, &sampleRate);
		desc->sampleRate = (Uint32)(sampleRate);

		alGetBufferi(m_bufferId, AL_CHANNELS, &channelCount);
		desc->nchannels = (Uint32)(channelCount);

		desc->length = m_length;
	}



	Bool SoundBuffer::Initialize(AudioFile* file) {
		AudioDesc desc;
		file->GetDesc(&desc);
		m_sampleData.resize((SizeT)(desc.sampleCount));
		if (file->Read(m_sampleData.data(), desc.sampleCount) == desc.sampleCount)
			return Update(desc.nchannels, desc.sampleRate);
		return false;
	}



	Bool SoundBuffer::Update(Uint32 nchannels, Uint32 sampleRate) {
		if (!nchannels || !sampleRate) {
			return false;
		}
		if (m_sampleData.empty()) {
			return false;
		}
		//check if the format is valid
		Int32 format = AudioDevice::GetFormat(nchannels);
		if (format == 0) {
			return false;
		}
		//copy the list of sounds so we can reattach later
		const SOUNDSET soundsCopy(m_registeredSounds);
		for (auto* soundPtr : soundsCopy) {
			soundPtr->ResetBuffer();
		}
		//fill the buffer 
		alBufferData(m_bufferId,
			format,
			m_sampleData.data(),
			(Int32)(m_sampleData.size() * sizeof(Int16)),
			(Int32)(sampleRate));

		m_length = TimeValue::FromSeconds(
			(Float)m_sampleData.size() /
			(Float)sampleRate /
			(Float)nchannels);

		for (auto* soundPtr : soundsCopy) {
			soundPtr->SetBuffer(this);
		}
		return true;
	}
};
/******************************************************************************
//EOF                                                                         |
******************************************************************************/