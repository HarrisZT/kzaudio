/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kziobuf.h												              |
| Desc: provides an interface and wrapper around a lower level file structure | 
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
#ifndef __KZIOBUF_H__
#define __KZIOBUF_H__
/**
if this define is set to 1, kz::IObuf will use a PHYSFS_File struct,
otherwise a standard c library FILE struct will be used*/
#define KZIOBUF_USING_PHYSFS 0

#if (KZIOBUF_USING_PHYSFS)
#  include <physfs.h>
#endif
#include "kzbasetypes.h" 
#include "kznoncopyable.h" 

namespace kz {



	/**
	provides an interface and wrapper around a lower level file structure*/
	class IObuf final : NonCopyable {
	public:
		IObuf();
		~IObuf();


		/**	open the file stream from a file path
			@param path: path/name of the file to open
			@return:     true on success, false on error*/
		Bool Open(const String& path);


		/** closes the file stream
			@return: true on success, false on error*/
		Bool Close();


		/**	read data from the stream
			@param buffer: buffer to store read data into
			@param nbytes: number of bytes being read from
			@return:       number of bytes read, or -1 on error*/
		Int64 Read(Void* buffer, Int64 nbytes);


		/**	seek to a new position with the file.
			@param position: number of bytes to offset from whence
			@param whence:   position from where offset is added
			@return:         the position sought to, or -1 on error*/
		Int64 Seek(Int64 position, Int64 whence = 0);


		/**	get the current reading position in the stream
			@return: The current position, or -1 on error*/
		Int64 Tell();


		/**	return the size of the file
			@return: total bytes available in the stream (-1 on error)*/
		Int64 GetSize();


		/** tests the end-of-file indicator for the stream
			@return: non-zero when end-of-file indicator associated
					 with the stream is set, else zero is returned.*/
		Int32 GetEndOfFile();


		/** returns true if the underlying structure is PHYSFS based*/
		Bool UsingPhysfs() const;


	private:
	#if (KZIOBUF_USING_PHYSFS)
		PHYSFS_File* m_file;
	#else
		FILE* m_file;
	#endif
	};
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/
