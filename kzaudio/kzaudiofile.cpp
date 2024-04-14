/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzaudiofile.h 												   	      |
| Desc: provides read access to audio files         	                      |
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
#include "kzaudiodecoder.h"
#include "kzaudiodevice.h"
#include "kzaudiofile.h"
namespace kz {



	AudioFile::AudioFile() {
		m_decoder      = NULL;
		m_iobuf        = NULL;
		m_iobufOwned   = false;
		m_sampleOffset = 0;
		m_sampleCount  = 0;
		m_nchannels    = 0;
		m_sampleRate   = 0;
	}



	AudioFile::~AudioFile() {
		Close();
	}



	Bool AudioFile::Load(const String& filename) {
		AudioDesc info;

		Close();

		m_decoder = CreateAudioDecoder(filename);
		if (!m_decoder) {
			return false;
		}
		m_iobuf = new IObuf();
		m_iobufOwned = true;

		if (!m_iobuf->Open(filename)) {
			Close();
			return false;
		}
		if (!m_decoder->Open(m_iobuf, &info)) {
			Close();
			return false;
		}
		m_sampleCount = info.sampleCount;
		m_nchannels   = info.nchannels;
		m_sampleRate  = info.sampleRate;

		return true;
	}



	Void AudioFile::Seek(Uint64 sampleOffset) {
		if (m_decoder && m_nchannels != 0) {
			m_sampleOffset = Min(sampleOffset / m_nchannels *
				                 m_nchannels, m_sampleCount);
			m_decoder->Seek(m_sampleOffset);
		}
	}



	Void AudioFile::Seek(TimeValue timeOffset) {
		Seek((Uint64)(timeOffset.AsSeconds() *
			 (Float)(m_sampleRate)) * m_nchannels);
	}



	Uint64 AudioFile::Read(Int16* samples, Uint64 maxCount) {
		Uint64 readSamples = 0;
		if (m_decoder && samples && maxCount)
			readSamples = m_decoder->Read(samples, maxCount);
		m_sampleOffset += readSamples;
		return readSamples;
	}



	Void AudioFile::Close() {
		if (m_decoder) {
			delete m_decoder;
			m_decoder = NULL;
		}
		if (m_iobufOwned && m_iobuf != NULL) {
			m_iobufOwned = false;
			if (m_iobuf) {
				delete m_iobuf;
				m_iobuf = NULL;
			} 
		}
		m_iobuf        = NULL;
		m_sampleOffset = 0;
		m_sampleCount  = 0;
		m_nchannels    = 0;
		m_sampleRate   = 0;
	}



	Void AudioFile::GetDesc(AudioDesc* desc) const {
		desc->sampleCount  = m_sampleCount;
		desc->nchannels    = m_nchannels;
		desc->sampleRate   = m_sampleRate;
		desc->sampleOffset = m_sampleOffset;

		if (m_nchannels == 0 || m_sampleRate == 0)
			desc->length = TimeValue();
		else {
			desc->length = TimeValue::FromSeconds(
				(Float)(m_sampleCount) /
				(Float)(m_nchannels) /
				(Float)(m_sampleRate));
		}
		if (m_nchannels == 0 || m_sampleRate == 0)
			desc->position = TimeValue();
		else {
			desc->position = TimeValue::FromSeconds(
				(Float)(m_sampleOffset) /
				(Float)(m_nchannels) /
				(Float)(m_sampleRate));
		}
	}
};
/*****************************************************************************/  
//EOF                                                                         |
/*****************************************************************************/