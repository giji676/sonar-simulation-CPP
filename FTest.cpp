#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::vector<std::vector<std::string>>> F(7);

    // Populate the 3D array with string values
    for (int i = 0; i < 7; ++i) {
        std::vector<std::vector<std::string>> js(8);
        for (int j = 0; j < 8; ++j) {
            std::vector<std::string> ks(9);
            for (int k = 0; k < 9; ++k) {
                ks[k] = std::to_string(i) + std::to_string(j) + std::to_string(k);
            }
            js[j] = ks;
        }
        F[i] = js;
    }

    // Print the 3D array (optional, for testing)
    std::cout << "String array:\n";
    //for (int i = 0; i < F.size(); ++i) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < F[i].size(); ++j) {
            for (int k = 0; k < F[i][j].size(); ++k) {
                std::cout << F[i][j][k] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}
