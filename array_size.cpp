/*************************************************************************/
/* A compile time array size example                                     */
/* Copyright (C) 2014                                                    */
/* Johannes Lorenz                                                       */
/* https://github.com/JohannesLorenz/                                    */
/*                                                                       */
/* This program is free software; you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation; either version 3 of the License, or (at */
/* your option) any later version.                                       */
/* This program is distributed in the hope that it will be useful, but   */
/* WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      */
/* General Public License for more details.                              */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program; if not, write to the Free Software           */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA  */
/*************************************************************************/

#include <iostream>

//! Sets @a value to true iff index @a Pos is contained in an array
//! @param Container a container which has an array named @a data
template<class Container, std::size_t Pos>
class is_index_in_range
{
	template<class Container2, std::size_t Pos2>
	struct int_and_pos {
		using cont = Container2;
		static constexpr std::size_t pos = Pos2;
		int_and_pos(std::size_t) {}
	};
	
	template<class T, char c>
	struct val_to_type { typedef std::size_t type; };
	
	typedef char yes[1];
	typedef char no[2];

	template<class C>
	static yes& sfinae(typename val_to_type<
		decltype(*C::cont::data), *(C::cont::data + C::pos)>::type );
	template<class C>
	static no& sfinae(C );
public:
	static constexpr bool value =
		sizeof(sfinae<int_and_pos<Container, Pos>>(42))
		== sizeof(yes);
};

//! Returns length of array, starting at @a StartPos
//! @param Container a container which has an array named @a data
template<class T, std::size_t StartPos = 0, bool = true>
struct array_length
{
	constexpr static std::size_t value = array_length<T, StartPos+1,
		is_index_in_range<T, StartPos+1>::value>::value + 1;
};

template<class T, std::size_t StartPos>
struct array_length<T, StartPos, false>
{
	constexpr static std::size_t value = 0;
};

//! Container to store the array,
//! as they can not be passed as template parameters
template <class T, T const *Data>
struct array_holder
{
	static constexpr const T* data = Data;
};

template<int const* Data>
using int_holder = array_holder<int, Data>;

constexpr int global_arr[] = { 4, 8, 15, 16, 23, 42 };

int main()
{
	std::cout << array_length<int_holder<global_arr>>::value << std::endl;
	return 0;
}