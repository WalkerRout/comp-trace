#ifndef VEC3_HPP
#define VEC3_HPP

#include <array>
#include <cmath>

#include "util.hpp"

template <typename T>
concept vec3_value_type_compatible = requires {
  requires std::is_arithmetic_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>;
  requires std::is_trivially_copyable_v<T>;
  requires std::is_trivially_default_constructible_v<T>;
};

template <vec3_value_type_compatible T> class vec3 {
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

  [[nodiscard]] constexpr auto length_squared() const noexcept -> value_type {
    return m_elems[0] * m_elems[0] + m_elems[1] * m_elems[1] + m_elems[2] * m_elems[2];
  }

  [[nodiscard]] constexpr auto length() const noexcept -> value_type
    requires(sqrt_compatible<value_type>)
  {
    return sqrt_constexpr(length_squared());
  }

  [[nodiscard]] constexpr auto operator[](const std::size_t i) noexcept -> value_type& {
    return m_elems[i];
  }
  [[nodiscard]] constexpr auto operator[](const std::size_t i) const noexcept -> value_type {
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

  // hidden friends
  [[nodiscard]] friend constexpr auto operator+(const vec3<value_type>& u,
                                                const vec3<value_type>& v) noexcept
      -> vec3<value_type> {
    return vec3<value_type>{u.x() + v.x(), u.y() + v.y(), u.z() + v.z()};
  }

  [[nodiscard]] friend constexpr auto operator-(const vec3<value_type>& v) noexcept
      -> vec3<value_type> {
    return vec3<value_type>{-v.x(), -v.y(), -v.z()};
  }

  [[nodiscard]] friend constexpr auto operator-(const vec3<value_type>& u,
                                                const vec3<value_type>& v) noexcept
      -> vec3<value_type> {
    return vec3<value_type>{u.x() - v.x(), u.y() - v.y(), u.z() - v.z()};
  }

  [[nodiscard]] friend constexpr auto operator*(const vec3<value_type>& u,
                                                const vec3<value_type>& v) noexcept
      -> vec3<value_type> {
    return vec3<value_type>{u.x() * v.x(), u.y() * v.y(), u.z() * v.z()};
  }

  [[nodiscard]] friend constexpr auto operator*(const value_type t,
                                                const vec3<value_type>& v) noexcept
      -> vec3<value_type> {
    return vec3<value_type>{t * v.x(), t * v.y(), t * v.z()};
  }

  [[nodiscard]] friend constexpr auto operator*(const vec3<value_type>& v,
                                                const value_type t) noexcept -> vec3<value_type> {
    return t * v;
  }

  [[nodiscard]] friend constexpr auto operator/(const vec3<value_type> v,
                                                const value_type t) noexcept -> vec3<value_type>
    requires(std::floating_point<value_type>)
  {
    return (value_type{1} / t) * v;
  }

private:
  std::array<value_type, 3> m_elems{};
};

template <vec3_value_type_compatible T>
[[nodiscard]] constexpr auto dot(const vec3<T>& u, const vec3<T>& v) noexcept -> T {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <vec3_value_type_compatible T>
[[nodiscard]] constexpr auto unit_vector(const vec3<T> v) noexcept -> vec3<T>
  requires(std::floating_point<T> && sqrt_compatible<T>)
{
  return v / v.length();
}

#endif // VEC3_HPP