#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

double distance(double x1, double y1, double x2, double y2) {
  return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

std::vector<std::vector<int>> zip(const std::vector<int> &arr1,
                                  const std::vector<int> &arr2) {
  int min_size = std::min(arr1.size(), arr2.size());

  std::vector<std::vector<int>> zipped_arr(min_size, std::vector<int>(3));

  for (int i = 0; i < min_size; i++) {
    zipped_arr[i][0] = i;
    zipped_arr[i][1] = arr1[i];
    zipped_arr[i][2] = arr2[i];
  }

  return zipped_arr;
}

std::vector<std::vector<std::vector<std::string>>> createStringArray() {
  std::vector<std::vector<std::vector<std::string>>> arr(7);

  for (int i = 0; i < 7; ++i) {
    std::vector<std::vector<std::string>> js(8);
    for (int j = 0; j < 8; ++j) {
      std::vector<std::string> ks(9);
      for (int k = 0; k < 9; ++k) {
        ks[k] = std::to_string(i) + std::to_string(j) +
                std::to_string(k); // Testing with strings
      }
      js[j] = ks;
    }
    arr[i] = js;
  }
  return arr;
}

void rollVector(std::vector<std::string> &vec, int shift) {
  if (shift == 0)
    return; // No need to shift if shift == 0

  int n = vec.size();
  shift = (shift % n + n) %
          n; // Normalize the shift to handle negative and large shifts
  std::rotate(vec.begin(), vec.begin() + (n - shift),
              vec.end()); // Use std::rotate for circular shift
}

// Function to roll each row of a 2D matrix along an axis (axis=1, i.e., along
// rows)
void rollAlongAxis(std::vector<std::vector<std::string>> &matrix, int shift) {
  for (auto &row : matrix) {
    rollVector(row, shift); // Shift each row
  }
}

int main() {
  int Ny = 100;
  int Nx = 100;
  double tau = 0.53;
  int Nt = 3000;
  int NL = 9;

  // double F[Ny][Nx][NL];
  auto F = createStringArray();
  bool cylinder[Ny][Nx];

  std::vector<int> cxs = {0, 0, 1, 1, 1, 0, -1, -1, -1};
  std::vector<int> cys = {0, 1, 1, 0, -1, -1, -1, 0, 1};

  std::default_random_engine generator;
  std::normal_distribution<double> distribution(0.0, 1.0);

  for (int i = 0; i < Ny; i++) {
    for (int j = 0; j < Nx; j++) {
      for (int k = 0; k < NL; k++) {
        //		F[i][j][k] = 1.0 + 0.1 * distribution(generator);
      }
      cylinder[i][j] = false;
    }
  }

  /*
  for (int i = 0; i < Ny; i++) {
          for (int j = 0; j < Nx; j++) {
                  F[i][j][3] = 2.3;
          }
  }
  */

  for (int i = 0; i < Ny; i++) {
    for (int j = 0; j << Nx; j++) {
      if (distance(Nx / 4, Ny / 2, j, i) < 13) {
        // cylinder[i][j] = true;
      }
    }
  }
  /*
  for (int i = 0; i < Ny; i++) {
          for (int j = 0; j < Nx; j++) {
                  for (int k = 0; k < NL; k++) {
                          std::cout << F[i][j][k] << " ";
                  }
                  std::cout << std::endl;
          }
          std::cout << std::endl;
  }
  */

  std::vector<std::vector<int>> zipped = zip(cxs, cys);

  for (const auto &entry : zipped) {
    int i = entry[0];  // Get the index of the 2D slice
    int cx = entry[1]; // Get the shift value
    int cy = entry[2]; // Get the shift value
    std::cout << i << " " << cx << " " << cy << std::endl;
    rollAlongAxis(F[i], cx); // Roll the 2D slice of F along the axis with the
                             // corresponding shift
  }

  std::cout << "Resulting array after rolling:\n";
  for (int i = 0; i < F.size(); i++) {
    for (int j = 0; j < F[i].size(); j++) {
      for (int k = 0; k < F[i][j].size(); k++) {
        std::cout << F[i][j][k] << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }

  return 0;
}
