#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "Connect4" ? 0 : 1;
}
