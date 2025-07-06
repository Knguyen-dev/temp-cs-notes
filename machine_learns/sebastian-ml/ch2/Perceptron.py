import numpy as np


class Perceptron:
    """
    Perceptron classifier.
    ## Parameters:
    eta: float
        Learning rate between 0.0 and 1.0.
    n_iter: int
        Passes over the training dataset.
    random_state: int
        Random number generator seed for initializing small random weights.
    ## Attributes:
    w_: 1d-array
        Weights after fitting.
    errors_: list
        Number of misclassifications (updates) in each epoch.
    """

    def __init__(self, eta=0.01, n_iter=50, random_state=1):
        self.eta = eta
        self.n_iter = n_iter
        self.random_state = random_state


    def net_input(self, X):
        """Calculate the net input for some sample/row
        NOTE: self.w_ is created when .fit is run, and it persists afterwards.
        We're calculating the net input for a given row using that dot product
        NOTE 2: You could calculate the dot product without NumPy, but Numpy is more efficient. Operations are vectorized, meaning
        arithmetic operations are automatically applied to all elements.
        """
        return np.dot(X, self.w_[1:]) + self.w_[0]


    def predict(self, X):
        """Return class label, or predicted class label. So this is your decision function, here we made
        the threshold 0."""

        # Calculate the net input for X (some sample/row), if it's greater than 0, return 1, else -1 (1 and -1 are class labels)

        return np.where(self.net_input(X) >= 0.0, 1, -1)


    def fit(self, X, y):
        """Fit training data. So basically this is the learning part of the Perceptron algorithm
        Parameters
        ----------
        X: {array-like, the data matrix}, shape = [n_samples, n_features]
          Training vectors, where n_samples is the number of samples/rows, and n_features is the
          number of features/input columns.
        y: array-like, shape={n_samples}
          Target values. So this is the vector containing the outcome values that we are training the perceptron to predict.
        Returns
        --------
        self: Object, doesn't really return anything.
        """
        rgen = np.random.RandomState(self.random_state)
        
        # Generate small random values for the weights. We add 1 as the first element in the row vector to represent the bias unit.
        # NOTE: If weights were 0, the learning rate n only affects hte magnitude of the change in weight. We want it to affect
        # magnitude and direction. That's the explanation t least, I don't really get it
        
        self.w_ = rgen.normal(loc=0.0, scale=0.01, size=1 + X.shape[1])
        self.errors_ = []
        for _ in range(self.n_iter):
            errors = 0
            '''
            + Loop through all the rows:

            1.  Do a prediction and calculate change in weight
            NOTE: delta weight = n(actual - predicted) * x^{i}_{j};

            2. Update all your weights with respect to the value of their features, and we achieve this by doing
            scalar multiplication with the row vector of features.

            3. If update != 0, this means we had a misclassification. So this expression evaluates to 
            true, and therefore 1. Allowing us a neat way to keep track of misclassifications.
            '''
            for xi, target in zip(X, y):
                update = self.eta * (target - self.predict(xi))
                self.w_[1:] += update * xi
                self.w_[0] += update
                errors += int(update != 0.0)
            self.errors_.append(errors)
        return self