#ifndef POINT3_HPP
#define POINT3_HPP

#include "vec3.hpp"

template <typename T>
concept point3_compatible = vec3_compatible<T>;

template <point3_compatible T> struct point3 {
  static_assert(std::is_trivially_copyable_v<T>, "point3 requires trivially copyable types");
  static_assert(std::is_trivially_default_constructible_v<T>,
                "point3 requires trivially default‐constructible types");

  using value_type = T;

  [[nodiscard]] constexpr point3() noexcept = default;
  [[nodiscard]] constexpr point3(const value_type x, const value_type y,
                                 const value_type z) noexcept
      : m_coords{x, y, z} {}
  [[nodiscard]] constexpr explicit point3(const vec3<value_type> v) noexcept : m_coords{v} {}

  [[nodiscard]] constexpr auto x() const noexcept -> value_type {
    return m_coords.x();
  }
  [[nodiscard]] constexpr auto y() const noexcept -> value_type {
    return m_coords.y();
  }
  [[nodiscard]] constexpr auto z() const noexcept -> value_type {
    return m_coords.z();
  }

  vec3<value_type> m_coords;
};

template <point3_compatible T>
[[nodiscard]] constexpr auto operator+(const point3<T>& p, const vec3<T>& v) noexcept -> point3<T> {
  return point3<T>{p.m_coords + v};
}

template <point3_compatible T>
[[nodiscard]] constexpr auto operator-(const point3<T>& p1, const point3<T>& p2) noexcept
    -> vec3<T> {
  return p1.m_coords - p2.m_coords;
}

template <point3_compatible T>
[[nodiscard]] constexpr auto operator-(const point3<T>& p, const vec3<T>& v) noexcept -> point3<T> {
  return point3<T>{p.m_coords - v};
}

using point3_d = point3<double>;

#endif // POINT3_HPP