/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 			          					      |
| File: kzaudiodecoderogg.cpp					              |
| Desc: interface for ogg audio                            	              |
|     						                              |
| This program is free software: you can redistribute it and/or modify	      |
| it under the terms of the GNU General Public License as published by	      |
| the Free Software Foundation, either version 3 of the License, or	      |
| (at your option) any later version.			                      |
| 					 				      |
| This program is distributed in the hope that it will be useful,             |
| but WITHOUT ANY WARRANTY; without even the implied warranty of	      |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		      |
| GNU General Public License for more details.			    	      |
| 							  		      |
| You should have received a copy of the GNU General Public License	      |
| along with this program.  If not, see <http://www.gnu.org/licenses/>.	      |
******************************************************************************/
#include <vorbis/vorbisfile.h>
#include "kzaudiodecoder.h" 
namespace kz {



    static SizeT Read(Lpvoid ptr, SizeT size, SizeT nmemb, Lpvoid data) {
        IObuf* file = (IObuf*)(data);
        return (SizeT)(file->Read(ptr, (Int64)(size * nmemb)));
    }



    static Int32 Seek(Lpvoid data, Int64 offset, Int32 whence) {
        IObuf* file = (IObuf*)(data);
        switch (whence) {
        case SEEK_SET:
	    break;
	case SEEK_CUR:
	    offset += file->Tell();
	    break;
	case SEEK_END:
	    offset = file->GetSize() - offset;
	}
	return (Int32)(file->Seek(offset));
    }



    static Long Tell(Lpvoid data) {
        IObuf* file = (IObuf*)(data);
	return (Long)(file->Tell());
    }



    static ov_callbacks s_callbacks = {
        &Read, &Seek, NULL, &Tell
    };



    Bool FileIsFormatOGG(IObuf* iobuf) {
        OggVorbis_File file;
	if (ov_test_callbacks(iobuf, &file, NULL, 0, s_callbacks) == 0) {
	    ov_clear(&file);
	    return true;
	}
	return false;
    }
    /**************************************************************************
    **************************************************************************/





    /**************************************************************************
    **************************************************************************/
    AudioDecoderOGG::AudioDecoderOGG() {
        m_nchannels = 0;
        m_oggfile = new OggVorbis_File();
        m_oggfile->datasource = NULL;
    }



    AudioDecoderOGG::~AudioDecoderOGG() {
        if (m_oggfile->datasource) {
            ov_clear(m_oggfile);
            m_oggfile->datasource = NULL;
	    m_nchannels = 0;
        }
	if (m_oggfile) {
	    delete m_oggfile;
	    m_oggfile = NULL;
	}
    }



    Bool AudioDecoderOGG::Open(IObuf* iobuf, AudioDesc* desc) {
        Int32        status;
        vorbis_info* vorbisInfo;

	status = ov_open_callbacks(iobuf, m_oggfile, NULL, 0, s_callbacks);
	if (status < 0) {
	    return false;
	}
	vorbisInfo        = ov_info(m_oggfile, -1);
	desc->nchannels   = (Uint32)(vorbisInfo->channels);
	desc->sampleRate  = (Uint32)(vorbisInfo->rate);
	desc->sampleCount = (SizeT)(ov_pcm_total(m_oggfile, -1) * vorbisInfo->channels);

	m_nchannels = desc->nchannels;
	return true;
    }



    Void AudioDecoderOGG::Seek(Uint64 offset) {
        ov_pcm_seek(m_oggfile, (Int64)(offset / m_nchannels));
    }



    Uint64 AudioDecoderOGG::Read(Int16* samples, Uint64 imax) {
        Uint64 count = 0;
	Int32  bytesToRead;
	Long   bytesRead, samplesRead;

	while (count < imax) {
	    bytesToRead = (Int32)(imax - count) * (Int32)sizeof(Int16);

	    bytesRead = ov_read(m_oggfile,
				(Char*)samples,
				bytesToRead, 0, 2, 1, NULL);

	    if (bytesRead > 0) {
	        samplesRead = bytesRead / (Long)sizeof(Int16);
		count += (Uint64)(samplesRead);
		samples += samplesRead;
	    }
	    else break;
	}
	return count;
    }
};
/*****************************************************************************/  
//EOF                                                                         |
/*****************************************************************************/
