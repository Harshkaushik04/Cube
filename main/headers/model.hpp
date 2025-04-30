#pragma once
#include<iostream>
#include<torch/torch.h>

class CubeNetImpl: public torch::nn::Module {
public:
    // std::shared_ptr<torch::nn::Linear> fc1,fc2,fc3,fc4;
    // std::shared_ptr<torch::nn::Dropout> dropout;
    // std::shared_ptr<torch::optim::Adam> optimizer;
    torch::nn::Linear fc1{nullptr},fc2{nullptr},fc3{nullptr},fc4{nullptr};
    torch::nn::Dropout dropout{nullptr};
    std::shared_ptr<torch::optim::Adam> optimizer;
    float learningRate;
    int inputSize=324;
    int hiddenLayer1=512;
    int hiddenLayer2=256;
    int hiddenLayer3=128;
    int numActions;
    int dropoutRate;
    std::shared_ptr<CubeNetImpl> model;
    CubeNetImpl();
    CubeNetImpl(float,float,int); //learning rate,dropout,numActions
    torch::Tensor forward(torch::Tensor);
    void saveModel(const std::string&);
    void loadModel(const std::string&);
    std::shared_ptr<CubeNetImpl>  clone();
    // void serialize(torch::serialize::OutputArchive&);
    // void serialize(torch::serialize::InputArchive& );
};

TORCH_MODULE(CubeNet);
