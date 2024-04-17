# Data_Processing

The program processes multiple text files containing data using both parallel and non-parallel programming. It reads large datasets and splits them into train and test data. The `File_Processor` class encapsulates functions for:

1. Reading data from a text file and saving it into a container (a list of vectors).
2. Filtering data based on specified minimum and maximum values of a specific column in the dataset.
3. Shuffling the selected data.
4. Creating a tuple of vectors (train_vector, test_vector).

In `main.cpp`, the program runs in a non-parallel manner and processes 14 files in approximately 0.255 seconds, as tested. Conversely, `main_parallel.cpp` runs in a parallelized version, processing the same datasets of 14 files in approximately 0.156 seconds.

