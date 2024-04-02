#include "mt_rng.hpp"
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>


int main() {
    init_mt(time(NULL));
    std::vector<uint32_t> uniform_data;
    std::vector<uint32_t> normal_data;

    for (int i = 0; i < 20000; ++i) {
        uint32_t a = get_next(1000, 1500);
        uniform_data.push_back(a);
    }

    for (int i = 0; i < 20000; ++i) {
        uint32_t a = get_normal(10000.0, 150.0);
        normal_data.push_back(a);
    }

    std::ofstream uniform_outfile("dist_random_numbers.txt");
    if (!uniform_outfile.is_open()) {
        std::cerr << "Error opening uniform output file for writing." << std::endl;
        return 1;
    }
    for (const auto& num : uniform_data) {
        uniform_outfile << num << std::endl;
    }
    uniform_outfile.close();
    std::cout << "Uniform random numbers have been written to 'dist_random_numbers.txt'." << std::endl;

    std::ofstream normal_outfile("norm_dist_random_numbers.txt");
    if (!normal_outfile.is_open()) {
        std::cerr << "Error opening normal output file for writing." << std::endl;
        return 1;
    }
    for (const auto& num : normal_data) {
        normal_outfile << num << std::endl;
    }
    normal_outfile.close();
    std::cout << "Normal random numbers have been written to 'norm_dist_random_numbers.txt'." << std::endl;

    return 0;
}
