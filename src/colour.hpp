#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "vec3.hpp"

template <typename T>
concept colour_value_type_compatible = requires {
  requires vec3_value_type_compatible<T>;
  requires std::is_trivially_copyable_v<T>;
  requires std::is_trivially_default_constructible_v<T>;
};

template <colour_value_type_compatible T> class colour {
public:
  using value_type = T;

  [[nodiscard]] constexpr colour() noexcept = default;
  [[nodiscard]] constexpr colour(const value_type r, const value_type g,
                                 const value_type b) noexcept
      : m_rgb{r, g, b} {}
  [[nodiscard]] constexpr explicit colour(const vec3<value_type> v) noexcept : m_rgb{v} {}

  [[nodiscard]] constexpr auto r() const noexcept -> value_type {
    return m_rgb.x();
  }
  [[nodiscard]] constexpr auto g() const noexcept -> value_type {
    return m_rgb.y();
  }
  [[nodiscard]] constexpr auto b() const noexcept -> value_type {
    return m_rgb.z();
  }

  // hidden friends
  [[nodiscard]] friend constexpr auto operator+(const colour<T>& c1, const colour<T>& c2) noexcept
      -> colour<T> {
    return colour<T>{c1.m_rgb + c2.m_rgb};
  }

  [[nodiscard]] friend constexpr auto operator*(const T t, const colour<T>& c) noexcept
      -> colour<T> {
    return colour<T>{t * c.m_rgb};
  }

  [[nodiscard]] friend constexpr auto operator*(const colour<T>& c, const T t) noexcept
      -> colour<T> {
    return colour<T>{c.m_rgb * t};
  }

private:
  vec3<value_type> m_rgb;
};

using colour_d = colour<double>;

#endif // COLOUR_HPP