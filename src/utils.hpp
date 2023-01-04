/**
 * @file utils.hpp
 * @author Reza Namazi (namazir@mcmaster.ca)
 * @brief A header file for utility functions used in the K-means clustering
 * program
 * @version 1.0
 * @date 2022-12-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "kmeans.hpp"

/**
 * @brief Read the dataset from a file
 *
 * @param points
 * @param filename name of the file
 * @param numPoints number of points in the dataset
 * @param numDimensions number of dimensions
 */
void readDataset(std::vector<Point> &points, char *filename,
                 uint64_t &numPoints, uint64_t &numDimensions) {
    std::FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        throw std::runtime_error("Could not open file");
    }

    // First line contains the number of points in the dataset
    fscanf(fp, "%lu", &numPoints);
    points.resize(numPoints);

    // Second line contains the number of dimensions
    fscanf(fp, "%lu", &numDimensions);

    // From the third line, each line contains the coordinates of a point
    for (uint64_t i = 0; i < numPoints; i++) {
        points[i] = Point(numDimensions);
        for (uint64_t j = 0; j < numDimensions; j++) {
            fscanf(fp, "%lf", &points[i].coordinates[j]);
        }
    }

    fclose(fp);
}

/**
 * @brief Print the dataset
 *
 * @param points pointer to the array of points
 * @param numPoints number of points in the dataset
 * @param numDimensions number of dimensions
 */
void printDataset(std::vector<Point> &points, uint64_t numPoints,
                  uint64_t numDimensions) {
    for (uint64_t i = 0; i < numPoints; i++) {
        for (uint64_t j = 0; j < numDimensions; j++) {
            std::cout << points[i].coordinates[j] << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Find the optimal value of k using the elbow method
 *
 * @param numPoints number of points in the dataset
 * @param numDimensions number of dimensions
 * @param points pointer to the array of points
 * @param minK minimum value of k
 * @param maxK maximum value of k
 * @return uint64_t optimal value of k
 */
uint64_t elbowMethod(uint64_t numPoints, uint64_t numDimensions,
                     std::vector<Point> points, uint64_t minK, uint64_t maxK) {
    // Check if minK is less than 1
    if (minK < 1) {
        throw std::runtime_error("Minimum value of k should be greater than 0");
    }

    // Check if maxK is less than minK
    if (maxK < minK) {
        throw std::runtime_error(
            "Maximum value of k should be greater than minimum value of k");
    }

    // Store all inertia values
    std::vector<double_t> inertia;

    // Run k-means for all values of k
    for (uint64_t k = minK; k <= maxK; k++) {
        KMeans kmeans(k, numDimensions, numPoints, points);
        kmeans.fit(100, 1e-6);
        inertia.push_back(kmeans.inertia());
    }

    // Connect a line between the first and last point and find the point
    // which is farthest from the line (elbow point)

    // The equation of the line is y = mx + b

    // m = (y2 - y1) / (x2 - x1)
    double m = (inertia[inertia.size() - 1] - inertia[0]) / double(maxK - minK);

    // b = y - mx
    double b = inertia[0] - m * double(minK);

    // Find the point which is farthest from the line
    double maxDistance = 0;
    uint64_t elbowPoint = 0;
    for (uint64_t i = 0; i < uint64_t(inertia.size()); i++) {
        // Distance of a point from a line is given by
        // d = |mx + b - y| / sqrt(m^2 + (-1)^2)
        uint64_t x = i + minK;
        double y = inertia[i];
        double distance =
            std::abs(m * double(x) + b - y) / std::sqrt(m * m + 1);

        if (distance > maxDistance) {
            maxDistance = distance;
            elbowPoint = i + minK;
        }
    }

    return elbowPoint;
}