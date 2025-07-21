
#include "render.hpp"

auto main(int argc, char* argv[]) -> int {
  (void)argc;
  (void)argv;

  static constexpr render_params params{512, 400};
  static constexpr auto img = render<params.width, params.height>();

  save_ppm(img, "out.ppm");

  return 0;
}