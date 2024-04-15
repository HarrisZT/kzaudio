/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 			          					                                      |
| File: kzaudiodecoder.cpp					                                  |
| Desc: abstract base type interface for audio decoders     	              |
|     						                                                  |
| This program is free software: you can redistribute it and/or modify	      |
| it under the terms of the GNU General Public License as published by	      |
| the Free Software Foundation, either version 3 of the License, or	          |
| (at your option) any later version.			                              |
| 					 				                                          |
| This program is distributed in the hope that it will be useful,             |
| but WITHOUT ANY WARRANTY; without even the implied warranty of	          |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		          |
| GNU General Public License for more details.			    	              |
| 							  		                                          |
| You should have received a copy of the GNU General Public License	          |
| along with this program.  If not, see <http://www.gnu.org/licenses/>.	      |
******************************************************************************/ 
#include <iostream>
#include "kzaudiodecoder.h" 
namespace kz {


	AudioDecoder* CreateAudioDecoder(const String& filename) {
	    IObuf file;
	    if (!file.Open(filename)) {
	        return NULL;
	    }
	    file.Seek(0);
	    if (FileIsFormatOGG(&file)) {
	 	return new AudioDecoderOGG;
	    }
	    file.Seek(0);
		if (FileIsFormatWAV(&file)) {
			return new AudioDecoderWAV;
		}
		std::cout << "failed to read audio file: " << filename
			      << ".\n format is not supported" << std::endl;
		return NULL;
	}
};
/*****************************************************************************/  
//EOF                                                                         |
/*****************************************************************************/
