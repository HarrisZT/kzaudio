/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: kzglobalinstance.h 												      |
| Desc: defines GlobalInstance base class                     			      |
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
#ifndef __KZGLOBALINSTANCE_H__
#define __KZGLOBALINSTANCE_H__

namespace kz { 

	/**
	allows global access to the currently active instance of a class.
	its similar to a singleton, but does not handle object construction.*/
	template<class T>
	class GlobalInstance {
	public:
		static T* GetInstance();

	protected:
		GlobalInstance();

		virtual ~GlobalInstance();

	private:
		static GlobalInstance<T>* s_instance;
	};



	template<typename T>
	inline GlobalInstance<T>::GlobalInstance() {
		s_instance = this;
	}

	template<typename T>
	inline GlobalInstance<T>::~GlobalInstance() {
		if (s_instance == this) {
			s_instance = nullptr;
		}
	}

	template<typename T>
	inline T* GlobalInstance<T>::GetInstance() {
		return (T*)(s_instance);
	}

	template<class T>
	GlobalInstance<T>* GlobalInstance<T>::s_instance = nullptr;
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/