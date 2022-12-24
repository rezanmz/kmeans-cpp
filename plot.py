from argparse import ArgumentParser
from matplotlib import pyplot as plt
from sklearn.decomposition import PCA
import numpy as np

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--data', type=str, default='data/blobs.txt')
    parser.add_argument('--clusters', type=str, default='data/clusters.txt')
    parser.add_argument('--fig', type=str, default='data/plot.png')
    args = parser.parse_args()
 
    with open(args.data, 'r') as f:
        n_samples = int(f.readline())
        n_features = int(f.readline())
 
        X = np.zeros((n_samples, n_features))
        for i in range(n_samples):
            line = f.readline()
            X[i] = np.array(list(map(float, line.split())))
    
    with open(args.clusters, 'r') as f:
        y = f.readlines()
        y = np.array(list(map(int, y)))

    X = PCA(n_components=2).fit_transform(X)

    plt.scatter(X[:, 0], X[:, 1], c=y)
    plt.savefig(args.fig)