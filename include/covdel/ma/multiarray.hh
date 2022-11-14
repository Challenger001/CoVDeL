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

#ifndef __COVDEL_INCLUDE_COVDEL_MA_MULTIARRAY_HH_1667987582__
#define __COVDEL_INCLUDE_COVDEL_MA_MULTIARRAY_HH_1667987582__

#include "datatype.hh"

#include <initializer_list>
#include <memory>
#include <vector>

namespace covdel::ma
{
  using std::size_t;

  class dimension {  // 24B
  public:
    // constructors
    dimension(const std::initializer_list<size_t> &dim);

    // operators
    size_t operator[](const size_t idx) const;
    bool operator==(const dimension &rhs) const noexcept;
    bool operator!=(const dimension &rhs) const noexcept;
    operator bool() const noexcept;
    friend std::ostream &operator<<(std::ostream &out, const dimension &obj);

    // getters
    const std::vector<size_t> &shape() const noexcept;
    size_t ndims() const noexcept;
    size_t size() const noexcept;
    std::string str() const noexcept;

  private:
    std::vector<size_t> m_shape;
  };

  template<typename _DType>
  class multiarray {  // 40B
    using value_type = std::enable_if_t<dtype::is_valid<_DType>, typename _DType::type>;

  public:
    // constructors
    multiarray(const dimension &dim);
    multiarray(const dimension &dim, const value_type fill);
    multiarray(const multiarray &copy)     = default;
    multiarray(multiarray &&move) noexcept = default;

    // operators
    operator bool() const noexcept;

    // getters
    constexpr datatype type() const noexcept;
    const std::vector<size_t> &shape() const noexcept;
    size_t ndims() const noexcept;
    size_t size() const noexcept;
    std::string str() const noexcept;

  private:
    std::shared_ptr<value_type[]> p_data;
    dimension m_dim;
  };

}  // namespace covdel::ma

#endif
