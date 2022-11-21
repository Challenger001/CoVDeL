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
#include "dimension.hh"

namespace covdel::ma
{
  template<typename _DType>
  class scalar {  // 1-8B
  public:
    using native_type = std::enable_if_t<dtype::is_valid<_DType>, typename _DType::type>;

    scalar() = default;
    scalar(const native_type value) noexcept;
    operator native_type() const noexcept;

  private:
    native_type m_value;
  };

  template<typename _DType>
  class multiarray {  // 32B
  public:
    using native_type = std::enable_if_t<dtype::is_valid<_DType>, typename _DType::type>;

    // constructors
    multiarray(const dimension &dim);
    multiarray(const dimension &dim, const native_type fill);
    multiarray(const multiarray &copy);
    multiarray(multiarray &&move) noexcept;
    ~multiarray() noexcept;

    // operators
    multiarray &operator=(multiarray other) noexcept;
    bool operator==(const multiarray &rhs) const noexcept;
    bool operator!=(const multiarray &rhs) const noexcept;
    operator bool() const noexcept;
    native_type &operator[](const index &idx);
    const native_type &operator[](const index &idx) const;
    friend std::ostream &operator<<(std::ostream &out, const multiarray &obj);

    // getters
    datatype type() const noexcept;
    const dimension &dim() const noexcept;
    size_t ndims() const noexcept;
    size_t size() const noexcept;
    bool is_base() const noexcept;
    std::string str() const noexcept;

    // general
    template<typename _AsArray, typename _AsType = typename _AsArray::native_type>
    _AsArray astype() const;
    multiarray copy() const;
    void swap(multiarray &other) noexcept;

    // shape manipulation
    multiarray &reshape(const dimension &new_dim);
    multiarray &flatten();
    multiarray &squeeze();

    // item manipulation
    void fill(const native_type value);

  private:
    native_type *p_data;
    dimension m_dim;
    bool m_is_base;

    template<typename _OtherType>
    friend class multiarray;
  };

}  // namespace covdel::ma

#endif
