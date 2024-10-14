
/*
    waterpool.hpp  -  Don Cross  -  2023-06-26

    Simulation of waves moving on the surface of water.
    Based on ideas from the video
    "How to write a Height-Field Water Simulator with 100 lines of code"
    by Matthias Müller / Ten Minute Physics:
    https://www.youtube.com/watch?v=hswBi5wcqAA
*/
#ifndef __COSINEKITTY_WATERPOOL_HPP
#define __COSINEKITTY_WATERPOOL_HPP

#include <cmath>
#include <iostream>
#include <vector>

namespace Sapphire {
struct WaterCell {
  float wet{1.0f};
  float pos{0.0f};
  float vel{0.0f};
  float acc{0.0f};
};

template <int WIDTH, int HEIGHT> class WaterPool {
private:
  static_assert(WIDTH > 0, "Width must be a positive integer.");
  static_assert(HEIGHT > 0, "Height must be a positive integer.");
  static const int SIZE = WIDTH * HEIGHT;

  std::vector<WaterCell> cell{SIZE};
  /*
  static constexpr int index(int i, int j) {
    // Use border-wraparound logic.
    // Tolerate -1, but not arbitrarily negative integers.
    return ((i + WIDTH) % WIDTH) + WIDTH * ((j + HEIGHT) % HEIGHT);
  }
  */
  static constexpr int index(int i, int j) {
    // get the index from 1D array, using row and collumn
    // without border-wraparound
    return (j * WIDTH) + i;
  }

  float acceleration(const WaterCell &h, int i, int j) const {
    if ((i > WIDTH-1) || (i < 0) || (j > HEIGHT-1) || (j < 0)) {
      return 0.0f;
    }
    const WaterCell &o = cell[index(i, j)];
    return o.wet * (o.pos - h.pos);
  }

public:
  const WaterCell &getCell(int i, int j) const { return cell.at(index(i, j)); }

  WaterCell &getCell(int i, int j) { return cell.at(index(i, j)); }

  void update(float dt, float halflife, float k) {
    const float damp = pow(0.5, dt / halflife);

    // Calculate acceleration of each water cell.
    for (int i = 0; i < WIDTH; ++i) {
      for (int j = 0; j < HEIGHT; ++j) {
        WaterCell &h = cell[index(i, j)];
        if (h.wet > 0.0f) {
          h.acc = k * (acceleration(h, i, j + 1) + acceleration(h, i, j - 1) +
                       acceleration(h, i - 1, j) + acceleration(h, i + 1, j));
        }
      }
    }

    // Use accelerations to update position and velocity of each water cell.
    for (int i = 0; i < WIDTH; ++i) {
      for (int j = 0; j < HEIGHT; ++j) {
        WaterCell &h = cell[index(i, j)];
        if (h.wet > 0.0f) {
          h.vel = (damp * h.vel) + (dt * h.acc);
          h.pos += (dt * h.vel);
        }
      }
    }
  }
};
} // namespace Sapphire

#endif // __COSINEKITTY_WATERPOOL_HPP
