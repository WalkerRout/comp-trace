#ifndef POINT3_HPP
#define POINT3_HPP

#include "vec3.hpp"

namespace rt {

template <typename T>
concept point3_value_type_compatible = requires {
  requires vec3_value_type_compatible<T>;
  requires std::is_trivially_copyable_v<T>;
  requires std::is_trivially_default_constructible_v<T>;
};

template <point3_value_type_compatible T> class point3 {
public:
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

  // hidden friends
  [[nodiscard]] friend constexpr auto operator+(const point3& p, const vec3<T>& v) noexcept
      -> point3 {
    return point3{p.m_coords + v};
  }

  [[nodiscard]] friend constexpr auto operator-(const point3& p1, const point3& p2) noexcept
      -> vec3<T> {
    return p1.m_coords - p2.m_coords;
  }

  [[nodiscard]] friend constexpr auto operator-(const point3& p, const vec3<T>& v) noexcept
      -> point3 {
    return point3{p.m_coords - v};
  }

private:
  vec3<value_type> m_coords;
};

using point3_d = point3<double>;

} // namespace rt

#endif // POINT3_HPP