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

#include "kmeans.hpp"

/**
 * @brief Generate a blob of points and save in a file - points are distributed
 * around k centroids
 *
 * @param fileName name of the file
 * @param numPoints number of points in the blob
 * @param numDimensions number of dimensions
 * @param numClusters number of clusters
 */
void generateBlob(char *fileName, uint64_t numPoints, uint64_t numDimensions,
                  uint64_t numClusters, double radius) {
    time_t nTime;
    srand((unsigned)time(&nTime));

    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // First line contains the number of points in the dataset
    file << numPoints << std::endl;

    // Second line contains the number of dimensions
    file << numDimensions << std::endl;

    // Initialize the centroids
    Point *centroids = new Point[numClusters];
    for (uint64_t i = 0; i < numClusters; i++) {
        centroids[i] = Point(numDimensions);
        for (uint64_t j = 0; j < numDimensions; j++) {
            centroids[i].coordinates[j] = (double)rand() / RAND_MAX;
        }
    }

    // Generate the points
    for (uint64_t i = 0; i < numPoints; i++) {
        // Choose a random centroid
        uint64_t centroidIndex = uint64_t(rand()) % numClusters;

        // Generate a random point around the centroid
        Point point(numDimensions);
        for (uint64_t j = 0; j < numDimensions; j++) {
            point.coordinates[j] = centroids[centroidIndex].coordinates[j] +
                                   ((double)rand() / RAND_MAX - 0.5) * radius;
        }

        // Write the point to the file
        for (uint64_t j = 0; j < numDimensions; j++) {
            file << point.coordinates[j] << " ";
        }
        file << std::endl;
    }

    file.close();
}