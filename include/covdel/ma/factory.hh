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
  // concise type aliases
  using D = dimension;
  using I = index;

  // convenience type aliases

  using bool8   = multiarray<dtype::bool8>;
  using int8    = multiarray<dtype::int8>;
  using int16   = multiarray<dtype::int16>;
  using int32   = multiarray<dtype::int32>;
  using int64   = multiarray<dtype::int64>;
  using uint8   = multiarray<dtype::uint8>;
  using uint16  = multiarray<dtype::uint16>;
  using uint32  = multiarray<dtype::uint32>;
  using uint64  = multiarray<dtype::uint64>;
  using float32 = multiarray<dtype::float32>;
  using float64 = multiarray<dtype::float64>;

  // multiarray factory template
  template<typename _MultiArray, typename... _Args>
  _MultiArray array(_Args &&...args)
  {
    return _MultiArray(std::forward<_Args>(args)...);
  }

}  // namespace covdel::ma

#endif
