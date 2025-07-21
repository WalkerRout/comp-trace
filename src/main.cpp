
#include "render.hpp"

template <std::size_t W, std::size_t H> struct render_params {
  static constexpr std::size_t width = W;
  static constexpr std::size_t height = H;
};

auto main(int argc, char* argv[]) -> int {
  (void)argc;
  (void)argv;

  static constexpr render_params<128, 96> params{};
  static constexpr auto img = render<params.width, params.height>();

  // dump the bytes that make up the image...
  dump_bytes(img);
  
  // then save the actual image...
  save_ppm(img, "out.ppm");

  return 0;
}