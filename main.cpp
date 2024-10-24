#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
// 0.0010025

const float PI_F = 3.14159265358979f;

const float REFL_COEF = 0.7; // Reflection coeficient
// Gamma - used for absorbing walls
const float G = (1 - REFL_COEF) / (1 + REFL_COEF);
const float LF = 0.5 * sqrt(0.5) * G; // Loss factor
const float C = 343; // Speed of sound constant
const float PULSE_FREQ = 40000.0f; // Frequency in Hz (for example, 440Hz = A4 note)
const int SIM_PER_FREQ = 10; // number of simulations that will get run per frequency
const float AMPLITUDE = 2.0f;    // Amplitude of the pulse
const int FPS = 60; // Actual FPS
const int SIM_RATE = PULSE_FREQ * 10; // Used to get the simulation time step (dt) 
const float DT = 1.0 / SIM_RATE;
// const float DX = C * DT * 1.01; // calculate the minimum and add 1% just in case
const float DX = (C * DT)/(sqrt(0.5)); // calculate the minimum and add 1% just in case
                                // // dt <= dx/C | dx >= C*dt ~~ 2.86
// Currently wave length ~~ 8.575mm
// 1 pixel == wave_length/4
const float WAVE_LENGTH = C/PULSE_FREQ;
const float PIXEL_SCALE = PULSE_FREQ/4;
const int WIDTH = 200; // Model assumes 100x100 represents 1m x 1m area irl
const int HEIGHT = 200;
const int PIXELS_PER_CELL = 3;
const bool VISUAL_WALL = true;

std::vector<float> pressures;

struct Cell {
  float u;
  bool wall;

  Cell() : u(0.0), wall(false) {}
};

std::vector<std::vector<Cell>> grid(HEIGHT, std::vector<Cell>(WIDTH));
std::vector<std::vector<Cell>> prev_grid;
std::vector<std::vector<Cell>> next_grid;

/*
std::vector<std::pair<int, int>> wall_line_list = {
  {static_cast<int>(WIDTH / 2 - 7), static_cast<int>(HEIGHT - 20)},
  {static_cast<int>(WIDTH / 2 - 5), static_cast<int>(HEIGHT - 10)},
  {static_cast<int>(WIDTH / 2 - 5), static_cast<int>(HEIGHT + 0)},
  {static_cast<int>(WIDTH / 2 + 5), static_cast<int>(HEIGHT + 0)},
  {static_cast<int>(WIDTH / 2 + 5), static_cast<int>(HEIGHT - 10)},
  {static_cast<int>(WIDTH / 2 + 7), static_cast<int>(HEIGHT - 20)},
};
*/

float deg2rad(float angle){
  return angle * (PI_F / 180.0);
}

std::vector<std::pair<int, int>> wall_line_list = {
  {static_cast<int>(WIDTH / 2), static_cast<int>(HEIGHT - 0)},
  {static_cast<int>(WIDTH / 2 + WIDTH*std::cos(deg2rad(90-30))), static_cast<int>(HEIGHT - WIDTH*std::sin(deg2rad(90-30)))},
  {static_cast<int>(WIDTH / 2), static_cast<int>(HEIGHT - 0)},
  {static_cast<int>(WIDTH / 2 + WIDTH*std::cos(deg2rad(90-45))), static_cast<int>(HEIGHT - WIDTH*std::sin(deg2rad(90-45)))},
  {static_cast<int>(WIDTH / 2), static_cast<int>(HEIGHT - 0)},
  {static_cast<int>(WIDTH / 2 + WIDTH*std::cos(deg2rad(90-60))), static_cast<int>(HEIGHT - WIDTH*std::sin(deg2rad(90-60)))},
  {static_cast<int>(WIDTH / 2), static_cast<int>(HEIGHT - 0)},
  {static_cast<int>(WIDTH / 2 + WIDTH*std::cos(deg2rad(90))), static_cast<int>(HEIGHT - WIDTH*std::sin(deg2rad(90)))},
  {static_cast<int>(WIDTH / 2), static_cast<int>(HEIGHT - 0)},
  {static_cast<int>(WIDTH / 2 + WIDTH*std::cos(deg2rad(90+60))), static_cast<int>(HEIGHT - WIDTH*std::sin(deg2rad(90+60)))},
  {static_cast<int>(WIDTH / 2), static_cast<int>(HEIGHT - 0)},
  {static_cast<int>(WIDTH / 2 + WIDTH*std::cos(deg2rad(90+45))), static_cast<int>(HEIGHT - WIDTH*std::sin(deg2rad(90+45)))},
  {static_cast<int>(WIDTH / 2), static_cast<int>(HEIGHT - 0)},
  {static_cast<int>(WIDTH / 2 + WIDTH*std::cos(deg2rad(90+30))), static_cast<int>(HEIGHT - WIDTH*std::sin(deg2rad(90+30)))},
};

