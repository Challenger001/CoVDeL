#include "covdel/ma/dimension.hh"

#include <numeric>

namespace covdel::ma
{
  //////////////////////////////////////// _DSI //////////////////////////////////////////

  ////////////// CONSTRUCTORS //////////////

  _dsi::_dsi(const _dsi &copy) : p_data { new size_t[MAX_SIZE] {} }, m_len { copy.m_len }
  {
    std::copy(copy.p_data, copy.p_data + m_len, p_data);
  }

  _dsi::_dsi(_dsi &&move) noexcept : p_data { move.p_data }, m_len { move.m_len }
  {
    move.p_data = nullptr;
  }

  _dsi::~_dsi() noexcept { delete[] p_data; }

  //////////////// OPERATORS ///////////////

  _dsi &_dsi::operator=(_dsi rhs) noexcept
  {
    this->swap(rhs);
    return *this;
  }

  size_t _dsi::operator[](const int idx) const
  {
    return idx < m_len ? p_data[idx] : throw std::out_of_range { "index out of bounds" };
  }

  bool _dsi::operator==(const _dsi &rhs) const noexcept
  {
    return m_len == rhs.m_len && std::equal(p_data, p_data + m_len, rhs.p_data);
  }

  bool _dsi::operator!=(const _dsi &rhs) const noexcept { return !(*this == rhs); }

  std::ostream &operator<<(std::ostream &out, const _dsi &obj)
  {
    return out << obj.str();
  }

  ///////////////// GENERAL ////////////////

  void _dsi::swap(_dsi &b) noexcept
  {
    std::swap(m_len, b.m_len);
    std::swap(p_data, b.p_data);
  }

  int _dsi::ndims() const noexcept { return m_len; }

  std::string _dsi::str() const
  {
    std::string out { "( " };
    for (int i { -1 }; ++i < m_len;) out += std::to_string(p_data[i]) + ' ';
    return out + ')';
  }

  ///////////////////////////////////// DIMENSION ////////////////////////////////////////

  size_t dimension::size() const noexcept
  {
    return std::accumulate(p_data, p_data + m_len, 1UL, std::multiplies<size_t> {});
  }

  void dimension::squeeze()
  {
    int i { -1 }, j { 0 };
    while (++i < m_len)
      if (p_data[i] != 1) p_data[j++] = p_data[i];
    m_len = j;
  }

  /////////////////////////////////////// INDEX //////////////////////////////////////////

  size_t index::flat(const dimension &dim) const
  {
    if (*this >= dim) throw std::out_of_range { "index out of corresponding dimension" };

    size_t stride { 1 }, flat_idx { p_data[m_len - 1] };
    for (int i { m_len }; --i > 0;) {
      stride *= dim[i];
      flat_idx += stride * p_data[i - 1];
    }
    return flat_idx;
  }

  bool index::operator<(const dimension &dim) const
  {
    if (m_len != dim.ndims())
      throw std::invalid_argument { "index is incompatible with given dimension" };

    for (int i { -1 }; ++i < m_len;)
      if (p_data[i] >= dim[i]) return false;
    return true;
  }

  bool index::operator>=(const dimension &dim) const { return !(*this < dim); }

}  // namespace covdel::ma
