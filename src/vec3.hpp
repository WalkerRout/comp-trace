#ifndef VEC3_HPP
#define VEC3_HPP

#include <array>
#include <cmath>

#include "util.hpp"

template <typename T>
concept vec3_compatible =
    std::is_arithmetic_v<T> && !std::same_as<std::remove_cvref_t<T>, bool> && requires(T a, T b) {
      { a + b } -> std::convertible_to<T>;
      { a - b } -> std::convertible_to<T>;
      { a * b } -> std::convertible_to<T>;
      { a / b } -> std::convertible_to<T>;
    };

template <vec3_compatible T> class vec3 {
  static_assert(std::is_trivially_copyable_v<T>, "vec3 requires trivially copyable types");
  static_assert(std::is_trivially_default_constructible_v<T>,
                "vec3 requires trivially default‐constructible types");

public:
  using value_type = T;
  using size_type = std::size_t;

  [[nodiscard]] constexpr vec3() noexcept = default;
  [[nodiscard]] constexpr vec3(const value_type x, const value_type y, const value_type z) noexcept
      : m_elems{x, y, z} {}

  [[nodiscard]] constexpr auto x() const noexcept -> value_type {
    return m_elems[0];
  }
  [[nodiscard]] constexpr auto y() const noexcept -> value_type {
    return m_elems[1];
  }
  [[nodiscard]] constexpr auto z() const noexcept -> value_type {
    return m_elems[2];
  }

  [[nodiscard]] constexpr auto operator[](const std::size_t i) const noexcept -> value_type {
    return m_elems[i];
  }
  [[nodiscard]] constexpr auto operator[](const std::size_t i) noexcept -> value_type& {
    return m_elems[i];
  }

  constexpr auto operator+=(const vec3<value_type>& v) noexcept -> vec3<value_type>& {
    m_elems[0] += v.m_elems[0];
    m_elems[1] += v.m_elems[1];
    m_elems[2] += v.m_elems[2];
    return *this;
  }

  constexpr auto operator*=(const value_type t) noexcept -> vec3<value_type>& {
    m_elems[0] *= t;
    m_elems[1] *= t;
    m_elems[2] *= t;
    return *this;
  }

  constexpr auto operator/=(const value_type t) noexcept -> vec3<value_type>&
      // full stop prevent integer division issues for now...
    requires(!std::integral<value_type>)
  {
    return *this *= value_type{1} / t;
  }

  [[nodiscard]] constexpr auto length_squared() const noexcept -> value_type {
    return m_elems[0] * m_elems[0] + m_elems[1] * m_elems[1] + m_elems[2] * m_elems[2];
  }

  [[nodiscard]] constexpr auto length() const noexcept -> value_type
    requires(sqrt_compatible<value_type>)
  {
    return sqrt_constexpr(length_squared());
  }

private:
  std::array<T, 3> m_elems{};
};

template <vec3_compatible T>
[[nodiscard]] constexpr auto operator+(const vec3<T>& u, const vec3<T>& v) noexcept -> vec3<T> {
  return vec3<T>{u.x() + v.x(), u.y() + v.y(), u.z() + v.z()};
}

template <vec3_compatible T>
[[nodiscard]] constexpr auto operator-(const vec3<T>& v) noexcept -> vec3<T> {
  return vec3<T>(-v.x(), -v.y(), -v.z());
}

template <vec3_compatible T>
[[nodiscard]] constexpr auto operator-(const vec3<T>& u, const vec3<T>& v) noexcept -> vec3<T> {
  return vec3<T>{u.x() - v.x(), u.y() - v.y(), u.z() - v.z()};
}

template <vec3_compatible T>
[[nodiscard]] constexpr auto operator*(const vec3<T>& u, const vec3<T>& v) noexcept -> vec3<T> {
  return vec3<T>{u.x() * v.x(), u.y() * v.y(), u.z() * v.z()};
}

template <vec3_compatible T>
[[nodiscard]] constexpr auto operator*(const T t, const vec3<T>& v) noexcept -> vec3<T> {
  return vec3<T>{t * v.x(), t * v.y(), t * v.z()};
}

template <vec3_compatible T>
[[nodiscard]] constexpr auto operator*(const vec3<T>& v, const T t) noexcept -> vec3<T> {
  return t * v;
}

template <vec3_compatible T>
[[nodiscard]] constexpr auto operator/(const vec3<T> v, const T t) noexcept -> vec3<T>
  requires(std::floating_point<T>)
{
  return (1 / t) * v;
}

template <vec3_compatible T>
[[nodiscard]] constexpr auto dot(const vec3<T>& u, const vec3<T>& v) noexcept -> T {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <vec3_compatible T>
[[nodiscard]] constexpr auto unit_vector(const vec3<T> v) noexcept -> vec3<T>
  requires(std::floating_point<T> && sqrt_compatible<T>)
{
  return v / v.length();
}

#endif // VEC3_HPP