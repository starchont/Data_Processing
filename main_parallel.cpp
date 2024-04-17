#include <iostream>
#include "File_Processor.h"
#include <filesystem>
#include <chrono>
#include <omp.h>

std::vector<std::string> getTxtFiles(const std::string& path) {
    std::vector<std::string> txtFiles{};
    try {
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                txtFiles.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    return txtFiles;
}

int main() {
    std::string path{"C:/Users/steph/Desktop/steph/Thesis/final_code_thesis/Data"};
    std::vector<std::string> filenames = getTxtFiles(path);
    double max_val{20};
    double min_val{8};
    int column{4};
    int perc{80};
    int file_count{0};
    std::vector<std::vector<VariantType>> final_trained_data;
    std::vector<std::vector<VariantType>> final_test_data;

    try {
        auto start = std::chrono::steady_clock::now();

        const int batchSize = 5; // Number of files to process in each batch
        #pragma omp parallel for shared(final_trained_data, final_test_data, file_count) schedule(dynamic, 1)
        for (size_t i = 0; i < filenames.size(); i += batchSize) {
            std::vector<std::vector<VariantType>> trained_data_batch;
            std::vector<std::vector<VariantType>> test_data_batch;
            int localFileCount = 0;

            for (size_t j = i; j < std::min(i + batchSize, filenames.size()); ++j) {
                std::string filename = filenames[j];
                File_Processor processor(filename);
                processor.ReadFile(filename);
                processor.SearchSpecificData(min_val, max_val, column);
                processor.ShuffleValues();
                auto result = processor.SplitValues(perc);

                trained_data_batch.insert(trained_data_batch.end(), std::get<0>(result).begin(), std::get<0>(result).end());
                test_data_batch.insert(test_data_batch.end(), std::get<1>(result).begin(), std::get<1>(result).end());
                ++localFileCount;
            }

            // Merge batch data into final vectors
            #pragma omp critical
            {
                final_trained_data.insert(final_trained_data.end(), trained_data_batch.begin(), trained_data_batch.end());
                final_test_data.insert(final_test_data.end(), test_data_batch.begin(), test_data_batch.end());
                file_count += localFileCount;
            }
        }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        std::cout << "Processed " << file_count << " files in " << elapsed_seconds.count() << " seconds." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
