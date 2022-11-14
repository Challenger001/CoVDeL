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

#ifndef __COVDEL_INCLUDE_COVDEL_MA_FACTORY_HH_1668068908__
#define __COVDEL_INCLUDE_COVDEL_MA_FACTORY_HH_1668068908__

#include "multiarray.hh"

namespace covdel::ma
{
  // convenience type aliases

  struct bool8 : multiarray<dtype::bool8> { };

  struct int8 : multiarray<dtype::int8> { };

  struct int16 : multiarray<dtype::int16> { };

  struct int32 : multiarray<dtype::int32> { };

  struct int64 : multiarray<dtype::int64> { };

  struct uint8 : multiarray<dtype::uint8> { };

  struct uint16 : multiarray<dtype::uint16> { };

  struct uint32 : multiarray<dtype::uint32> { };

  struct uint64 : multiarray<dtype::uint64> { };

  struct float32 : multiarray<dtype::float32> { };

  struct float64 : multiarray<dtype::float64> { };

  // multiarray factory template
  // TODO add perfect forwarding to delegate construction task without overloading
  template<typename _MultiArray = float64>
  _MultiArray array(const dimension &dim)
  {
    return _MultiArray { dim };
  }

}  // namespace covdel::ma

#endif
