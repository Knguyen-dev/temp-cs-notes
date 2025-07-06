import matplotlib.pyplot as plt
import numpy as np


class AdalineGD(object):
    """ADAptive LInear Neuron classifier

    Parameters
    ----------
    eta: float
      Learning rate (0.0, 1.0)
    n_iter: int
      Number of passes over the training data set
    random_state: int
      Random number generator seed that we use to initialize small random weights

    Attributes
    ----------
    w_: 1d-array
      Weights after fitting
    cost_: list
      Sum-of-squares cost function value in each epoch/iteration

    NOTE: Batch gradient descent version
    """

    def __init__(self, eta=0.01, n_iter=50, random_state=1):
        self.eta = eta
        self.n_iter = n_iter
        self.random_state = random_state

    def net_input(self, X):
        """Calculate net input 

        NOTE: In this example, we have to include w._[0] after because we haven't prepared a column vector of ones for our data matrix X.
        In a serious implementation you would probably include that column vector of ones.
        """
        return np.dot(X, self.w_[1:]) + self.w_[0]
  
    def activation(self, X):
        """Compute the linear activation. It's really simple in this case, as the linear 
        activation function was really just the dot product $w^{T}x$, but that was already done, so there's not 
        much to really do except return the row vector. 
        
        NOTE: Of course we'll calculate errors and update weights, but that happens after we calculate the "linear activation"
        and it's going to be done in our fit function. Here it doesn't modify things, so it's an identity function, but we still 
        have it to show teh process that we have a linear activation function.
        """
        return X
    
    def predict(self, X):
        """Return class label after our linear activation function is used.
        
        Basically if the net input is greater than 0, return 1, else -1.
        """
        return np.where(self.activation(self.net_input(X)) >= 0.0, 1, -1)

    def fit(self, X, y):
        """Fit (learn from) the training data

        Parameters
        ----------

        X: {array-like, the data matrix}, shape = [n_samples, n_features]
          The data matrix that contains all the row vectors representing the rows in your
          training data.
        y: array-like, shape=[n_samples]
          A vector containing the target values that we want to predict.

        Returns
        --------
        self: object
        """

        rgen = np.random.RandomState(self.random_state)
        self.w_ = rgen.normal(loc=0.0, scale=0.01, size= 1+ X.shape[1])
        self.cost_ = []

        for i in range(self.n_iter):

            # 1. Calculate the net input for all rows.
            net_input = self.net_input(X)

            # 2. Use activation function; 
            output = self.activation(net_input)

            # 3. Calculate the amount of error we have, actual - predicted
            # We'll get a vector of error values for each row in the sample
            errors = (y - output)

            # 4. Update the weights based on respective error amounts 
            # NOTE: The formula was listed sum of (actual-expected) * feature_value, but 
            # how does this relate? This is matrix vector multiplication. The error is the actual-expected and 
            # using with the dot product, we are multiplying a feature by its respective error. Then we're summing it up.
            # You should realize that this is batch gradient descent.
            self.w_[1:] += self.eta * X.T.dot(errors)

            # Then for updating the unit bias, just do a simple sum of error values multiplied by learning rate.
            self.w_[0] += self.eta * errors.sum()

            # 5. Calculate loss value for the given epoch; using formula above
            cost = (errors**2).sum() / 2


'''
The figure with n = 0.01, just shows that as epochs increase, the amount of errors 
actually increase. Whilst the plot with eta =0.0001 shows our cost decreasing as epochs increase.
This is just to illustrate that having too high a learning rate could make your model less accurate.


page: 87
'''
