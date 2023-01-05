/**
 * @file main.cpp
 * @author Reza Namazi (namazir@mcmaster.ca)
 * @brief A program to run the K-means clustering algorithm
 * @version 1.0
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <stdexcept>

#include "blob_generator.hpp"
#include "kmeans.hpp"
#include "utils.hpp"

/**
 * @brief Entry point of the program for the K-means clustering algorithm
 *
 * @param argc number of arguments
 * @param argv array of arguments
 * @return int exit code
 */
int main(int argc, char *argv[]) {
    /*
    Usage:
        - Train:
            - With a predefined number of clusters:
                ./kmeans <input_file> <num_clusters> <max_iters> <threshold>
    <model_output_file>
            - Automatically find the best number of clusters:
                ./kmeans <input_file> <min_k> <max_k> <max_iters> <threshold>
    <model_output_file>
        - Prediction:
            ./kmeans <input_file> <model_file> <output_file>
        - Generate blob dataset:
            ./kmeans generate <file_address> <num_points> <num_dimensions>
    <num_clusters> <radius>
    */

    // Check if the number of arguments is correct
    if (argc != 4 && argc != 6 && argc != 7) {
        std::cout << "Error: Invalid number of arguments";
        return 1;
    }

    // Generate blob dataset
    if (argc == 7 && std::string(argv[1]) == "generate") {
        char *fileAddress = argv[2];
        uint64_t numPoints = std::stoul(argv[3]);
        uint64_t numDimensions = std::stoul(argv[4]);
        uint64_t numClusters = std::stoul(argv[5]);
        double_t radius = std::stod(argv[6]);

        try {
            // Generate dataset
            generateBlob(fileAddress, numPoints, numDimensions, numClusters,
                         radius);
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
            return 1;
        }

    }
    // Train
    else if (argc == 6 || argc == 7) {
        // When the number of clusters is predefined
        if (argc == 6) {
            char *inputFile = argv[1];
            uint64_t numClusters = std::stoul(argv[2]);
            uint64_t maxIters = std::stoul(argv[3]);
            double_t threshold = std::stod(argv[4]);
            char *modelOutputFile = argv[5];

            try {
                // Read dataset
                uint64_t numPoints, numDimensions;
                std::vector<Point> points;
                readDataset(points, inputFile, numPoints, numDimensions);

                // Train
                KMeans kmeans(numClusters, numDimensions, numPoints, points);
                kmeans.fit(maxIters, threshold);

                // Save model
                kmeans.saveModel(modelOutputFile);
            } catch (const std::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
                return 1;
            }

        }
        // Find the number of clusters using the elbow method
        else {
            char *inputFile = argv[1];
            uint64_t minK = std::stoul(argv[2]);
            uint64_t maxK = std::stoul(argv[3]);
            uint64_t maxIters = std::stoul(argv[4]);
            double_t threshold = std::stod(argv[5]);
            char *modelOutputFile = argv[6];

            try {
                // Read dataset
                uint64_t numPoints, numDimensions;
                std::vector<Point> points;
                readDataset(points, inputFile, numPoints, numDimensions);

                // Find the number of clusters
                uint64_t numClusters =
                    elbowMethod(numPoints, numDimensions, points, minK, maxK);

                // Train
                KMeans kmeans(numClusters, numDimensions, numPoints, points);
                kmeans.fit(maxIters, threshold);

                // Save model
                kmeans.saveModel(modelOutputFile);
            } catch (const std::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
                return 1;
            }
        }
    }
    // Predict
    else {
        char *inputFile = argv[1];
        char *modelFile = argv[2];
        char *outputFile = argv[3];

        try {
            // Read dataset
            uint64_t numPoints, numDimensions;
            std::vector<Point> points;
            readDataset(points, inputFile, numPoints, numDimensions);

            // Load model
            KMeans kmeans(numPoints, points, modelFile);

            // Predict
            kmeans.savePredictions(outputFile);
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}