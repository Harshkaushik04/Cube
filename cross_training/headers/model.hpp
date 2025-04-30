#pragma once
#include <iostream>
#include <torch/torch.h>

class CubeNetImpl : public torch::nn::Module {
public:
    torch::nn::Sequential face_branch{nullptr};
    torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};
    torch::nn::Dropout dropout{nullptr};
    std::shared_ptr<torch::optim::Adam> optimizer;
    float learningRate;
    float dropoutRate;
    int numActions;
    CubeNetImpl();
    CubeNetImpl(float,float,int);
    torch::Tensor forward(torch::Tensor);
    void saveModel(const std::string&);
    void loadModel(const std::string&);
    std::shared_ptr<CubeNetImpl> clone();
};

// TORCH_MODULE macro to define a module holder
TORCH_MODULE(CubeNet);