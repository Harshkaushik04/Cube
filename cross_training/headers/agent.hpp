#pragma once
#include<torch/torch.h>
#include "model.hpp"
#include "env.hpp"
#include<tuple>
#include<deque>

class ReplayBuffer{
public:
    long capacity;
    torch::DeviceType device;
    torch::DeviceType largerMemoryDevice;
    std::deque<std::tuple<torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor>> memory;
    ReplayBuffer(long,torch::DeviceType,torch::DeviceType);
    void insert(std::tuple<torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor>);
    bool canSample(int);
    std::vector<torch::Tensor> sample(int);
};

class Agent{
public:
    std::shared_ptr<CubeNetImpl> model;
    std::shared_ptr<CubeNetImpl> targetModel;
    std::shared_ptr<ReplayBuffer> memory;
    float epsilon;
    float minEpsilon;
    float epsilonDecay;
    torch::DeviceType device;
    torch::DeviceType largerMemoryDevice;
    int batchSize;
    float gamma;
    int numActions;
    float learningRate;
    std::shared_ptr<torch::optim::Adam> optimizer;
    Agent(CubeNetImpl ,torch::DeviceType,torch::DeviceType,float,float,int,int,long,int,float,float);
    torch::Tensor getAction(torch::Tensor);
    void train(Env,long,long);
    void test(Env);
};