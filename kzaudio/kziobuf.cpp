/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| file: kziobuf.cpp												              |
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
#include "kziobuf.h"
namespace kz {



	IObuf::IObuf() :
		m_file(nullptr) {
	}

	IObuf::~IObuf() {
		Close();
	}



#if (KZIOBUF_USING_PHYSFS)

	Bool IObuf::Close() {
		return m_file ? PHYSFS_close(m_file) : false;
	}


	Bool IObuf::Open(const String& filename) {
		Close();
		m_file = PHYSFS_openRead(filename.c_str());
		return m_file != nullptr;
	}


	Int64 IObuf::Read(Void* buffer, Int64 nbytes) {
		if (m_file)
			return PHYSFS_readBytes(m_file, buffer, nbytes);
		return -1;
	}


	Int64 IObuf::Seek(Int64 position, Int64 whence) {
		if (m_file) {
			if (PHYSFS_seek(m_file, position))
				return Tell();
		}
		return -1;
	}


	Int64 IObuf::Tell() {
		return m_file ? PHYSFS_tell(m_file) : -1;
	}


	Int64 IObuf::GetSize() {
		return m_file ? PHYSFS_fileLength(m_file) : -1;
	}


	Int32 IObuf::GetEndOfFile() {
		return m_file ? PHYSFS_eof(m_file) : 0;
	}


	Bool IObuf::UsingPhysfs() const {
		return true;
	} 


#else  

	Bool IObuf::Close() {
		return m_file ? !fclose(m_file) : false;
	}


	Bool IObuf::Open(const String& filename) {
		Close();
		m_file = fopen(filename.c_str(), "rb");
		return m_file != nullptr;
	}


	Int64 IObuf::Read(Lpvoid buffer, Int64 nbytes) {
		if (m_file)
			return (Int64)(fread(buffer, 1, (SizeT)(nbytes), m_file));
		return -1;
	}


	Int64 IObuf::Seek(Int64 position, Int64 whence) {
		if (m_file) {
			if (fseek(m_file, (Long)(position), whence) == 0)
				return Tell();
		}
		return -1;
	}


	Int64 IObuf::Tell() {
		return m_file ? ftell(m_file) : -1;
	}


	Int64 IObuf::GetSize() {
		Int64 size, position;
		if (m_file) {
			position = Tell();
			fseek(m_file, 0, SEEK_END);
			size = Tell();
			if (Seek(position) != -1)
				return size;
		}
		return -1;
	}


	Int32 IObuf::GetEndOfFile() {
		return m_file ? feof(m_file) : 0;
	}


	Bool IObuf::UsingPhysfs() const {
		return false;
	}

#endif
};
/*****************************************************************************/  
//EOF                                                                         |
/*****************************************************************************/