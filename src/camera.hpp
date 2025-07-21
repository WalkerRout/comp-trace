#ifndef CAMERA_H
#define CAMERA_H

#include "point3.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace rt {

class camera {
public:
  [[nodiscard]] constexpr camera() noexcept {
    auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    m_origin = {0.0, 0.0, 0.0};
    m_horizontal = {viewport_width, 0.0, 0.0};
    m_vertical = {0.0, viewport_height, 0.0};
    m_lower_left_corner =
        m_origin - m_horizontal / 2.0 - m_vertical / 2.0 - vec3{0.0, 0.0, focal_length};
  }

  [[nodiscard]] constexpr auto get_ray(const double u, const double v) const noexcept -> ray_d {
    return {m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin};
  }

private:
  point3_d m_origin;
  point3_d m_lower_left_corner;
  vec3<double> m_horizontal;
  vec3<double> m_vertical;
};

} // namespace rt

#endif // CAMERA_H