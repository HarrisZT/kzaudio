/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzaudiodecoderwav.cpp 					                 	   	      |
| Desc: interface for WAV audio decoder          			                  |
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
namespace kz {

#define WAV_CHUNKSIZE      0x000C  
#define WAV_FORMAT_PCM     0x0001 
#define WAV_FORMAT_EXT     0xFFFE
#define WAV_SUBFORMAT_PCM  "\x01\x00\x00\x00\x00\x00\x10\x00"\
                           "\x80\x00\x00\xAA\x00\x38\x9B\x71" 



	/** Decode stream with 8-bit value*/
	static Bool Decode8Bit(IObuf* iobuf, Uint8& value) {
		return (SizeT)(iobuf->Read(&value, sizeof(value))) == sizeof(value);
	}



	/** Decode stream with 16-bit value*/
	static Bool Decode16Bit(IObuf* iobuf, Int16& value) {
		Byte bytes[sizeof(value)];
		if ((SizeT)(iobuf->Read(bytes, (Int64)(sizeof(bytes)))) != sizeof(bytes))
			return false;
		value = (Int16)(bytes[0] | (bytes[1] << 0x8));
		return true;
	}



	/** Decode stream with unsigned 16-bit value*/
	static Bool Decode16BitU(IObuf* iobuf, Uint16& value) {
		Byte bytes[sizeof(value)];
		if ((SizeT)(iobuf->Read(bytes, (Int64)(sizeof(bytes)))) != sizeof(bytes))
			return false;
		value = (Uint16)(bytes[0] | (bytes[1] << 0x8));
		return true;
	}



	/** Decode stream with 24-bit value*/
	static Bool Decode24bit(IObuf* iobuf, Uint32& value) {
		Byte bytes[3];
		if ((SizeT)(iobuf->Read(bytes, (Int64)(sizeof(bytes)))) != sizeof(bytes))
			return false;
		value = (Uint32)(bytes[0] |
			(bytes[1] << 0x8) |
			(bytes[2] << 0x10));
		return true;
	}



	/** Decode stream with 32-bit value*/
	static Bool Decode32Bit(IObuf* iobuf, Uint32& value) {
		Byte bytes[sizeof(value)];
		if ((SizeT)(iobuf->Read(bytes, (Int64)(sizeof(bytes)))) != sizeof(bytes))
			return false;
		value = (Uint32)(bytes[0] |
			(bytes[1] << 0x8) |
			(bytes[2] << 0x10) |
			(bytes[3] << 0x18));
		return true;
	}



	Bool FileIsFormatWAV(IObuf* iobuf) {
		Char header[WAV_CHUNKSIZE];

		if (iobuf->Read(header, sizeof(header)) < (Int64)(sizeof(header)))
			return false;
		return (
			(header[0]  == 'R') && (header[1]  == 'I') &&
			(header[2]  == 'F') && (header[3]  == 'F') &&
			(header[8]  == 'W') && (header[9]  == 'A') &&
			(header[10] == 'V') && (header[11] == 'E'));
	}
	/**************************************************************************
	**************************************************************************/





	/**************************************************************************
	**************************************************************************/
	AudioDecoderWAV::AudioDecoderWAV() {
		m_iobuf = NULL;
		m_bytesPerSample = 0;
		m_bufferStart = 0;
		m_bufferEnd = 0;
	}



	Bool AudioDecoderWAV::Open(IObuf* iobuf, AudioDesc* desc) {
		m_iobuf = iobuf;
		return Parse(desc);
	}



	Void AudioDecoderWAV::Seek(Uint64 offset) {
		m_iobuf->Seek((Int64)(m_bufferStart + offset * m_bytesPerSample));
	}



	Uint64 AudioDecoderWAV::Read(Int16* samples, Uint64 imax) {
		Uint64 count = 0;
		Uint64 startPos = (Uint64)m_iobuf->Tell();

		while ((count < imax) &&
			(startPos + count * m_bytesPerSample < m_bufferEnd)) {

			switch (m_bytesPerSample) {
			case 1:
			{
				Uint8 sample = 0;
				if (Decode8Bit(m_iobuf, sample))
					*samples++ = (Int16)(((Int16)(sample)-0x80) << 0x8);
				else return count;
				break;
			}
			case 2:
			{
				Int16 sample = 0;
				if (Decode16Bit(m_iobuf, sample))
					*samples++ = sample;
				else return count;
				break;
			}
			case 3:
			{
				Uint32 sample = 0;
				if (Decode24bit(m_iobuf, sample))
					*samples++ = (Int16)(sample >> 0x8);
				else return count;
				break;
			}
			case 4:
			{
				Uint32 sample = 0;
				if (Decode32Bit(m_iobuf, sample))
					*samples++ = (Int16)(sample >> 0x10);
				else return count;
				break;
			}
			default: return 0;
			}
			++count;
		}
		return count;
	}



