/**
 * @file blob_generator.hpp
 * @author Reza Namazi (namazir@mcmaster.ca)
 * @brief This file contains the declaration and definition of the blob
 * generation function
 * @version 1.0
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <iostream>
#include <random>
#include <vector>

#include "kmeans.hpp"

/**
 * @brief Generate a blob of points and save in a file
 *
 * @param fileName Name of the file to save the points in
 * @param numPoints Number of points to generate
 * @param numDimensions Number of dimensions of each points in the dataset
 * @param numClusters Number of desired clusters in the dataset
 * @param radius Radius of the blob - the points will be generated around the
 * centroids. The radius is the distance between the centroid and the points in
 * the blob. The smaller the radius, the more dense the blob will be.
 */
void generateBlob(char *fileName, uint64_t numPoints, uint64_t numDimensions,
                  uint64_t numClusters, double radius) {
    // Initialize the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // First line contains the number of points in the dataset
    file << numPoints << std::endl;

    // Second line contains the number of dimensions
    file << numDimensions << std::endl;

    // Initialize the centroids
    std::vector<Point> centroids(numClusters);
    for (uint64_t i = 0; i < numClusters; i++) {
        centroids[i] = Point(numDimensions);
        for (uint64_t j = 0; j < numDimensions; j++) {
            // Generate a random number between 0 and 1
            centroids[i].coordinates[j] = (double)gen() / gen.max();
        }
    }

    // Generate the points
    for (uint64_t i = 0; i < numPoints; i++) {
        // Choose a random centroid
        uint64_t centroidIndex = uint64_t(rand()) % numClusters;

        // Generate a random point around the centroid
        Point point(numDimensions);
        for (uint64_t j = 0; j < numDimensions; j++) {
            // Generate a random number between -1 and 1
            double random = (double)gen() / gen.max() * 2 - 1;

            // Multiply the random number by the radius and add it to the
            // centroid
            point.coordinates[j] =
                centroids[centroidIndex].coordinates[j] + random * radius;
        }

        // Write the point to the file
        for (uint64_t j = 0; j < numDimensions; j++) {
            file << point.coordinates[j] << " ";
        }
        file << std::endl;
    }

    file.close();
}