#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "plot.hpp"

void LivePlot::update_plot(const std::vector<double>& returns, const std::vector<double>& epsilon, const std::string& initial_bias){
    avgReturns = returns;
        epsilonCheckpoint = epsilon;
        epochs = avgReturns.size();

        // Save data to CSV file
        std::ofstream file("data.csv");
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing!" << std::endl;
            return;
        }

        for (size_t i = 0; i < avgReturns.size(); ++i) {
            file << avgReturns[i] << "," << epsilonCheckpoint[i] << "\n";
        }
        file.close();

        // Construct the filename
        std::string output_file = "plots/plot_" + initial_bias + ".png";

        // Call the Python script to generate the plot
        std::string command = "python3 plot.py data.csv " + output_file;
        int result = std::system(command.c_str());
        if (result != 0) {
            std::cerr << "Error executing Python script!" << std::endl;
        }
}