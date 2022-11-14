#include "covdel/ma/multiarray.hh"

#include <algorithm>
#include <numeric>

namespace covdel::ma
{
  ///////////////////////////////////// DIMENSION ////////////////////////////////////////

  ////////////// CONSTRUCTORS //////////////

  dimension::dimension(const std::initializer_list<size_t> &dim) : m_shape { dim }
  {
    if (!dim.size())
      throw std::invalid_argument { "'dim' parameter should be non-empty" };
  }

  //////////////// OPERATORS ///////////////

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

  dimension::operator bool() const noexcept { return size(); }

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

  ///////////////////////////////////// MULTIARRAY ///////////////////////////////////////

  ////////////// CONSTRUCTORS //////////////

  template<typename _DType>
  multiarray<_DType>::multiarray(const dimension &dim)
    : p_data { std::make_shared<value_type[]>(dim.size()) }, m_dim(dim)
  { }

  template<typename _DType>
  multiarray<_DType>::multiarray(const dimension &dim, const value_type fill)
    : multiarray { dim }
  {
    std::fill_n(p_data.get(), m_dim.size(), fill);
  }

  //////////////// OPERATORS ///////////////

  template<typename _DType>
  multiarray<_DType>::operator bool() const noexcept
  {
    return m_dim;
  }

  // FIXME add explicit instantiations?
  template<typename _DType>
  std::ostream &operator<<(std::ostream &out, const multiarray<_DType> &obj)
  {
    return out << obj.str();
  }

  ///////////////// GETTERS ////////////////

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

  // TODO finish implementing this after indexing has been added
  template<typename _DType>
  std::string multiarray<_DType>::str() const noexcept
  {
    return "<NOT_IMPLEMENTED_YET>";
  }

  // explicit template instantiations

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

}  // namespace covdel::ma
