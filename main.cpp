#include<iostream>
#include "File_Processor.h"
#include <filesystem>
#include <chrono>


std::vector<std::string> getTxtFiles(const std::string& path){
    std::vector<std::string> txtFiles{};
    try{
        for(const auto &entry : std::filesystem::directory_iterator(path)){
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                txtFiles.push_back(entry.path().filename().string());
            }
        }
    }catch(const std::filesystem::filesystem_error& e){
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    return txtFiles;
}


int main(){
    std::string path{"C:/Users/steph/Desktop/steph/Thesis/final_code_thesis/Data"};
    std::vector<std::string> filenames = getTxtFiles(path);
    double max_val {20};
    double min_val {8};
    int column {4};
    int perc{80};
    int file_count{0};
    std::vector<std::vector<VariantType>> final_trained_data;
    std::vector<std::vector<VariantType>> final_test_data;
    //std::string filename{"APR_1.txt"};

    try{
        auto start = std::chrono::steady_clock::now();
        for (const auto& filename : filenames) {
            File_Processor processor(filename);
            processor.ReadFile(filename);
            processor.SearchSpecificData(min_val, max_val, column);
            processor.ShuffleValues();
            auto result = processor.SplitValues(perc);

            std::vector<std::vector<VariantType>> trained_data = std::get<0>(result);
            std::vector<std::vector<VariantType>> test_data = std::get<1>(result);

            // Add the data to final vectors
            final_trained_data.insert(final_trained_data.end(), trained_data.begin(), trained_data.end());
            final_test_data.insert(final_test_data.end(), test_data.begin(), test_data.end());
            ++file_count;
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        /*
        // Print final trained data
        std::cout << "Final Trained Data:" << std::endl;
        for (const auto& inner_vector : final_trained_data) {
            for (const auto& element : inner_vector) {
                std::visit([](const auto& arg) { std::cout << arg << " "; }, element);
            }
            std::cout << std::endl;
        }

        // Print final test data
        std::cout << "Final Test Data:" << std::endl;
        for (const auto& inner_vector : final_test_data) {
            for (const auto& element : inner_vector) {
                std::visit([](const auto& arg) { std::cout << arg << " "; }, element);
            }
            std::cout << std::endl;
        }
        */

        std::cout << "Processed " << file_count << " files in " << elapsed_seconds.count() << " seconds." << std::endl;
    }catch(const std::exception& e){
        std::cerr<<"Error: "<<e.what()<<std::endl;
        return 1;
    }

    return 0;
}