#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cassert>
#include <limits>
#include <optional>

#include "point3.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "vec3.hpp"

template <typename T>
concept sphere_compatible = std::floating_point<T> && point3_compatible<T>;

template <sphere_compatible T> class sphere {
  static_assert(std::is_trivially_copyable_v<T>, "sphere requires trivially copyable types");
  static_assert(std::is_trivially_default_constructible_v<T>,
                "sphere requires trivially default‐constructible types");

public:
  using value_type = T;

  [[nodiscard]] constexpr sphere() noexcept = default;
  // radius must be positive
  [[nodiscard]] constexpr sphere(const point3<value_type>& center, const value_type radius) noexcept
      : m_center(center), m_radius(radius) {}

  [[nodiscard]] constexpr auto center() const noexcept -> point3<value_type> {
    return m_center;
  }
  [[nodiscard]] constexpr auto radius() const noexcept -> value_type {
    return m_radius;
  }

  [[nodiscard]] constexpr auto hit(const ray<value_type>& r, const value_type t_min,
                                   const value_type t_max) const noexcept
      -> std::optional<hit_record<value_type>> {
    // early validation
    if (!(t_min < t_max)) {
      return std::nullopt;
    }

    const vec3<value_type> oc = r.origin() - m_center;
    const value_type a = dot(r.direction(), r.direction());
    const value_type half_b = dot(oc, r.direction());
    const value_type c = dot(oc, oc) - m_radius * m_radius;

    const value_type discriminant = half_b * half_b - a * c;
    if (discriminant < value_type{0}) {
      return std::nullopt;
    }

    const value_type sqrtd = sqrt_constexpr(discriminant);

    // nearest root in range
    value_type root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
      root = (-half_b + sqrtd) / a;
      if (root < t_min || root > t_max) {
        return std::nullopt;
      }
    }

    hit_record<value_type> rec;
    rec.t = root;
    rec.p = r.at(root);
    rec.set_face_normal(r, (rec.p - m_center) / m_radius);

    return rec;
  }

private:
  point3<value_type> m_center;
  value_type m_radius;
};

using sphere_d = sphere<double>;

// we should be able to move sphere around wherever we want...
static_assert(std::is_trivially_copyable_v<sphere_d>, "sphere_d must be trivially copyable");

#endif // SPHERE_HPP