	Bool AudioDecoderWAV::Parse(AudioDesc* info) {
		Char mainChunk[WAV_CHUNKSIZE];

		if ((SizeT)(m_iobuf->Read(mainChunk,
			(Int64)(sizeof(mainChunk)))) != sizeof(mainChunk)) {
			return false;
		}
		Bool foundChunk = false;

		while (!foundChunk) {
			Char subChunkId[4];
			if ((SizeT)(m_iobuf->Read(subChunkId,
				(Int64)(sizeof(subChunkId)))) != sizeof(subChunkId)) {
				return false;
			}
			Uint32 subChunkSize = 0;
			if (!Decode32Bit(m_iobuf, subChunkSize)) {
				return false;
			}
			Int64 subChunkStart = m_iobuf->Tell();
			if (subChunkStart == -1) {
				return false;
			}
			// Check which chunk it is
			if ((subChunkId[0] == 'f') && (subChunkId[1] == 'm') &&
				(subChunkId[2] == 't') && (subChunkId[3] == ' ')) {

				Uint16 format = 0;
				if (!Decode16BitU(m_iobuf, format)) {
					return false;
				}
				if ((format != WAV_FORMAT_PCM) &&
					(format != WAV_FORMAT_EXT)) {
					return false;
				}
				Uint16 channelCount = 0;
				if (!Decode16BitU(m_iobuf, channelCount)) {
					return false;
				}
				info->nchannels = channelCount;

				Uint32 sampleRate = 0;
				if (!Decode32Bit(m_iobuf, sampleRate)) {
					return false;
				}
				info->sampleRate = sampleRate;

				Uint32 byteRate = 0;
				if (!Decode32Bit(m_iobuf, byteRate)) {
					return false;
				}
				Uint16 blockAlign = 0;
				if (!Decode16BitU(m_iobuf, blockAlign)) {
					return false;
				}
				Uint16 bitsPerSample = 0;
				if (!Decode16BitU(m_iobuf, bitsPerSample)) {
					return false;
				}
				if (bitsPerSample != 0x08 && bitsPerSample != 0x10 &&
					bitsPerSample != 0x18 && bitsPerSample != 0x20) {
					return false;
				}
				m_bytesPerSample = bitsPerSample / 8;

				if (format == WAV_FORMAT_EXT) {
					// Extension size
					Uint16 extensionSize = 0;
					if (!Decode16BitU(m_iobuf, extensionSize)) {
						return false;
					}
					// Valid bits per sample
					Uint16 validBitsPerSample = 0;
					if (!Decode16BitU(m_iobuf, validBitsPerSample)) {
						return false;
					}
					// Channel mask
					Uint32 channelMask = 0;
					if (!Decode32Bit(m_iobuf, channelMask)) {
						return false;
					}
					// Subformat
					Char subformat[16];
					if ((SizeT)(m_iobuf->Read(subformat,
						(Int64)(sizeof(subformat)))) != sizeof(subformat)) {
						return false;
					}
					if (memcmp(subformat, WAV_SUBFORMAT_PCM,
						sizeof(subformat)) != 0) {
						return false;
					}

					if (validBitsPerSample != bitsPerSample) {
						return false;
					}
				}
				// Skip potential extra information
				if (m_iobuf->Seek(subChunkStart + subChunkSize) == -1)
					return false;
			}
			else if ((subChunkId[0] == 'd') && (subChunkId[1] == 'a') &&
				(subChunkId[2] == 't') && (subChunkId[3] == 'a')) {

				info->sampleCount = subChunkSize / m_bytesPerSample;

				// Store the start and end position of samples in the file
				m_bufferStart = (Uint64)(subChunkStart);
				m_bufferEnd = m_bufferStart + info->sampleCount * m_bytesPerSample;
				foundChunk = true;
			}
			else if (m_iobuf->Seek(
				m_iobuf->Tell() + subChunkSize) == -1) {
				return false;
			}
		}
		return true;
	}
};
/*****************************************************************************/  
//EOF                                                                         |
/*****************************************************************************/