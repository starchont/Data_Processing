#ifndef _FILE_PROCESSOR_H_
#define _FILE_PROCESSOR_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <sstream>
#include <variant>
#include <list>
#include <tuple>

using VariantType = std::variant<double,std::string>;

class File_Processor{
private:
    std::string filename;
    std::vector<std::string> data;
    std::list<std::vector<VariantType>> converted_data;
    std::vector<std::vector<VariantType>> selected_data;
    std::vector<std::vector<VariantType>> trained_data;
    std::vector<std::vector<VariantType>> test_data;

public:
    void ReadFile(const std::string &filename);
    void SearchSpecificData(double &min_value, double &max_value, int &indx_to_check);
    void ShuffleValues();
    std::tuple<std::vector<std::vector<VariantType>>, std::vector<std::vector<VariantType>>> SplitValues(int &train_percentage);
    File_Processor(const std::string& filename);
    File_Processor();
    ~File_Processor();
};

#endif