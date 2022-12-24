from argparse import ArgumentParser
from sklearn.datasets import make_blobs

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--n_samples', type=int, default=100)
    parser.add_argument('--n_features', type=int, default=2)
    parser.add_argument('--centers', type=int, default=3)
    parser.add_argument('--cluster_std', type=float, default=1.0)
    parser.add_argument('--random_state', type=int, default=42)
    parser.add_argument('--output', type=str, default='data/blobs.txt')
    args = parser.parse_args()

    X, y = make_blobs(
        n_samples=args.n_samples,
        n_features=args.n_features,
        centers=args.centers,
        cluster_std=args.cluster_std,
        random_state=args.random_state
    )

    with open(args.output, 'w') as f:
        f.write(f'{X.shape[0]}\n')
        f.write(f'{X.shape[1]}\n')

        for point in X:
            for coordinate in point:
                f.write(f'{coordinate} ')
            f.write('\n')
