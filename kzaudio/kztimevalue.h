/*****************************************************************************\
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 		                                                              |			 
| File: kztimevalue.h							      |
| Desc: interface for representing a time value           	              |
|     									      |
| This program is free software: you can redistribute it and/or modify	      |
| it under the terms of the GNU General Public License as published by	      |
| the Free Software Foundation, either version 3 of the License, or	      |
| (at your option) any later version.					      |
| 									      |
| This program is distributed in the hope that it will be useful,	      |
| but WITHOUT ANY WARRANTY; without even the implied warranty of	      |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		      |
| GNU General Public License for more details.				      |
| 									      |
| You should have received a copy of the GNU General Public License	      |
| along with this program.  If not, see <http://www.gnu.org/licenses/>.	      |
******************************************************************************/
#ifndef __KZTIMEVALUE_H__ 
#define __KZTIMEVALUE_H__ 

#include "kzbasetypes.h"
namespace kz {



    /**
    interface for representing a time value*/
	class TimeValue final {
	public:
		TimeValue() : m_microseconds(0) {}


		/** return the time value as seconds**/
		Float AsSeconds() const;

		/** return the time value as milliseconds**/
		Int32 AsMilliseconds() const;

		/** return the time value as microseconds**/
		Int64 AsMicroseconds() const;


		/** construct a time value from a number of seconds*/
		static TimeValue FromSeconds(Float seconds);

		/** construct a time value from a number of milliseconds*/
		static TimeValue FromMilliseconds(Int32 milliseconds);

		/** construct a time value from a number of microseconds*/
		static TimeValue FromMicroseconds(Int64 microseconds);


	private:
		/**
		construct from a number of microseconds*/
		explicit TimeValue(Int64 microseconds) :
			m_microseconds(microseconds) {}
		Int64 m_microseconds; //time value stored as microseconds
	};





	inline Float TimeValue::AsSeconds() const {
		return (Float)((Double)(m_microseconds) / 1000000.0);
	}

	inline Int32 TimeValue::AsMilliseconds() const {
		return (Int32)(m_microseconds / 1000);
	}

	inline Int64 TimeValue::AsMicroseconds() const {
		return m_microseconds;
	}

	inline TimeValue TimeValue::FromSeconds(Float amount) {
		return TimeValue((Int64)(amount * 1000000));
	}

	inline TimeValue TimeValue::FromMilliseconds(Int32 amount) {
		return TimeValue((Int64)(amount) * 1000);
	}

	inline TimeValue TimeValue::FromMicroseconds(Int64 amount) {
		return TimeValue(amount);
	}





	inline Bool operator ==(TimeValue a, TimeValue b) {
		return a.AsMicroseconds() == b.AsMicroseconds();
	}
	inline Bool operator !=(TimeValue a, TimeValue b) {
		return a.AsMicroseconds() != b.AsMicroseconds();
	}
	inline Bool operator <(TimeValue a, TimeValue b) {
		return a.AsMicroseconds() < b.AsMicroseconds();
	}
	inline Bool operator >(TimeValue a, TimeValue b) {
		return a.AsMicroseconds() > b.AsMicroseconds();
	}
	inline Bool operator <=(TimeValue a, TimeValue b) {
		return a.AsMicroseconds() <= b.AsMicroseconds();
	}
	inline Bool operator >=(TimeValue a, TimeValue b) {
		return a.AsMicroseconds() >= b.AsMicroseconds();
	}
	inline TimeValue operator -(TimeValue b) {
		return TimeValue::FromMicroseconds(-b.AsMicroseconds());
	}
	inline TimeValue operator +(TimeValue a, TimeValue b) {
		return TimeValue::FromMicroseconds(a.AsMicroseconds() + b.AsMicroseconds());
	}
	inline TimeValue& operator +=(TimeValue& a, TimeValue b) {
		return a = a + b;
	}
	inline TimeValue operator -(TimeValue a, TimeValue b) {
		return TimeValue::FromMicroseconds(a.AsMicroseconds() - b.AsMicroseconds());
	}
	inline TimeValue& operator -=(TimeValue& a, TimeValue b) {
		return a = a - b;
	}
	inline TimeValue operator *(TimeValue a, Float b) {
		return TimeValue::FromSeconds(a.AsSeconds() * b);
	}
	inline TimeValue operator *(TimeValue a, Int64 b) {
		return TimeValue::FromMicroseconds(a.AsMicroseconds() * b);
	}
	inline TimeValue operator *(Float a, TimeValue b) {
		return b * a;
	}
	inline TimeValue operator *(Int64 a, TimeValue b) {
		return b * a;
	}
	inline TimeValue& operator *=(TimeValue& a, Float b) {
		return a = a * b;
	}
	inline TimeValue& operator *=(TimeValue& a, Int64 b) {
		return a = a * b;
	}
	inline TimeValue operator /(TimeValue a, Float b) {
		return TimeValue::FromSeconds(a.AsSeconds() / b);
	}
	inline TimeValue operator /(TimeValue a, Int64 b) {
		return TimeValue::FromMicroseconds(a.AsMicroseconds() / b);
	}
	inline TimeValue& operator /=(TimeValue& a, Float b) {
		return a = a / b;
	}
	inline TimeValue& operator /=(TimeValue& a, Int64 b) {
		return a = a / b;
	}
	inline Float operator /(TimeValue a, TimeValue b) {
		return a.AsSeconds() / b.AsSeconds();
	}
	inline TimeValue operator %(TimeValue a, TimeValue b) {
		return TimeValue::FromMicroseconds(a.AsMicroseconds() % b.AsMicroseconds());
	}
	inline TimeValue& operator %=(TimeValue& a, TimeValue b) {
		return a = a % b;
	}
};
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/
