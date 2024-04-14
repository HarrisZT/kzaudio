/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzaudiodevice.h														  |
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
#ifndef __KZAUDIODEVICE_H__
#define __KZAUDIODEVICE_H__

#include "kzaudiointernal.h"
namespace kz {



	/**
	manager for OpenAL audio device*/
	class AudioDevice final {
	public:

		/** Construct audio device instance*/
		AudioDevice();
		~AudioDevice();


		/** load and initialize the OpenAL audio device
			@return: true on success, false on failure*/
		Bool Initialize();


		/** set the global device volume (affects all sounds and music)
			@param volume: volume value [0-100]*/
		Void SetGlobalVolume(Int32 volume);


		/** returns the global device volume [0-100]*/
		Int32 GetGlobalVolume() const;


		/** check whether the device has an OpenAL extension-
			@param ext: identifies the extension to check for
			@return: true if extension exists, else false*/
		Bool HasExtension(const String& ext) const;


		/** returns the appropriate audio format (OpenAL enum value)
			for the given number of channels, or zero if none found*/
		static Int32 GetFormat(Uint32 channels);


	private:
		Bool                      m_initialized;
		Float                     m_globalVolume;
		struct ALCdevice_struct*  m_alDevice;
		struct ALCcontext_struct* m_alContext;
	};
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/