void apply_pulse(float time, float angle) {
  angle = 45;
  int antena_spacing = 2; // half a wavelength in pixels
  //
  float radian = deg2rad(angle);
  //float phase_shift = (PI_F * std::sin(radian))/2;
  float phase_shift = (2 * PI_F * (WAVE_LENGTH/2) * std::sin(radian))/WAVE_LENGTH/2;
  if (angle > 90) {
    phase_shift *= -1;
  }

  // Apply cumulative phase shifts to pulses
  float pulse1 = AMPLITUDE * std::sin(2 * PI_F * PULSE_FREQ * time + phase_shift * 0);
  float pulse2 = AMPLITUDE * std::sin(2 * PI_F * PULSE_FREQ * time + phase_shift * 1);
  float pulse3 = AMPLITUDE * std::sin(2 * PI_F * PULSE_FREQ * time + phase_shift * 2);
  float pulse4 = AMPLITUDE * std::sin(2 * PI_F * PULSE_FREQ * time + phase_shift * 3);
  float pulse5 = AMPLITUDE * std::sin(2 * PI_F * PULSE_FREQ * time + phase_shift * 4);
  float pulse6 = AMPLITUDE * std::sin(2 * PI_F * PULSE_FREQ * time + phase_shift * 5);
  float pulse7 = AMPLITUDE * std::sin(2 * PI_F * PULSE_FREQ * time + phase_shift * 6);

  // Assign pulses to grid positions
  grid[static_cast<int>(HEIGHT - 2)][static_cast<int>(WIDTH / 2 - antena_spacing * 3)].u = pulse1;
  grid[static_cast<int>(HEIGHT - 2)][static_cast<int>(WIDTH / 2 - antena_spacing * 2)].u = pulse2;
  grid[static_cast<int>(HEIGHT - 2)][static_cast<int>(WIDTH / 2 - antena_spacing * 1)].u = pulse3;
  grid[static_cast<int>(HEIGHT - 2)][static_cast<int>(WIDTH / 2 + antena_spacing * 0)].u = pulse4;
  grid[static_cast<int>(HEIGHT - 2)][static_cast<int>(WIDTH / 2 + antena_spacing * 1)].u = pulse5;
  grid[static_cast<int>(HEIGHT - 2)][static_cast<int>(WIDTH / 2 + antena_spacing * 2)].u = pulse6;
  grid[static_cast<int>(HEIGHT - 2)][static_cast<int>(WIDTH / 2 + antena_spacing * 3)].u = pulse7;
}

float read_pressure(int x, int y) {
  float pressure = grid[y][x].u;
  return pressure;
}

void read_lobes(std::vector<float> &lobes_pressure) {
  int r = 50;
  for (int i = 0; i < 180; ++i) {
    int x = WIDTH/2 - static_cast<int>(r*std::cos(deg2rad(i)));
    int y = HEIGHT-2 - static_cast<int>(r*std::sin(deg2rad(i)));

    float val = read_pressure(x, y);
    lobes_pressure[i] = val;
  }
}

struct SimRender {
  const int screenWidth = WIDTH * PIXELS_PER_CELL;
  const int screenHeight = HEIGHT * PIXELS_PER_CELL;

  void draw_cell(int x, int y, Color color) {
    DrawRectangle(x * PIXELS_PER_CELL, y * PIXELS_PER_CELL, PIXELS_PER_CELL,
                  PIXELS_PER_CELL, color);
  }
};

