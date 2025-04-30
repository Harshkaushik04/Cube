#include "headers/model.hpp"
using namespace std;

CubeNetImpl::CubeNetImpl(){ //default constructor
    this->learningRate=0;
    this->dropoutRate=0;
    this->numActions=0;
    fc1=nullptr,fc2=nullptr,fc3=nullptr,fc4=nullptr,dropout=nullptr,optimizer=nullptr;
}

//  // Constructor
// CubeNetImpl::CubeNetImpl(float learningRate, float dropoutRate, int numActions)
//  : learningRate(learningRate), dropoutRate(dropoutRate), numActions(numActions) {
//  // Define the shared CNN branch for each face
//  face_branch = register_module("face_branch", torch::nn::Sequential(
//      torch::nn::Conv2d(torch::nn::Conv2dOptions(6, 64, 2).stride(1).padding(0)),
//      torch::nn::ReLU(),
//      torch::nn::Conv2d(torch::nn::Conv2dOptions(64, 128, 2).stride(1).padding(0)),
//      torch::nn::ReLU(),
//      torch::nn::Flatten()
//  ));
// fc1=torch::nn::Linear(6 * 128,512);
// fc2=torch::nn::Linear(512,256);
// fc3=torch::nn::Linear(256,numActions);
// dropout=torch::nn::Dropout(dropoutRate);
//  // Define fully connected layers
// register_module("fc1", fc1);  // Input size: 768, Output size: 512
// register_module("fc2", fc2);      // Input size: 512, Output size: 256
// register_module("fc3", fc3); // Input size: 256, Output size: numActions

// register_module("dropout", dropout);
// optimizer=make_shared<torch::optim::Adam>(this->parameters(), torch::optim::AdamOptions(this->learningRate));
// }

// // Forward pass
// torch::Tensor CubeNetImpl::forward(torch::Tensor inputBatch) {
//  // Ensure input is on the correct device (CUDA if available)
//  inputBatch = inputBatch.to(torch::kCUDA);

//  // Reshape input tensor to [batch_size * 6, 3, 3, 6]
//  inputBatch = inputBatch.view({-1, 3, 3, 6});

//  // Permute dimensions from [batch_size * 6, 3, 3, 6] to [batch_size * 6, 6, 3, 3]
//  inputBatch = inputBatch.permute({0, 3, 1, 2});

//  // Process each face independently
//  torch::Tensor x = face_branch->forward(inputBatch);  // Shape: [768, 128]

//  // Reshape to [batch_size, 6 * 128]
//  x = x.view({inputBatch.size(0) / 6, -1});  // Shape: [128, 768]

//  // Fully connected layers with dropout
//  x = torch::relu(fc1(x));  // Shape: [128, 512]
//  x = dropout(x);
//  x = torch::relu(fc2(x));  // Shape: [128, 256]
//  x = dropout(x);
//  x = fc3(x);  // Shape: [128, numActions]

//  return x;
// }



CubeNetImpl::CubeNetImpl(float learningRate,float dropoutRate,int numActions){
    this->learningRate=learningRate;
    this->dropoutRate=dropoutRate;
    this->numActions=numActions;
    fc1 = torch::nn::Linear(inputSize, hiddenLayer1);
        fc2 = torch::nn::Linear(hiddenLayer1, hiddenLayer2);
        fc3 = torch::nn::Linear(hiddenLayer2, hiddenLayer3);
        fc4 = torch::nn::Linear(hiddenLayer3, numActions);
        dropout = torch::nn::Dropout(dropoutRate);

        // Register layers
        register_module("fc1", fc1);
        register_module("fc2", fc2);
        register_module("fc3", fc3);
        register_module("fc4", fc4);
        register_module("dropout", dropout);

    optimizer=make_shared<torch::optim::Adam>(this->parameters(), torch::optim::AdamOptions(this->learningRate));
}


torch::Tensor CubeNetImpl::forward(torch::Tensor inputBatch){
    // cout<<"=================================================="<<endl;
    // cout<<"Input batch:"<<endl;
    // cout<<inputBatch<<endl;
    // cout<<"=================================================="<<endl;
    inputBatch.to(torch::kCUDA);
    inputBatch = inputBatch.view({inputBatch.size(0), -1});
    inputBatch=torch::relu(fc1(inputBatch));
    inputBatch=dropout(inputBatch);
    inputBatch=torch::relu(fc2(inputBatch));
    inputBatch=dropout(inputBatch);
    inputBatch=torch::relu(fc3(inputBatch));
    inputBatch=dropout(inputBatch);
    inputBatch=fc4(inputBatch);
    return inputBatch;
}



shared_ptr<CubeNetImpl> CubeNetImpl::clone() {
    return std::make_shared<CubeNetImpl>(*this);
}

// Save Model
void CubeNetImpl::saveModel(const std::string& path) {
    torch::serialize::OutputArchive archive;
    this->save(archive);  // Save the entire model
    archive.save_to(path);
    // std::cout << "Model saved to " << path << std::endl;
}

// Load Model
void CubeNetImpl::loadModel(const std::string& path) {
    torch::serialize::InputArchive archive;
    archive.load_from(path);
    this->load(archive);  // Load the entire model
    std::cout << "Model loaded from " << path << std::endl;
}
