#include "raylib.h"
#include <fstream>
#include <iostream>
#include <vector>

const float AMPLITUDE_MULT = 20; // 2*20 = 40, read ampl max ~~ 2
const int WIDTH = 600;
const int HEIGHT = 100;

struct SimRender {
  const int screenWidth = WIDTH;
  const int screenHeight = HEIGHT;

  void plot_point(float u, float time) {
    DrawRectangle(WIDTH-time*2, HEIGHT/2-u*AMPLITUDE_MULT, 4, 4, BLACK);
  }
};

int main() {
  SimRender sim_render;

  InitWindow(sim_render.screenWidth, sim_render.screenHeight, "Pressure plot"),
  SetTargetFPS(60);

  std::vector<float> numbers;
  float num;

  std::ifstream inFile("output.txt");

  if (inFile.is_open()) {
    while (inFile >> num) {
      numbers.push_back(num); // Read floats from the file and store in vector
    }
    inFile.close(); // Close the file
  } else {
    std::cerr << "Unable to open file for reading.\n";
  }
  int index = 0;

  ClearBackground(WHITE);
  while (!WindowShouldClose()) {
    BeginDrawing();
    if (index < numbers.size()){
      sim_render.plot_point(numbers[index], index);
      index++;
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
