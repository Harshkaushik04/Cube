#include "headers/model.hpp"
using namespace std;

CubeNetImpl::CubeNetImpl(){ //default constructor
    this->learningRate=0;
    this->dropoutRate=0;
    this->numActions=0;
    fc1=nullptr,fc2=nullptr,fc3=nullptr,fc4=nullptr,dropout=nullptr,optimizer=nullptr;
}

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
    inputBatch.to(torch::kCUDA);
    inputBatch = inputBatch.view({inputBatch.size(0), -1});
    inputBatch=torch::relu(fc1(inputBatch));
    inputBatch=dropout(inputBatch);
    inputBatch=torch::relu(fc2(inputBatch));
    inputBatch=dropout(inputBatch);
    inputBatch=torch::relu(fc3(inputBatch));
    inputBatch=dropout(inputBatch);
    inputBatch=torch::relu(fc4(inputBatch));
    return inputBatch;
}

// torch::Tensor CubeNetImpl::forward(torch::Tensor inputBatch){
//     inputBatch=inputBatch.view({inputBatch.size(0)-1,-1});
//     inputBatch=torch::relu((*fc1)(inputBatch));
//     inputBatch=(*dropout)(inputBatch);
//     inputBatch=torch::relu((*fc2)(inputBatch));
//     inputBatch=(*dropout)(inputBatch);
//     inputBatch=torch::relu((*fc3)(inputBatch));
//     inputBatch=(*dropout)(inputBatch);
//     inputBatch=torch::relu((*fc4)(inputBatch));
//     return inputBatch;
// }

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
