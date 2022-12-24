# K-Means Clustering

K-Mean Clustering is a method of vector quantization, originally from signal processing, that is popular for cluster analysis in data mining. K-Mean Clustering aims to partition n observations into k clusters in which each observation belongs to the cluster with the nearest mean, serving as a prototype of the cluster.

# How K-Means Clustering Works

In the first step, the algorithm randomly selects k points from the data set as the initial cluster centers. Then, the algorithm iterates between two steps until convergence:

1. Assign each observation to the cluster whose center is closest to it.

2. Update the cluster centers to be the mean of the observations in the cluster.

Each trained model will be saved in a file, and the user will be able to load the model and use it to predict the cluster of a new observation.

# How does the program determine the number of clusters K?

This program will use the elbow method to automatically select the number of clusters K. Elbow method is a heuristic method of interpretation and validation of consistency within cluster analysis designed to help finding the appropriate number of clusters in a dataset. The method consists of plotting the explained variance as a function of the number of clusters, and picking the elbow of the curve as the number of clusters to use.

Below is a an example of what an elbow plot looks like:

![image](/inertia.png)

To select the elbow, the program will find the line that connects the first and last point, and then find the point that is the farthest from the line.

Below is an example of how the program finds the elbow:

![image](/optimal-k.png)

# Compilation

The program can be compiled using the following command:

```bash
g++ main.cpp -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wpedantic -o kmeans
```

# Usage

The program can be used in two modes: training and prediction.

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

In prediction mode, user can load a model and use it to predict the cluster of a new observation (or on the same dataset that it was trained on). The program will then save the prediction in a file.

- Prediction:

```bash
./kmeans <input_file> <model_file> <output_file>
```

# Example

This repository contains a sample blob dataset that was generated using `scikit-learn`'s `make_blobs` function. The dataset contains 1000 samples with 2 features. The dataset is saved in `data/blob.txt`.

To train a model with 3 clusters on the dataset, run the following command:

```bash
./kmeans data/blob.txt 3 100 0.001 model.txt
```

The model will be saved in `model.txt`.

After training, to predict the cluster of the blob dataset, run the following command:

```bash
./kmeans data/blob.txt model.txt prediction.txt
```

The prediction will be saved in `prediction.txt`.

# Experiment with the blob dataset

To further experiment with the blob dataset, you can use the `make_blob.py` included in this repository. This script will generate a blob dataset with the desired settings:

For example:

```bash
python3 make_blob.py --n_samples 1000 --n_features 2 --centers 3 --cluster_std 1.0 --random_state 42 --output data/blob.txt
```

I have also included another script to visualize the dataset and the prediction. To visualize the dataset, run the following command:

```bash
python3 plot.py --data data/blob.txt --clusters data/clusters.txt --fig data/plot.png
```

You will need to install `scikit-learn`, `matplotlib`, and `numpy` to run the scripts:

```bash
pip3 install scikit-learn matplotlib numpy
```
