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
#ifndef __KZAUDIOFILE_H__ 
#define __KZAUDIOFILE_H__ 

#include "kzaudiointernal.h"
namespace kz {



	/**
	provides read access to audio files*/
	class AudioFile {
	public:

		/** Construct AudioFile instance*/
		AudioFile();
		~AudioFile();


		/**	open a file for reading.
			Supports formats WAV (PCM only) and OGG/Vorbis.
			@return: true if the file was successfully opened, else false*/
		Bool Load(const String& filename);


		/**	read audio samples from the open file-
			@psamples: sample array to fill
			@nsamples: max number of samples to read
			@return  : samples read (may be less than nsamples)*/
		Uint64 Read(Int16* psamples, Uint64 nsamples);


		/**	set the read position to the given offset*/
		Void Seek(Uint64 offset);


		/**	set the read position to the given offset*/
		Void Seek(TimeValue offset);


		/**	close the file (data and resources are freed)*/
		Void Close();


		/** get information about the sound resource
			@param desc: structure to fill with data*/
		Void GetDesc(AudioDesc* desc) const;


	private:
		AudioDecoder* m_decoder;
		IObuf*        m_iobuf;
		Bool          m_iobufOwned;
		Uint32        m_nchannels;
		Uint64        m_sampleOffset;
		Uint64        m_sampleCount;
		Uint32        m_sampleRate;
	};
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/