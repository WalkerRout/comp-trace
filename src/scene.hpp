#ifndef SCENE_HPP
#define SCENE_HPP

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <span>

#include "ray.hpp"
#include "sphere.hpp"

// for complex objects to be placed inside of a scene...
template <typename T>
concept scene_value_type_compatible =
    requires {
      // we need to be able to extract a float type...
      typename float_type_of_t<T>;
      requires std::floating_point<float_type_of_t<T>>;
    } &&
    requires(T obj, ray<float_type_of_t<T>> r, float_type_of_t<T> t_min, float_type_of_t<T> t_max) {
      { obj.hit(r, t_min, t_max) } -> std::same_as<std::optional<hit_record<float_type_of_t<T>>>>;
    };

template <scene_value_type_compatible T, std::size_t N> class scene;

// scene extracts nested float type
template <scene_value_type_compatible T, std::size_t N> struct float_type_of<scene<T, N>> {
  using type = float_type_of_t<T>; // recurse
};

template <scene_value_type_compatible T, std::size_t N> class scene {
public:
  using value_type = T;
  using size_type = std::size_t;
  using float_type = float_type_of_t<T>;

  [[nodiscard]] constexpr scene() noexcept = default;

  constexpr void add(const value_type& object) noexcept {
    assert(m_count < N && "scene capacity exceeded");
    m_objects[m_count] = object;
    m_count += 1;
  }

  [[nodiscard]] constexpr auto hit(const ray<float_type>& r, const float_type t_min,
                                   const float_type t_max) const noexcept
      -> std::optional<hit_record<float_type>> {
    std::optional<hit_record<float_type>> closest_hit;
    float_type closest_so_far = t_max;

    for (size_type i = 0; i < m_count; ++i) {
      if (const auto hit_opt = m_objects[i].hit(r, t_min, closest_so_far)) {
        closest_hit = hit_opt;
        closest_so_far = hit_opt->t;
      }
    }

    return closest_hit;
  }

private:
  std::array<value_type, N> m_objects{};
  std::size_t m_count{};
};

#endif // SCENE_HPP