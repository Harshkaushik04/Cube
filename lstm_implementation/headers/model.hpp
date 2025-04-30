#pragma once
#include <iostream>
#include <torch/torch.h>

class CubeNetImpl : public torch::nn::Module {
public:
    torch::nn::LSTM lstm{nullptr};
    torch::nn::Linear fc{nullptr};
    std::shared_ptr<torch::optim::Adam> optimizer;
    float learningRate;
    int inputSize = 324;
    int hiddenSize = 256;
    int numLayers = 2;
    int numActions;

    CubeNetImpl();
    CubeNetImpl(float, int, int);
    torch::Tensor forward(torch::Tensor);
    void saveModel(const std::string&);
    void loadModel(const std::string&);
    std::shared_ptr<CubeNetImpl> clone();
};

// TORCH_MODULE macro to define a module holder
TORCH_MODULE(CubeNet);
