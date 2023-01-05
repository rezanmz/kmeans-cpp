# K-Means Clustering

K-Mean Clustering is a method of vector quantization, originally from signal processing, that is popular for cluster analysis in data mining. K-Mean Clustering aims to partition n observations into k clusters in which each observation belongs to the cluster with the nearest mean, serving as a prototype of the cluster.

# How K-Means Clustering Works

In the first step, the algorithm randomly selects k points from the data set as the initial cluster centers. Then, the algorithm iterates between two steps until convergence:

1. Assign each observation to the cluster whose center is closest to it.

2. Update the cluster centers to be the mean of the observations in the cluster.

Each trained model will be saved in a file, and the user will be able to load the model and use it to predict the cluster of a new observation.

# How does the program determine the number of clusters K?

This program will use the elbow method to automatically select the number of clusters K. Elbow method is a heuristic method of interpretation and validation of consistency within cluster analysis designed to help finding the appropriate number of clusters in a dataset. The method consists of plotting the explained variance as a function of the number of clusters, and picking the elbow of the curve as the number of clusters to use.

Below is a an example of what an elbow plot looks like [[Source of the figure]](https://pythonprogramminglanguage.com/kmeans-elbow-method/):

![image](/inertia.png)

To select the elbow, the program will find the line that connects the first and last point, and then find the point that is the farthest from the line.

Below is an example of how the program finds the elbow [[Source of the figure]](https://www.researchgate.net/figure/Selection-of-the-optimal-k-for-the-k-means-algorithm-using-the-elbow-method-089-093_fig4_334611981):

![image](/optimal-k.png)

# Compilation

The program can be compiled using the following command:

```bash
g++ src/main.cpp -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wpedantic -std=c++20 -o kmeans
```

# Usage

The program can be used in three modes: generating sample blobs of data, training, and prediction

## Generating sample blobs of data

The program has a subcommand to generate a blob dataset. The program will generate a dataset and save it in a file:

- Generate a blob dataset:

```bash
./kmeans generate [output_file] [num_points] [num_dimensions] [num_clusters] [radius]
```

## Training

In training mode, user can either specify the number of clusters K or let the program automatically select the number of clusters K. The program will then train the model and save it in a file:

- With a predefined number of clusters:

```bash
./kmeans <input_file> <num_clusters> <max_iters> <threshold>
<model_output_file>
```

- Automatically find the best number of clusters:

```bash
./kmeans <input_file> <min_k> <max_k> <max_iters> <threshold>
<model_output_file>
```

The program will then train the model and save it in a file. The first line of the file will be the number of clusters K, the second line will be the number of dimensions (features), and the rest of the file will be the cluster centers.

## Prediction

In prediction mode, user can load a model and use it to predict the cluster of a new observation (or on the same dataset that it was trained on). The program will then save the prediction in a file.

- Prediction:

```bash
./kmeans <input_file> <model_file> <output_file>
```

# Example

## Dataset generation

To generate a blob dataset with 1000 points in a 2-dimensional space, 3 clusters, and a radius of 0.5, run the following command:

```bash
./kmeans generate data/blobs.txt 1000 2 3 0.5
```

The first line of the file will be the number of points, the second line will be the number of dimensions, and the rest of the file will be the points. An example output is shown below:

```bash
1000
2
0.759904 0.502418
-0.11645 0.251438
0.0676298 0.109861
0.0718524 0.428411
1.07953 0.212233
.
.
.
0.826449 0.217375
0.246605 -0.0679699
0.0836539 0.0667823
0.327049 0.060512
```

It will be saved to `data/blobs.txt`.

## Training with a predefined number of clusters

To train the model on a given dataset, the dataset should be stored in a file similar to the one above. To fit a model with 2 clusters with a maximum of 100 iterations and a threshold of 0.0001, run the following command:

```bash
./kmeans data/blobs.txt 2 100 0.0001 data/model.txt
```

The model will iteratively train until a threshold is reached or the maximum number of iterations is reached. The model will be saved to `data/model.txt`. An example output is shown below:

```bash
2
2
0.728192 0.467138
0.121523 0.549123
```

The first line is the number of clusters, the second line is the number of dimensions, and the rest of the file is the cluster centers.

## Training with automatic number of clusters selection

You can also let the program automatically select the number of clusters K using the elbow method. To fit a model with a minimum of 2 clusters and a maximum of 10 clusters with a maximum of 100 iterations and a threshold of 0.0001, run the following command:

```bash
./kmeans data/blobs.txt 2 10 100 0.0001 data/model.txt
```

The program will then automatically select the number of clusters K. The model will be saved to `data/model.txt`. An example output is shown below:

```bash
3
2
0.516175 0.857023
0.961861 0.507088
0.566069 0.113088
```

The first line is the number of clusters, the second line is the number of dimensions, and the rest of the file is the cluster centers.

## Prediction

To predict the cluster of a new observation, the observation should be stored in a file similar to the one above. To predict the cluster of a new observation, run the following command:

```bash
./kmeans data/blobs.txt data/model.txt data/predictions.txt
```

The program will first load the model from `data/model.txt` and then predict the cluster of each point in `data/blobs.txt`. The prediction will be saved to `data/predictions.txt`. An example output is shown below:

```bash
3
2
0
.
.
.
2
4
```

Each line of the file is the cluster of the corresponding point in `data/blobs.txt`. E.g. the first line represents the cluster of the first point in `data/blobs.txt`, the second line represents the cluster of the second point in `data/blobs.txt`, and so on.

# Visualization

You can use `plot.py` to visualize the dataset and the clusters. This scripts receives the dataset (with the format described above) and the predictions (with the format described above) as input and will plot the data points in two dimensions with their corresponding cluster colors. If the data points are in a higher dimension, the program will first reduce the dimensionality of the data points using PCA. An example of the script is shown below:

```bash
python3 plot.py --data data/blobs.txt --cluster data/predictions.txt --fig data/plot.png
```

The output will be saved to `data/plot.png`. An example output is shown below:

![image](/data/plot.png)
