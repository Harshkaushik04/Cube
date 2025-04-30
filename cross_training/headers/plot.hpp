#pragma once
#include<vector>
#include<string>

class LivePlot {
    public:
        std::vector<double> avgReturns;
        std::vector<double> epsilonCheckpoint;
        int epochs;
        LivePlot() : epochs(0) {}
        void update_plot(const std::vector<double>& returns, const std::vector<double>& epsilon, const std::string& initial_bias);
};