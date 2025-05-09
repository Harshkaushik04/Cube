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
    float epsilonStage2;
    float minEpsilon;
    float epsilonDecay;
    torch::DeviceType device;
    torch::DeviceType largerMemoryDevice;
    int batchSize;
    float gamma;
    int numActions;
    float learningRate;
    std::shared_ptr<torch::optim::Adam> optimizer;
    Env env;
    Agent(CubeNetImpl ,torch::DeviceType,torch::DeviceType,float,float,int,int,long,int,float,float,Env);
    torch::Tensor getAction(torch::Tensor,long);
    void train(Env,long,long);
    void train2(Env,long,long);
    void test(Env);
    void testing(Env,long,long);
};