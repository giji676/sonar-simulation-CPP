#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

const float PI_F = 3.14159265358979f;

const float REFL_COEF = 0.7; // Reflection coeficient
// Gamma - used for absorbing walls
const float G = (1 - REFL_COEF) / (1 + REFL_COEF);
const float LF = 0.5 * sqrt(0.5) * G; // Loss factor

const float C = 343;   // Speed of sound constant
const float DX = 2.9;  // dt <= dx/C | dx >= C*dt ~~ 2.86
const int WIDTH = 100; // Model assumes 100x100 represents 1m x 1m area irl
const int HEIGHT = 100;
const int PIXELS_PER_CELL = 6;
const int FPS = 120;

struct Cell {
  float u{0.0};
  bool wall{false};
  float refl_coef{0.9};
};

std::vector<std::vector<Cell>> grid(HEIGHT, std::vector<Cell>(WIDTH));
std::vector<std::vector<Cell>> prev_grid = grid;
std::vector<std::vector<Cell>> next_grid = grid;

float get_LF(Cell cell) {
  float g = (1 - cell.refl_coef) / (1 + cell.refl_coef);
  float lf = 0.5 * sqrt(0.5) * g;

  return lf;
}

float calculate_pressure(int x, int y, int k, float dt) {
  float cfl_factor = pow((C * dt / DX), 2);

  float val =
      (1 / (1 + LF * (4 - k))) * ((2 - 0.5 * k) * grid[y][x].u +
                                  0.5 * (grid[y][x + 1].u + grid[y][x - 1].u +
                                         grid[y + 1][x].u + grid[y - 1][x].u) +
                                  (LF * (4 - k) - 1) * prev_grid[y][x].u);

  val *= cfl_factor;

  return val;
}

void find_min_max(float &min_val, float &max_val) {
  min_val = std::numeric_limits<float>::max();
  max_val = std::numeric_limits<float>::min();

  for (const auto &row : grid) {
    for (const auto &cell : row) {
      if (cell.u < min_val) {
        min_val = cell.u;
      }
      if (cell.u > max_val) {
        max_val = cell.u;
      }
    }
  }
}

int cos_interp(int a, int b, float t) {
  float ft = t * PI_F;
  float f = (1 - std::cos(ft)) / 2;
  return static_cast<int>(a + f * (b - a));
}

void get_colour(Cell cell, float max_val, int (&colour)[3]) {
  if (cell.wall) {
    colour[0] = 255; // Red
    colour[1] = 0;   // Green
    colour[2] = 0;   // Blue
  } else {
    float cval = cell.u;
    cval = (cval - (-max_val)) / (max_val - (-max_val));
    cval = std::clamp(cval, 0.0f, 1.0f);
    int r = cos_interp(0, 255, cval); // R goes from 0 to 255
    int g = cos_interp(0, 255, cval); // G goes from 0 to 255
    int b = cos_interp(255, 0, cval); // B goes from 255 to 0

    colour[0] = r;
    colour[1] = g;
    colour[2] = b;
  }
}

int main() {
  for (int y = 1; y < HEIGHT - 1; y++) {
    for (int x = 1; x < WIDTH - 1; x++) {
      int k = 4;
      if (y == 1 || y == HEIGHT - 2) {
        k -= 1;
      }
      if (x == 1 || x == WIDTH - 2) {
        k -= 1;
      }

      std::vector<std::pair<int, int>> neighbours_list = {
          {x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1}};
      for (const std::pair<int, int> neighbour_cell : neighbours_list) {
        if (grid[neighbour_cell.second][neighbour_cell.first].wall) {
          k -= 1;
        }
      }
      if (!grid[y][x].wall) {
        float val = calculate_pressure(x, y, k, 1.0 / FPS);
        next_grid[y][x].u = val;
      }
    }
  }
  float min_val;
  float max_val;
  find_min_max(min_val, max_val);

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      int colour[3];
      get_colour(grid[y][x], max_val, colour);
    }
  }
}
