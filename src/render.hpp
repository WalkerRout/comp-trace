#ifndef RENDER_HPP
#define RENDER_HPP

#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <print>
#include <ranges>

#include "camera.hpp"
#include "colour.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "util.hpp"

namespace rt {

template <std::size_t N> using sphere_scene = scene<sphere_d, N>;

[[nodiscard]] constexpr auto build_scene() noexcept -> sphere_scene<3> {
  sphere_scene<3> world{};
  world.add(sphere_d{{0.0, 0.0, -1.0}, 0.5});
  world.add(sphere_d{{1.3, 0.0, -0.9}, 0.2});
  world.add(sphere_d{{0.0, -100.5, -1.0}, 100.0});
  return world;
}

// guess what this function does...
template <colour_value_type_compatible C, pixel_value_type_compatible P>
[[nodiscard]] constexpr auto colour_to_pixel(const colour<C>& c) noexcept -> pixel<P> {
  auto to_channel = [](C value) constexpr noexcept -> P {
    if constexpr (std::floating_point<C>) {
      // clamp then scale
      const C clamped = std::clamp(value, C{0}, C{1});
      return static_cast<P>(clamped * std::numeric_limits<P>::max());
    } else if constexpr (std::integral<C> && std::integral<P>) {
      // cool, int to int...
      if constexpr (sizeof(C) > sizeof(P)) {
        // downscale
        return static_cast<P>(value >> (8 * (sizeof(C) - sizeof(P))));
      } else {
        // direct (or upscale?)
        return static_cast<P>(value);
      }
    } else {
      // todo other conversions
      return static_cast<P>(value);
    }
  };

  return {to_channel(c.r()), to_channel(c.g()), to_channel(c.b())};
}

template <scene_value_type_compatible Scene>
[[nodiscard]] constexpr auto ray_colour(const ray<float_type_of_t<Scene>>& r,
                                        const Scene& world) noexcept
    -> colour<float_type_of_t<Scene>> {
  using float_type = float_type_of_t<Scene>;

  const auto hit = world.hit(r, std::numeric_limits<float_type>::epsilon(),
                             std::numeric_limits<float_type>::infinity());
  if (hit) {
    // show normals...
    const auto& n = hit->normal;
    return colour<float_type>{float_type{0.5} * (n.x() + float_type{1}),
                              float_type{0.5} * (n.y() + float_type{1}),
                              float_type{0.5} * (n.z() + float_type{1})};
  }

  // background gradient
  const auto unit_dir = unit_vector(r.direction());
  const float_type t = float_type{0.5} * (unit_dir.y() + float_type{1});
  const colour<float_type> white{float_type{1}, float_type{1}, float_type{1}};
  const colour<float_type> blue{float_type{0.5}, float_type{0.7}, float_type{1}};
  return (float_type{1} - t) * white + t * blue;
}

template <std::size_t W, std::size_t H>
concept valid_render_dimensions =
    (W > 0) && (H > 0) && (W * H <= std::numeric_limits<std::size_t>::max() / 3);

template <std::size_t Width, std::size_t Height>
  requires valid_render_dimensions<Width, Height>
[[nodiscard]] consteval auto render() noexcept -> image<Width, Height> {
  const auto world = build_scene();
  const camera cam{};
  image<Width, Height> img{};

  for (const auto [row, col] : std::views::cartesian_product(
           std::views::iota(std::size_t{0}, Height), std::views::iota(std::size_t{0}, Width))) {
    const auto u = static_cast<double>(col) / static_cast<double>(Width - 1);
    const auto v = static_cast<double>(row) / static_cast<double>(Height - 1);
    const ray_d r = cam.get_ray(u, v);
    const colour_d pixel_colour = ray_colour(r, world);
    img.set_pixel(col, Height - row - 1, colour_to_pixel<double, std::uint8_t>(pixel_colour));
  }

  return img;
}

} // namespace rt

#endif // RENDER_HPP