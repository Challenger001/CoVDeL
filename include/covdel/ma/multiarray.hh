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
    dimension(const std::vector<size_t> &dim);

    template<typename... _Args,
      std::enable_if_t<std::conjunction_v<std::is_integral<_Args>...>> * = nullptr>
    dimension(_Args... args)
      : dimension { std::vector<size_t> { static_cast<size_t>(args)... } }
    { }

    // operators
    dimension &operator=(std::vector<size_t> rhs) noexcept;
    size_t operator[](const size_t idx) const;
    bool operator==(const dimension &rhs) const noexcept;
    bool operator!=(const dimension &rhs) const noexcept;
    friend std::ostream &operator<<(std::ostream &out, const dimension &obj);

    // getters
    const std::vector<size_t> &shape() const noexcept;
    size_t ndims() const noexcept;
    size_t size() const noexcept;
    std::string str() const noexcept;

    // general
    void swap(dimension &other) noexcept;

  private:
    std::vector<size_t> m_shape;
  };

  class index {
  public:
    index(const std::initializer_list<size_t> &idx);
    size_t flat(const dimension &dim) const;
    bool operator<(const dimension &dim) const;
    bool operator>=(const dimension &dim) const;
    size_t ndims() const noexcept;

  private:
    std::vector<size_t> m_index;
  };

  template<typename _DType>
  class multiarray {  // 40B
  public:
    using value_type = std::enable_if_t<dtype::is_valid<_DType>, typename _DType::type>;

    // constructors
    multiarray(const dimension &dim);
    multiarray(const dimension &dim, const value_type fill);
    multiarray(const multiarray &copy);
    multiarray(multiarray &&move) noexcept;
    ~multiarray() noexcept;

    // operators
    multiarray &operator=(multiarray other) noexcept;
    bool operator==(const multiarray &rhs) const noexcept;
    bool operator!=(const multiarray &rhs) const noexcept;
    operator bool() const noexcept;
    value_type &operator[](const index &idx);
    const value_type &operator[](const index &idx) const;
    friend std::ostream &operator<<(std::ostream &out, const multiarray &obj);

    // getters
    constexpr datatype type() const noexcept;
    const dimension &dim() const noexcept;
    const std::vector<size_t> &shape() const noexcept;
    size_t ndims() const noexcept;
    size_t size() const noexcept;
    bool is_base() const noexcept;
    std::string str() const noexcept;

    // general methods
    template<typename _AsArray, typename _AsType = typename _AsArray::value_type>
    _AsArray astype() const;
    multiarray copy() const;
    void swap(multiarray &other) noexcept;

    // shape manipulation methods
    multiarray &reshape(const dimension &new_dim);
    multiarray &flatten();
    multiarray &squeeze();

    // item manipulation methods
    void fill(const value_type value);

  private:
    value_type *p_data;
    dimension m_dim;
    bool m_is_base;

    template<typename _OtherType>
    friend class multiarray;
  };

}  // namespace covdel::ma

#endif
