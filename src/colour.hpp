#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "vec3.hpp"

template <typename T>
concept colour_compatible = vec3_compatible<T>;

template <colour_compatible T> struct colour {
  static_assert(std::is_trivially_copyable_v<T>, "colour requires trivially copyable types");
  static_assert(std::is_trivially_default_constructible_v<T>,
                "colour requires trivially default‐constructible types");

  using value_type = T;

  [[nodiscard]] constexpr colour() noexcept = default;
  [[nodiscard]] constexpr colour(T r, T g, T b) noexcept : m_rgb{r, g, b} {}
  [[nodiscard]] constexpr explicit colour(vec3<T> v) noexcept : m_rgb{v} {}

  [[nodiscard]] constexpr auto r() const noexcept -> T {
    return m_rgb.x();
  }
  [[nodiscard]] constexpr auto g() const noexcept -> T {
    return m_rgb.y();
  }
  [[nodiscard]] constexpr auto b() const noexcept -> T {
    return m_rgb.z();
  }

  vec3<T> m_rgb;
};

template <colour_compatible T>
[[nodiscard]] constexpr auto operator+(const colour<T>& c1, const colour<T>& c2) noexcept
    -> colour<T> {
  return colour<T>{c1.m_rgb + c2.m_rgb};
}

template <colour_compatible T>
[[nodiscard]] constexpr auto operator*(T t, const colour<T>& c) noexcept -> colour<T> {
  return colour<T>{t * c.m_rgb};
}

template <colour_compatible T>
[[nodiscard]] constexpr auto operator*(const colour<T>& c, T t) noexcept -> colour<T> {
  return colour<T>{c.m_rgb * t};
}

using colour_d = colour<double>;

#endif // COLOUR_HPP