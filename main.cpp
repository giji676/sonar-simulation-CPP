#include "raylib.h"
#include <cmath>
#include <vector>

const float REFL_COEF = 0.7;  // Reflection coeficient
// Gamma - used for absorbing walls
const float G = (1 - REFL_COEF) / (1 + REFL_COEF);
const float LF = 0.5 * sqrt(0.5) * G;  // Loss factor

const float C = 343;   // Speed of sound constant
const float DX = 2.9;  // dt <= dx/C | dx >= C*dt ~~ 2.86
const int WIDTH = 100;  // Model assumes 100x100 represents 1m x 1m area irl
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
