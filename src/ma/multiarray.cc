#include "covdel/ma/multiarray.hh"

#include <algorithm>

namespace covdel::ma
{
  /////////////////////////////////////// SCALAR /////////////////////////////////////////

  template<typename _DType>
  scalar<_DType>::scalar(const native_type value) noexcept : m_value { value }
  { }

  template<typename _DType>
  scalar<_DType>::operator native_type() const noexcept
  {
    return m_value;
  }

  ///////////////////////////////////// MULTIARRAY ///////////////////////////////////////

  ////////////// CONSTRUCTORS //////////////

  template<typename _DType>
  multiarray<_DType>::multiarray(const dimension &dim)
    : p_data { new native_type[dim.size()] {} }, m_dim { dim }, m_is_base { true }
  { }

  template<typename _DType>
  multiarray<_DType>::multiarray(const dimension &dim, const native_type value)
    : multiarray { dim }
  {
    this->fill(value);
  }

  template<typename _DType>
  multiarray<_DType>::multiarray(const multiarray &copy)
    : p_data { copy.p_data }, m_dim { copy.m_dim }, m_is_base { false }
  { }

  template<typename _DType>
  multiarray<_DType>::multiarray(multiarray &&move) noexcept
    : p_data {}, m_dim { 0 }, m_is_base {}
  {
    this->swap(move);
  }

  template<typename _DType>
  multiarray<_DType>::~multiarray() noexcept
  {
    if (m_is_base) delete[] p_data;
  }

  //////////////// OPERATORS ///////////////

  template<typename _DType>
  multiarray<_DType> &multiarray<_DType>::operator=(multiarray other) noexcept
  {
    this->swap(other);
    return *this;
  }

  template<typename _DType>
  bool multiarray<_DType>::operator==(const multiarray &rhs) const noexcept
  {
    return m_dim == rhs.m_dim
        && std::equal(p_data, p_data + m_dim.size(), rhs.p_data, rhs.p_data + rhs.size());
  }

