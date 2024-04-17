#include <iostream>
#include "File_Processor.h"
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <sstream>
#include <list>
#include <tuple>

File_Processor::File_Processor()
    :filename{}, data{},converted_data{},selected_data{},trained_data{},test_data{}{

    }

File_Processor::File_Processor(const std::string& filename)
    :filename{filename}, data{},converted_data{},selected_data{},trained_data{},test_data{}{

    }

File_Processor::~File_Processor(){

}

void File_Processor:: ReadFile(const std::string &filename){
    std::string line;
    std::ifstream myfile (filename);

    if (!myfile.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }

    while(std::getline(myfile,line)){
            //std::cout<<line<<std::endl;
            data.push_back(line);
    }
    myfile.close();

    /*Take every row and extract the data. Convert from string to double type when it can be done*/
    for(const auto &row : data){
        //std::cout<<row<<std::endl;
        std::istringstream iss(row);
        std::vector<VariantType> rowData;
        std::string token;

        while(std::getline(iss,token,' ')){
            try{
                double value{std::stod(token)};
                rowData.push_back(value);
            }
            catch(...){
                rowData.push_back(token);
            }
        }
        converted_data.push_back(rowData);
    }

    /*Print the list of Vectors*/
    /*for (const auto& inner_vector : converted_data) {
        for (const auto& element : inner_vector) {
            std::visit([](const auto& arg) { std::cout << arg << " "; }, element);
        }
        std::cout << std::endl;
    }*/
}

void File_Processor:: SearchSpecificData(double &min_value, double &max_value, int &indx_to_check){
    for(const auto& inner_vector : converted_data){
        if(std::holds_alternative<double>(inner_vector.at(indx_to_check)) && std::get<double>(inner_vector.at(indx_to_check)) > min_value && std::get<double>(inner_vector.at(indx_to_check))<max_value){
            selected_data.push_back(inner_vector);
        }
    }
}

void File_Processor:: ShuffleValues(){
    auto random_gen = std::default_random_engine {};
    std::shuffle(selected_data.begin(),selected_data.end(),random_gen);

    /*std::cout<<"Shuffled_data"<<std::endl;
    for (const auto& inner_vector : selected_data) {
        for (const auto& element : inner_vector) {
            std::visit([](const auto& arg) { std::cout << arg << " "; }, element);
        }
        std::cout << std::endl;
    }*/
}

std::tuple<std::vector<std::vector<VariantType>>, std::vector<std::vector<VariantType>>> File_Processor:: SplitValues(int &train_percentage){
    size_t total_num_data {selected_data.size()};
    double train_fraction = static_cast<double>(train_percentage) / 100.0;
    size_t train_num = static_cast<size_t>(total_num_data * train_fraction);
    trained_data.assign(selected_data.begin(),selected_data.begin()+train_num);
    test_data.assign(selected_data.begin()+train_num, selected_data.end());
    /*std::cout<<"trained"<<std::endl;
    for (const auto& inner_vector : trained_data) {
        for (const auto& element : inner_vector) {
            std::visit([](const auto& arg) { std::cout << arg << " "; }, element);
        }
        std::cout << std::endl;
    }

    std::cout<<"test"<<std::endl;
    for (const auto& inner_vector : test_data) {
        for (const auto& element : inner_vector) {
            std::visit([](const auto& arg) { std::cout << arg << " "; }, element);
        }
        std::cout << std::endl;
    }
    */
    return std::make_tuple(trained_data, test_data);
}


