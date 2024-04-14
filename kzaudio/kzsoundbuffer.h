/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzsoundbuffer.h 											          |
| Desc: source data used by Sounds          			                      |
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
#ifndef __KZSOUNDBUFFER_H__ 
#define __KZSOUNDBUFFER_H__ 

#include "kzaudiointernal.h" 
namespace kz {



	/**
    source data used by Sounds*/
	class SoundBuffer final {
	public:
		SoundBuffer();
		SoundBuffer(const SoundBuffer& copy);
		SoundBuffer& operator=(const SoundBuffer& copy);
		~SoundBuffer();

		/** load the sound data from a file.
			@param filename: name of the file to load
			@return: true on success, false on failure*/
		Bool Load(const String& filename);

		/** get information about the sound resource
			@param desc: structure to fill with data*/
		Void GetDesc(AudioDesc* desc) const;


	private:
		friend class Sound;
		typedef std::unordered_set<Sound*> SOUNDSET;

		Bool Initialize(AudioFile* file);
		Bool Update(Uint32 channels, Uint32 sampleRate);

		SAMPLEDATA       m_sampleData;
		TimeValue        m_length;
		Uint32           m_bufferId;
		mutable SOUNDSET m_registeredSounds;
	};
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/