  template<typename _DType>
  bool multiarray<_DType>::operator!=(const multiarray &rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template<typename _DType>
  multiarray<_DType>::operator bool() const noexcept
  {
    return m_dim.size();
  }

  template<typename _DType>
  typename multiarray<_DType>::native_type &multiarray<_DType>::operator[](
    const index &idx)
  {
    return const_cast<native_type &>(const_cast<const multiarray &>(*this)[idx]);
  }

  template<typename _DType>
  const typename multiarray<_DType>::native_type &multiarray<_DType>::operator[](
    const index &idx) const
  {
    return p_data[idx.flat(m_dim)];
  }

  // FIXME add explicit instantiations?
  template<typename _DType>
  std::ostream &operator<<(std::ostream &out, const multiarray<_DType> &obj)
  {
    return out << obj.str();
  }

  ///////////////// GETTERS ////////////////

  template<typename _DType>
  const dimension &multiarray<_DType>::dim() const noexcept
  {
    return m_dim;
  }

  template<typename _DType>
  datatype multiarray<_DType>::type() const noexcept
  {
    return _DType::s_type;
  }

  template<typename _DType>
  size_t multiarray<_DType>::ndims() const noexcept
  {
    return m_dim.ndims();
  }

  template<typename _DType>
  size_t multiarray<_DType>::size() const noexcept
  {
    return m_dim.size();
  }

  template<typename _DType>
  bool multiarray<_DType>::is_base() const noexcept
  {
    return m_is_base;
  }

  // TODO finish implementing this after indexing has been added
  template<typename _DType>
  std::string multiarray<_DType>::str() const noexcept
  {
    return "<NOT_IMPLEMENTED_YET>";
  }

  ///////////////// GENERAL ////////////////

  template<typename _DType>
  template<typename _AsArray, typename _AsType>
  _AsArray multiarray<_DType>::astype() const
  {
    _AsArray out { m_dim };
    for (size_t i { -1UL }; ++i < m_dim.size();)
      out.p_data[i] = static_cast<_AsType>(p_data[i]);
    return out;
  }

  template<typename _DType>
  multiarray<_DType> multiarray<_DType>::copy() const
  {
    return astype<multiarray<_DType>>();
  }

  template<typename _DType>
  void multiarray<_DType>::swap(multiarray &other) noexcept
  {
    std::swap(p_data, other.p_data);
    m_dim.swap(other.m_dim);
    std::swap(m_is_base, other.m_is_base);
  }

  /////////// SHAPE MANIPULATION ///////////

  template<typename _DType>
  multiarray<_DType> &multiarray<_DType>::reshape(const dimension &new_dim)
  {
    if (m_dim.size() != new_dim.size())
      throw std::invalid_argument { "array size should be preserved during reshape" };
    m_dim = new_dim;
    return *this;
  }

  template<typename _DType>
  multiarray<_DType> &multiarray<_DType>::flatten()
  {
    return reshape({ m_dim.size() });
  }

  template<typename _DType>
  multiarray<_DType> &multiarray<_DType>::squeeze()
  {
    m_dim.squeeze();
    return *this;
  }

  //////////// ITEM MANIPULATION ///////////

  template<typename _DType>
  void multiarray<_DType>::fill(const native_type value)
  {
    std::fill_n(p_data, m_dim.size(), value);
  }

  //////// TEMPLATE INSTANTIATIONS /////////

  // multiarray class
  template class multiarray<dtype::bool8>;
  template class multiarray<dtype::int8>;
  template class multiarray<dtype::int16>;
  template class multiarray<dtype::int32>;
  template class multiarray<dtype::int64>;
  template class multiarray<dtype::uint8>;
  template class multiarray<dtype::uint16>;
  template class multiarray<dtype::uint32>;
  template class multiarray<dtype::uint64>;
  template class multiarray<dtype::float32>;
  template class multiarray<dtype::float64>;

  // multiarray::astype method
#define ASTYPE_INSTANTIATIONS(from_type)                            \
 template multiarray<dtype::bool8>                                  \
 multiarray<from_type>::astype<multiarray<dtype::bool8>>() const;   \
 template multiarray<dtype::int8>                                   \
 multiarray<from_type>::astype<multiarray<dtype::int8>>() const;    \
 template multiarray<dtype::int16>                                  \
 multiarray<from_type>::astype<multiarray<dtype::int16>>() const;   \
 template multiarray<dtype::int32>                                  \
 multiarray<from_type>::astype<multiarray<dtype::int32>>() const;   \
 template multiarray<dtype::int64>                                  \
 multiarray<from_type>::astype<multiarray<dtype::int64>>() const;   \
 template multiarray<dtype::uint8>                                  \
 multiarray<from_type>::astype<multiarray<dtype::uint8>>() const;   \
 template multiarray<dtype::uint16>                                 \
 multiarray<from_type>::astype<multiarray<dtype::uint16>>() const;  \
 template multiarray<dtype::uint32>                                 \
 multiarray<from_type>::astype<multiarray<dtype::uint32>>() const;  \
 template multiarray<dtype::uint64>                                 \
 multiarray<from_type>::astype<multiarray<dtype::uint64>>() const;  \
 template multiarray<dtype::float32>                                \
 multiarray<from_type>::astype<multiarray<dtype::float32>>() const; \
 template multiarray<dtype::float64>                                \
 multiarray<from_type>::astype<multiarray<dtype::float64>>() const;

  ASTYPE_INSTANTIATIONS(dtype::bool8);
  ASTYPE_INSTANTIATIONS(dtype::int8);
  ASTYPE_INSTANTIATIONS(dtype::int16);
  ASTYPE_INSTANTIATIONS(dtype::int32);
  ASTYPE_INSTANTIATIONS(dtype::int64);
  ASTYPE_INSTANTIATIONS(dtype::uint8);
  ASTYPE_INSTANTIATIONS(dtype::uint16);
  ASTYPE_INSTANTIATIONS(dtype::uint32);
  ASTYPE_INSTANTIATIONS(dtype::uint64);
  ASTYPE_INSTANTIATIONS(dtype::float32);
  ASTYPE_INSTANTIATIONS(dtype::float64);

}  // namespace covdel::ma
