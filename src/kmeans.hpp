/**
 * @file kmeans.hpp
 * @author Reza Namazi (namazir@mcmaster.ca)
 * @brief A header file for the K-means clustering algorithm
 * @version 1.0
 * @date 2022-12-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief A class to represent a point in the dataset
 */
class Point {
   public:
    uint64_t numDims;                 // number of dimensions
    std::vector<double> coordinates;  // pointer to the array of coordinates
    uint64_t cluster;  // cluster number to which the point belongs
    /**
     * @brief Construct a new Point object
     *
     */
    Point() {}
    /**
     * @brief Construct a new Point object
     *
     * @param n number of dimensions
     */
    Point(uint64_t n) {
        this->numDims = n;
        this->coordinates.resize(n);
    }
};

/**
 * @brief A class to represent the K-means clustering algorithm
 */
class KMeans {
   public:
    uint64_t numClusters;          // number of clusters
    uint64_t numDims;              // number of dimensions
    uint64_t numPoints;            // number of points in the dataset
    std::vector<Point> points;     // pointer to the array of points
    std::vector<Point> centroids;  // pointer to the array of centroids

    /**
     * @brief Construct a new KMeans object
     *
     * @param k number of clusters
     * @param n number of dimensions
     * @param numDataPoints number of points in the dataset
     * @param dataPoints pointer to the array of points
     */
    KMeans(uint64_t k, uint64_t n, uint64_t numDataPoints,
           std::vector<Point> dataPoints) {
        this->numClusters = k;
        centroids.resize(k);
        for (uint64_t i = 0; i < k; i++) {
            centroids[i] = Point(n);
        }
        this->numDims = n;
        this->numPoints = numDataPoints;
        this->points = dataPoints;
    }

    /**
     * @brief Construct a new KMeans object
     *
     * @param numDataPoints number of points in the dataset
     * @param dataPoints pointer to the array of points
     * @param filename path to the file containing the model
     */
    KMeans(uint64_t numDataPoints, std::vector<Point> dataPoints,
           char *filename) {
        this->numPoints = numDataPoints;
        this->points = dataPoints;

        // Load the model from the file
        this->loadModel(filename);
    }

    /**
     * @brief Randomly initialize the centroids to points in the dataset
     *
     */
    void initializeCentroids() {
        // An array to keep track of the selected centroids so that we don't
        // select the same point twice as a centroid
        std::vector<bool> selected(numPoints, false);
        for (uint64_t i = 0; i < numClusters; i++) {
            uint64_t index = uint64_t(rand()) % numPoints;
            while (selected[index]) {
                index = uint64_t(rand()) % numPoints;
            }
            selected[index] = true;
            for (uint64_t j = 0; j < numDims; j++) {
                centroids[i].coordinates[j] = points[index].coordinates[j];
            }
        }
    }

    /**
     * @brief Assign points to the nearest centroid
     *
     */
    void assignPointsToCentroids() {
        for (uint64_t i = 0; i < numPoints; i++) {
            double minDistance = 1e9;  // initialize the minimum distance to a
                                       // large number (infinity)
            uint64_t cluster = 0;
            for (uint64_t j = 0; j < numClusters; j++) {
                double distance = 0;
                for (uint64_t l = 0; l < numDims; l++) {
                    double _distance =
                        points[i].coordinates[l] - centroids[j].coordinates[l];
                    distance += _distance * _distance;
                }
                if (distance < minDistance) {
                    minDistance = distance;
                    cluster = j;
                }
            }
            points[i].cluster = cluster;
        }
    }

