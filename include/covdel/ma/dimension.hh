#ifndef __COVDEL_INCLUDE_COVDEL_MA_DIMENSION_HH_1668933640__
#define __COVDEL_INCLUDE_COVDEL_MA_DIMENSION_HH_1668933640__

#include <ostream>

namespace covdel::ma
{
  using std::size_t;

  class _dsi {  // 16B
  public:
    // copy-move semantics
    _dsi(const _dsi &copy);
    _dsi(_dsi &&move) noexcept;
    ~_dsi() noexcept;
    _dsi &operator=(_dsi rhs) noexcept;

    // operators
    size_t operator[](const int idx) const;
    bool operator==(const _dsi &rhs) const noexcept;
    bool operator!=(const _dsi &rhs) const noexcept;
    friend std::ostream &operator<<(std::ostream &out, const _dsi &obj);

    //  general
    int ndims() const noexcept;
    std::string str() const;
    void swap(_dsi &other) noexcept;

  protected:
    template<typename... _Args,
      typename = std::enable_if_t<(std::is_integral_v<_Args> && ...)>>
    _dsi(_Args... args);

    static constexpr int MAX_SIZE { 6 };
    size_t *p_data;
    int m_len;
  };

  class dimension : public _dsi {  // 16B
  public:
    template<typename... _Args>
    dimension(_Args... args);

    //  general
    size_t size() const noexcept;
    void squeeze();
  };

  class index : public _dsi {  // 16B
  public:
    template<typename... _Args>
    index(_Args... args);

    // general
    size_t flat(const dimension &dim) const;
    bool operator<(const dimension &dim) const;
    bool operator>=(const dimension &dim) const;
  };

  // in-header definitions

  template<typename... _Args, typename>
  _dsi::_dsi(_Args... args) : p_data { new size_t[MAX_SIZE] {} }, m_len { 0 }
  {
    if (sizeof...(args) == 0 || sizeof...(args) > MAX_SIZE)
      throw std::invalid_argument { "no. of arguments must be non-zero and less than 6" };
    ((p_data[m_len++] = args), ...);
  }

  template<typename... _Args>
  dimension::dimension(_Args... args) : _dsi { args... }
  { }

  template<typename... _Args>
  index::index(_Args... args) : _dsi { args... }
  { }
}  // namespace covdel::ma

#endif
