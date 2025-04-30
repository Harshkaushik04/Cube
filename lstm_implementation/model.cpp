#include "headers/model.hpp"
using namespace std;

CubeNetImpl::CubeNetImpl() { // Default constructor
    this->learningRate = 0;
    this->numActions = 0;
    lstm = nullptr;
    fc = nullptr;
    optimizer = nullptr;
}

CubeNetImpl::CubeNetImpl(float learningRate, int hiddenSize, int numActions) {
    this->learningRate = learningRate;
    this->hiddenSize = hiddenSize;
    this->numActions = numActions;

    lstm = torch::nn::LSTM(torch::nn::LSTMOptions(inputSize, hiddenSize).num_layers(2).batch_first(true));
    fc = torch::nn::Linear(hiddenSize, numActions);

    // Register layers
    register_module("lstm", lstm);
    register_module("fc", fc);

    optimizer = make_shared<torch::optim::Adam>(this->parameters(), torch::optim::AdamOptions(this->learningRate));
}

torch::Tensor CubeNetImpl::forward(torch::Tensor inputBatch) {
    inputBatch = inputBatch.to(torch::kCUDA);

    // Reshape for LSTM input (batch_size, sequence_length, input_size)
    inputBatch = inputBatch.view({inputBatch.size(0), -1, inputSize});

    // Initialize hidden state and cell state
    auto h0 = torch::zeros({2, inputBatch.size(0), hiddenSize}).to(torch::kCUDA);
    auto c0 = torch::zeros({2, inputBatch.size(0), hiddenSize}).to(torch::kCUDA);

    // LSTM forward pass
    auto lstm_out = lstm->forward(inputBatch, {h0, c0}).output;

    // Take the last time step's output
    auto last_hidden_state = lstm_out.index({torch::indexing::Slice(), -1, torch::indexing::Slice()});

    // Fully connected layer for final prediction
    auto output = fc(last_hidden_state);
    return output;
}

shared_ptr<CubeNetImpl> CubeNetImpl::clone() {
    return std::make_shared<CubeNetImpl>(*this);
}

// Save Model
void CubeNetImpl::saveModel(const std::string& path) {
    torch::serialize::OutputArchive archive;
    this->save(archive);
    archive.save_to(path);
}

// Load Model
void CubeNetImpl::loadModel(const std::string& path) {
    torch::serialize::InputArchive archive;
    archive.load_from(path);
    this->load(archive);
    std::cout << "Model loaded from " << path << std::endl;
}
