/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                         
| Author: Zachary T Harris                                                    
| 																			   
| File: kzaudiodecoder.h 										               
| Desc: abstract base type interface for audio decoders     	               
|     																		   
| This program is free software: you can redistribute it and/or modify		   
| it under the terms of the GNU General Public License as published by		   
| the Free Software Foundation, either version 3 of the License, or			  
| (at your option) any later version.										  
| 																			  
| This program is distributed in the hope that it will be useful,			   
| but WITHOUT ANY WARRANTY; without even the implied warranty of			   
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				  
| GNU General Public License for more details.								  
| 																			  
| You should have received a copy of the GNU General Public License			   
| along with this program.  If not, see <http://www.gnu.org/licenses/>.		   
******************************************************************************/
#ifndef __KZAUDIODECODER_H__
#define __KZAUDIODECODER_H__

#include "kzaudiointernal.h"
namespace kz {



	/**
	abstract base type interface for audio decoders*/
	class AudioDecoder {
	public:
		virtual ~AudioDecoder() {}

		/**open an audio file for decoding
		   @param desc: structure to fill with audio properties
		   @return    : true if successfully opened, else false*/
		virtual Bool Open(IObuf* file, AudioDesc* desc) = 0;

		/**read audio samples from the file
		   @param samples: sample array to be filled
		   @param imax   : maximum samples to be read
		   @return       : samples read (may be less than imax)*/
		virtual Uint64 Read(Int16* samples, Uint64 imax) = 0;

		/**change the read position to the given sample offset
		   @param offset: index of sample to go to (relative to beginning)*/
		virtual Void Seek(Uint64 offset) = 0;
	};





	/**
	audio decoder for WAV format files*/
	class AudioDecoderWAV final : public AudioDecoder {
	public:
		AudioDecoderWAV();

		Bool   Open(IObuf* file, AudioDesc* desc) override;
		Uint64 Read(Int16* samples, Uint64 imax) override;
		Void   Seek(Uint64 offset) override;

	private:
		Bool Parse(AudioDesc* desc);

		IObuf*  m_iobuf;
		Uint32  m_bytesPerSample;
		Uint64  m_bufferStart;
		Uint64  m_bufferEnd;
        };





	/**
	audio decoder for OGG format files*/
	class AudioDecoderOGG : public AudioDecoder {
	public:
		AudioDecoderOGG();
		~AudioDecoderOGG();

		Bool   Open(IObuf* file, AudioDesc* desc) override;
		Uint64 Read(Int16* samples, Uint64 imax) override;
		Void   Seek(Uint64 offset) override;

	private:
		struct OggVorbis_File* m_oggfile;
		Uint32                 m_nchannels;
	};





	/** validate that the given file is WAV format*/
	extern Bool FileIsFormatWAV(IObuf* iobuf);

	/** validate that the given file is OGG format*/
	extern Bool FileIsFormatOGG(IObuf* iobuf);


	/** create a decoder for the given file-
		@param filename: path of the sound file
		@param return:   AudioDecoder that can read the given file,
						 or null if the format is unsupported.
						 [CALLER IS RESPONSIBLE FOR FREEING THE INSTANCE]*/
	extern AudioDecoder* CreateAudioDecoder(const String& filename);

};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/