    /**
     * @brief Update the centroids to the mean of the points in the cluster
     *
     */
    void updateCentroids() {
        // Initialize the centroids to zero
        std::vector<uint64_t> numPointsInCluster(numClusters, 0);
        for (uint64_t i = 0; i < numClusters; i++) {
            for (uint64_t j = 0; j < numDims; j++) {
                centroids[i].coordinates[j] = 0;
            }
        }

        // Add the coordinates of all points in a cluster
        for (uint64_t i = 0; i < numPoints; i++) {
            uint64_t cluster = points[i].cluster;
            numPointsInCluster[cluster]++;
            for (uint64_t j = 0; j < numDims; j++) {
                centroids[cluster].coordinates[j] += points[i].coordinates[j];
            }
        }

        // Divide the sum by the number of points in the cluster to get the
        // coordinates of the centroid
        for (uint64_t i = 0; i < numClusters; i++) {
            for (uint64_t j = 0; j < numDims; j++) {
                centroids[i].coordinates[j] /= double(numPointsInCluster[i]);
            }
        }
    }

    /**
     * @brief Run the k-means algorithm until a threshold is reached or maximum
     * number of iterations is reached
     *
     * @param maxIterations maximum number of iterations
     * @param threshold threshold
     */
    void fit(uint64_t maxIterations, double threshold) {
        initializeCentroids();
        uint64_t iteration = 0;
        while (iteration < maxIterations) {
            assignPointsToCentroids();

            // Store the old centroids
            std::vector<Point> oldCentroids(numClusters);
            for (uint64_t i = 0; i < numClusters; i++) {
                oldCentroids[i] = Point(numDims);
                for (uint64_t j = 0; j < numDims; j++) {
                    oldCentroids[i].coordinates[j] =
                        centroids[i].coordinates[j];
                }
            }

            updateCentroids();

            // Calculate the maximum distance between the old and new centroids
            double maxDistance = 0;
            for (uint64_t i = 0; i < numClusters; i++) {
                double distance = 0;
                for (uint64_t j = 0; j < numDims; j++) {
                    double _distance = oldCentroids[i].coordinates[j] -
                                       centroids[i].coordinates[j];
                    distance += _distance * _distance;
                }
                if (distance > maxDistance) {
                    maxDistance = distance;
                }
            }

            // If the maximum distance is less than the threshold, stop the
            // algorithm
            if (maxDistance < threshold) {
                break;
            }

            iteration++;
        }
    }

    /**
     * @brief Save the predictions to a file
     *
     * @param filename
     */
    void savePredictions(std::string filename) {
        assignPointsToCentroids();
        std::ofstream file(filename);
        for (uint64_t i = 0; i < numPoints; i++) {
            file << points[i].cluster << std::endl;
        }
    }

    /**
     * @brief Calculate the inertia of the model (sum of squared distances of
     * samples to their closest cluster center)
     *
     * @return double
     */
    double inertia() {
        double inertia = 0;
        for (uint64_t i = 0; i < numPoints; i++) {
            uint64_t cluster = points[i].cluster;
            for (uint64_t j = 0; j < numDims; j++) {
                double distance = points[i].coordinates[j] -
                                  centroids[cluster].coordinates[j];
                inertia += distance * distance;
            }
        }
        return inertia;
    }

    /**
     * @brief Save the model to a file
     *
     * @param filename
     */
    void saveModel(std::string filename) {
        std::ofstream file(filename);
        file << numClusters << std::endl;  // First line is number of clusters
        file << numDims << std::endl;  // Second line is number of dimensions
        // Next lines are the coordinates of the centroids
        for (uint64_t i = 0; i < numClusters; i++) {
            for (uint64_t j = 0; j < numDims; j++) {
                file << centroids[i].coordinates[j] << " ";
            }
            file << std::endl;
        }
        file.close();
    }

    /**
     * @brief Load the model from a file
     *
     * @param filename
     */
    void loadModel(std::string filename) {
        std::ifstream file(filename);
        file >> numClusters;  // First line is number of clusters
        file >> numDims;      // Second line is number of dimensions
        this->numClusters = numClusters;
        centroids.resize(numClusters);
        this->numDims = numDims;
        // Next lines are the coordinates of the centroids
        std::vector<Point> oldCentroids(numClusters);
        for (uint64_t i = 0; i < numClusters; i++) {
            centroids[i] = Point(numDims);
            for (uint64_t j = 0; j < numDims; j++) {
                file >> centroids[i].coordinates[j];
            }
        }
        file.close();
    }
};
