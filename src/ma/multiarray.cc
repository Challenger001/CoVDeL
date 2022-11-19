#include "covdel/ma/multiarray.hh"

#include <algorithm>
#include <numeric>

namespace covdel::ma
{
  ///////////////////////////////////// DIMENSION ////////////////////////////////////////

  ////////////// CONSTRUCTORS //////////////

  dimension::dimension(const std::vector<size_t> &dim) : m_shape { dim }
  {
    if (dim.empty())
      throw std::invalid_argument { "'dim' parameter should be non-empty" };
  }

  //////////////// OPERATORS ///////////////

  dimension &dimension::operator=(std::vector<size_t> rhs) noexcept
  {
    std::swap(m_shape, rhs);
    return *this;
  }

  size_t dimension::operator[](const size_t idx) const
  {
    if (idx >= m_shape.size()) throw std::out_of_range { "index 'idx' is out of bounds" };
    return m_shape[idx];
  }

  bool dimension::operator==(const dimension &rhs) const noexcept
  {
    return m_shape == rhs.m_shape;
  }

  bool dimension::operator!=(const dimension &rhs) const noexcept
  {
    return !(*this == rhs);
  }

  std::ostream &operator<<(std::ostream &out, const dimension &obj)
  {
    return out << obj.str();
  }

  ///////////////// GETTERS ////////////////

  const std::vector<size_t> &dimension::shape() const noexcept { return m_shape; }

  size_t dimension::ndims() const noexcept { return m_shape.size(); }

  size_t dimension::size() const noexcept
  {
    return std::accumulate(
      m_shape.begin(), m_shape.end(), 1UL, std::multiplies<size_t> {});
  }

  std::string dimension::str() const noexcept
  {
    std::string out { "( " };
    for (const auto d : m_shape) out += std::to_string(d) + ' ';
    return out + ')';
  }

  ///////////////// GENERAL ////////////////

  void dimension::swap(dimension &other) noexcept { std::swap(m_shape, other.m_shape); }

  /////////////////////////////////////// INDEX //////////////////////////////////////////

  ////////////// CONSTRUCTORS //////////////

  // TODO validate the n-dimensional aspect
  index::index(const std::initializer_list<size_t> &idx) : m_index { idx } { }

  size_t index::flat(const dimension &dim) const
  {
    auto d_begin { dim.shape().rbegin() };
    size_t stride { 1 };
    return std::accumulate(m_index.rbegin() + 1, m_index.rend(), m_index.back(),
      [&](const size_t flat_idx, const size_t i) {
        stride *= *d_begin++;
        return flat_idx + i * stride;
      });
  }

  bool index::operator<(const dimension &dim) const
  {
    auto d_begin { dim.shape().begin() };
    return std::accumulate(m_index.begin(), m_index.end(), true,
      [&](const bool out, const size_t i) { return out && i < *d_begin++; });
  }

  bool index::operator>=(const dimension &dim) const { return !(*this < dim); }

  size_t index::ndims() const noexcept { return m_index.size(); }

  ///////////////////////////////////// MULTIARRAY ///////////////////////////////////////

  ////////////// CONSTRUCTORS //////////////

  template<typename _DType>
  multiarray<_DType>::multiarray(const dimension &dim)
    : p_data { new value_type[dim.size()] {} }, m_dim { dim }, m_is_base { true }
  { }

  template<typename _DType>
  multiarray<_DType>::multiarray(const dimension &dim, const value_type value)
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
  typename multiarray<_DType>::value_type &multiarray<_DType>::operator[](
    const index &idx)
  {
    return const_cast<value_type &>(const_cast<const multiarray &>(*this)[idx]);
  }

