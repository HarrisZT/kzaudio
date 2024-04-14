/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kznoncopyable.h									                      |
| Desc: mixin class that disables copying                                     | 
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
#ifndef __KZNONCOPYABLE_H__ 
#define __KZNONCOPYABLE_H__ 

namespace kz { 

	/**
	intended to be used as a private mixin base class, where other classes
	that inherit from it can't be copied or assigned because the copy
	constructor and copy assignment operator become private.*/
	class NonCopyable {
	protected:
		NonCopyable() {}
		~NonCopyable() {}
	private:
		NonCopyable(const NonCopyable&);
		const NonCopyable& operator=(const NonCopyable&);
	};
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/ 