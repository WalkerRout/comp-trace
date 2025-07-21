#ifndef RAY_HPP
#define RAY_HPP

#include <optional>

#include "point3.hpp"
#include "vec3.hpp"

template <typename T>
concept ray_compatible = std::floating_point<T> && point3_compatible<T> && vec3_compatible<T>;

template <ray_compatible T> class ray {
  static_assert(std::is_trivially_copyable_v<T>, "ray requires trivially copyable types");
  static_assert(std::is_trivially_default_constructible_v<T>,
                "ray requires trivially default‐constructible types");

public:
  using value_type = T;

  [[nodiscard]] constexpr ray() noexcept = default;
  [[nodiscard]] constexpr ray(const point3<value_type>& origin,
                              const vec3<value_type>& direction) noexcept
      : m_origin(origin), m_direction(direction) {}

  [[nodiscard]] constexpr auto origin() const noexcept -> point3<value_type> {
    return m_origin;
  }
  [[nodiscard]] constexpr auto direction() const noexcept -> vec3<value_type> {
    return m_direction;
  }

  [[nodiscard]] constexpr auto at(const value_type t) const noexcept -> point3<value_type> {
    return m_origin + t * m_direction;
  }

private:
  point3<value_type> m_origin;
  vec3<value_type> m_direction;
};

using ray_d = ray<double>;

template <ray_compatible T> struct hit_record {
  // ray_compatible IS point3 and vec3 compatible...
  point3<T> p;
  vec3<T> normal;
  double t;
  bool front_face;

  constexpr void set_face_normal(const ray<T>& r, const vec3<T>& outward_normal) noexcept {
    front_face = dot(r.direction(), outward_normal) < T{0};
    normal = front_face ? outward_normal : -outward_normal;
  }
};

using hit_record_d = hit_record<double>;

#endif // RAY_HPP