  template<typename _DType>
  const typename multiarray<_DType>::value_type &multiarray<_DType>::operator[](
    const index &idx) const
  {
    if (!(idx.ndims() == m_dim.ndims() && idx < m_dim))
      throw std::invalid_argument { "index 'idx' is incompatible / incomplete" };
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
  constexpr datatype multiarray<_DType>::type() const noexcept
  {
    return _DType::s_type;
  }

  template<typename _DType>
  const std::vector<size_t> &multiarray<_DType>::shape() const noexcept
  {
    return m_dim.shape();
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
    auto shape { m_dim.shape() };
    shape.erase(std::remove(shape.begin(), shape.end(), 1), shape.end());
    m_dim = shape;
    return *this;
  }

  //////////// ITEM MANIPULATION ///////////

  template<typename _DType>
  void multiarray<_DType>::fill(const value_type value)
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
  // bool8
  template multiarray<dtype::bool8>
  multiarray<dtype::bool8>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::bool8>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::bool8>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::bool8>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::bool8>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::bool8>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::bool8>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::bool8>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::bool8>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::bool8>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::bool8>::astype<multiarray<dtype::float64>>() const;
  // int8
  template multiarray<dtype::bool8>
  multiarray<dtype::int8>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::int8>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::int8>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::int8>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::int8>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::int8>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::int8>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::int8>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::int8>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::int8>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::int8>::astype<multiarray<dtype::float64>>() const;
  // int16
  template multiarray<dtype::bool8>
  multiarray<dtype::int16>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::int16>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::int16>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::int16>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::int16>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::int16>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::int16>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::int16>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::int16>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::int16>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::int16>::astype<multiarray<dtype::float64>>() const;
  // int32
  template multiarray<dtype::bool8>
  multiarray<dtype::int32>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::int32>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::int32>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::int32>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::int32>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::int32>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::int32>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::int32>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::int32>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::int32>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::int32>::astype<multiarray<dtype::float64>>() const;
  // int64
  template multiarray<dtype::bool8>
  multiarray<dtype::int64>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::int64>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::int64>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::int64>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::int64>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::int64>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::int64>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::int64>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::int64>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::int64>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::int64>::astype<multiarray<dtype::float64>>() const;
  // uint8
  template multiarray<dtype::bool8>
  multiarray<dtype::uint8>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::uint8>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::uint8>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::uint8>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::uint8>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::uint8>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::uint8>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::uint8>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::uint8>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::uint8>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::uint8>::astype<multiarray<dtype::float64>>() const;
  // uint16
  template multiarray<dtype::bool8>
  multiarray<dtype::uint16>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::uint16>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::uint16>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::uint16>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::uint16>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::uint16>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::uint16>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::uint16>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::uint16>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::uint16>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::uint16>::astype<multiarray<dtype::float64>>() const;
  // uint32
  template multiarray<dtype::bool8>
  multiarray<dtype::uint32>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::uint32>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::uint32>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::uint32>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::uint32>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::uint32>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::uint32>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::uint32>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::uint32>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::uint32>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::uint32>::astype<multiarray<dtype::float64>>() const;
  // uint64
  template multiarray<dtype::bool8>
  multiarray<dtype::uint64>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::uint64>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::uint64>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::uint64>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::uint64>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::uint64>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::uint64>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::uint64>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::uint64>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::uint64>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::uint64>::astype<multiarray<dtype::float64>>() const;
  // float32
  template multiarray<dtype::bool8>
  multiarray<dtype::float32>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::float32>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::float32>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::float32>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::float32>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::float32>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::float32>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::float32>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::float32>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::float32>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::float32>::astype<multiarray<dtype::float64>>() const;
  // float64
  template multiarray<dtype::bool8>
  multiarray<dtype::float64>::astype<multiarray<dtype::bool8>>() const;
  template multiarray<dtype::int8>
  multiarray<dtype::float64>::astype<multiarray<dtype::int8>>() const;
  template multiarray<dtype::int16>
  multiarray<dtype::float64>::astype<multiarray<dtype::int16>>() const;
  template multiarray<dtype::int32>
  multiarray<dtype::float64>::astype<multiarray<dtype::int32>>() const;
  template multiarray<dtype::int64>
  multiarray<dtype::float64>::astype<multiarray<dtype::int64>>() const;
  template multiarray<dtype::uint8>
  multiarray<dtype::float64>::astype<multiarray<dtype::uint8>>() const;
  template multiarray<dtype::uint16>
  multiarray<dtype::float64>::astype<multiarray<dtype::uint16>>() const;
  template multiarray<dtype::uint32>
  multiarray<dtype::float64>::astype<multiarray<dtype::uint32>>() const;
  template multiarray<dtype::uint64>
  multiarray<dtype::float64>::astype<multiarray<dtype::uint64>>() const;
  template multiarray<dtype::float32>
  multiarray<dtype::float64>::astype<multiarray<dtype::float32>>() const;
  template multiarray<dtype::float64>
  multiarray<dtype::float64>::astype<multiarray<dtype::float64>>() const;

}  // namespace covdel::ma
