// Copyright (C) 2022 Dasu Pradyumna
//
// This file is part of CoVDeL.
//
// CoVDeL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CoVDeL is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CoVDeL.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __COVDEL_INCLUDE_COVDEL_MA_DATATYPE_HH_1667987916__
#define __COVDEL_INCLUDE_COVDEL_MA_DATATYPE_HH_1667987916__

#include <climits>
#include <cstdint>
#include <type_traits>

// checking floating point widths
// TODO add automatic datatype adjustment in case of failure
static_assert(sizeof(float) * CHAR_BIT == 32, "'float' type should be 32 bits\n");
static_assert(sizeof(double) * CHAR_BIT == 64, "'double' type should be 64 bits\n");

namespace covdel::ma
{
  // supported datatype labels
  enum class datatype {
    // boolean
    bool8,
    // signed integers
    int8,
    int16,
    int32,
    int64,
    // unsigned integers
    uint8,
    uint16,
    uint32,
    uint64,
    // floating point numbers
    float32,
    float64
  };

  namespace dtype
  {
    // base class for type validation of supported datatypes
    struct _dtype { };

// supported datatype subclass definition
#define DTYPE(type_name, native_type)                       \
 struct type_name : _dtype {                                \
  using type = native_type;                                 \
  static constexpr datatype s_type { datatype::type_name }; \
 }

    DTYPE(bool8, bool);
    DTYPE(int8, std::int8_t);
    DTYPE(int16, std::int16_t);
    DTYPE(int32, std::int32_t);
    DTYPE(int64, std::int64_t);
    DTYPE(uint8, std::uint8_t);
    DTYPE(uint16, std::uint16_t);
    DTYPE(uint32, std::uint32_t);
    DTYPE(uint64, std::uint64_t);
    DTYPE(float32, float);
    DTYPE(float64, double);

    // compile-time type validation helper
    template<typename _Type>
    static constexpr bool is_valid {
      std::is_base_of_v<_dtype, _Type> && !std::is_same_v<_dtype, _Type>
    };

  }  // namespace dtype

}  // namespace covdel::ma

#endif
