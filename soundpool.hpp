#include "raylib.h"
#include <fstream>
#include <iostream>
#include <vector>

struct SimRender {
  const int screenWidth = 300;
  const int screenHeight = 100;

  void plot_point(float u, float time) { DrawRectangle(2, 2, 2, 2, BLACK); }
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

  for (const float &n : numbers) {
    std::cout << n << " ";
  }
  std::cout << std::endl;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    //DRAW
    EndDrawing();

  }

  CloseWindow();
  return 0;
}