float calculate_pressure(int x, int y, int k, float dt) {
  float val =
    (1 / (1 + LF * (4 - k))) * ((2 - 0.5 * k) * grid[y][x].u +
    0.5 * (grid[y][x + 1].u + grid[y][x - 1].u +
    grid[y + 1][x].u + grid[y - 1][x].u) +
    (LF * (4 - k) - 1) * prev_grid[y][x].u);

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

Color get_color(Cell cell, float max_val) {
  Color color;
  if (cell.wall) {
    color.r = 255; // Red
    color.g = 255; // Green
    color.b = 255; // Blue
    color.a = 255; // Alpha (fully opaque)
  } else {
    float cval = cell.u;
    cval = (cval - (-max_val)) / (max_val - (-max_val));
    cval = std::clamp(cval, 0.0f, 1.0f);

    color.r = static_cast<unsigned char>(
        cos_interp(0, 255, cval)); // R goes from 0 to 255
    color.g = static_cast<unsigned char>(
        cos_interp(0, 255, cval)); // G goes from 0 to 255
    color.b = static_cast<unsigned char>(
        cos_interp(255, 0, cval)); // B goes from 255 to 0
    color.a = 255;                 // Alpha (fully opaque)
  }
  return color;
}

std::vector<std::pair<int, int>> bresenham_line(int x0, int y0, int x1,
                                                int y1) {
  std::vector<std::pair<int, int>> points;
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (true) {
    points.push_back(
        std::make_pair(x0, y0)); // Add the current point to the list
    if (x0 == x1 && y0 == y1) {  // If we've reached the end point, stop
      break;
    }
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }

  return points;
}

void set_wall_cells(std::vector<std::vector<Cell>> &grid,
                    const std::vector<std::pair<int, int>> &wall_line_list) {
  for (size_t i = 0; i < wall_line_list.size() - 1; ++i) {
    int x0 = wall_line_list[i].first;
    int y0 = wall_line_list[i].second;
    int x1 = wall_line_list[i + 1].first;
    int y1 = wall_line_list[i + 1].second;

    // Generate all points between these two coordinates
    std::vector<std::pair<int, int>> line_points =
        bresenham_line(x0, y0, x1, y1);

    // Set the corresponding cells in the grid as walls
    for (const auto &point : line_points) {
      int x = point.first;
      int y = point.second;

      // Check if coordinates are within grid bounds
      if (x >= 0 && x < grid[0].size() && y >= 0 && y < grid.size()) {
        grid[y][x].wall = true;
      }
    }
  }
}

int main() {
  SimRender sim_render;

  InitWindow(sim_render.screenWidth, sim_render.screenHeight, "Sonar simulation");
  SetTargetFPS(FPS);


  set_wall_cells(grid, wall_line_list); // Only visual no effect on wave rn
  prev_grid = grid;
  next_grid = grid;

  //assign_initial_state();
  float time = 0.0;

  while (!WindowShouldClose()) {
    time += DT;
    apply_pulse(time, PI_F/3);

    for (int y = 1; y < HEIGHT - 1; ++y) {
      for (int x = 1; x < WIDTH - 1; ++x) {
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
            if (!VISUAL_WALL) {
              k -= 1;
            }
          }
        }
        if (!grid[y][x].wall || VISUAL_WALL) {
          float val = calculate_pressure(x, y, k, DT);
          next_grid[y][x].u = val;
        }
      }
    }

    float min_val;
    float max_val;
    find_min_max(min_val, max_val);
    max_val = AMPLITUDE; // Temp override for testing

    std::vector<float> lobes_pressure(180, 0.0);
    read_lobes(lobes_pressure);

    BeginDrawing();
    ClearBackground(BLACK);

    for (int y = 0; y < HEIGHT; ++y) {
      for (int x = 0; x < WIDTH; ++x) {
        Color color = get_color(grid[y][x], max_val);
        if (grid[y][x].wall) {
          color = WHITE;
        }
        sim_render.draw_cell(x, y, color);
      }
    }
    for (int x = 0; x < lobes_pressure.size(); ++x) {
      sim_render.draw_cell(x, HEIGHT-std::abs((lobes_pressure[x]*120)), WHITE);
    }

    pressures.push_back(read_pressure(50, 1));
    EndDrawing();
    prev_grid = grid;
    grid = next_grid;
  }
  std::ofstream outFile("output.txt");

  // Check if the file is open
  if (outFile.is_open()) {
    for (const float& num : pressures) {
      outFile << num << " ";  // Write each float followed by a space
    }
    outFile.close();  // Close the file
  } else {
    std::cerr << "Unable to open file for writing.\n";
  }
  CloseWindow();
  return 0;
}
