import numpy as np

class AdalineSGD(object):
  """Adaptive Linear Neuron Classifier Algorithm, but with stochastic gradient descent,
  rather than batch.

  Parameters
  ------------
  eta : float
    Learning rate (between 0.0 and 1.0)
  n_iter : int
    Passes over the training dataset. This is the epoch amount.
  shuffle : bool (default: True)
    Shuffles training data every epoch if True to prevent cycles. 
  random_state : int
    Random number generator seed for random weight.
  initialization.

  Attributes
  -----------
  w_ : 1d-array
    Weights after fitting.
  cost_ : list
    Sum-of-squares cost function value averaged over all training samples in each epoch.
  """
  def __init__(self, eta=0.01, n_iter=10, shuffle=True, random_state=None):
    self.eta = eta
    self.n_iter = n_iter
    self.w_initialized = False
    self.shuffle = shuffle
    self.random_state = random_state
  

  def fit(self, X, y):
    """ Fit training data.
    Parameters
    ----------
    X : {array-like}, shape = [n_samples, n_features]
      Training vectors, where n_samples is the number of samples and n_features is the number of features.
    y : array-like, shape = [n_samples]
      Vector of target values.
    Returns
    -------
    self : object
    """

    # Create a vector of weights for each feature
    self._initialize_weights(X.shape[1])

    # Record the average cost for each epoch
    self.cost_ = []
    for i in range(self.n_iter):

      # The idea of shuffling before each epoch; this is usually done for stochastic gradient descent. 
      if self.shuffle:
        X, y = self._shuffle(X, y)
      
      # Calculate the cost for each sample in X
      cost = []
      for xi, target in zip(X, y):
        cost.append(self._update_weights(xi, target))

      # Sum and average the costs
      avg_cost = sum(cost) / len(y)
      self.cost_.append(avg_cost)
    return self
  
  def partial_fit(self, X, y):
    """
    Fit training data without reinitializing the weights. This is useful
    for online learning when we already have a model as you want to keep 
    the weights, but you want it to keep learning based on new data.
    """
    if not self.w_initialized:
      self._initialize_weights(X.shape[1])

    # Don't really know?
    if y.ravel().shape[0] > 1:
      for xi, target in zip(X, y):
        self._update.weights(xi, target)
    else:
      self._update_weights(X, y)
    return self
  
  def _shuffle(self, X, y):
    """Shuffle training data"""
    r = self.rgen.permutation(len(y))
    return X[r], y[r]
  
  def _initialize_weights(self, m):
    """Initialize weights to small random numbers"""
    self.rgen = np.random.RandomState(self.random_state)
    self.w_ = self.rgen.normal(loc=0.0, scale=0.01, size=1+m)
    self.w_initialized = True

  def _update_weights(self, xi, target):
    """
    Apply Adaline learning rule to update the weights
    1. Get the net input
    2. Get how much we are off by
    3. Apply formula for updating weights. n(actual - predicted)sample_vector. Do this 
    to the feature weights and the unit bias.
    4. Calculate the cost according to the cost function.
    """
    output = self.activation(self.net_input(xi))
    error = (target - output)
    self.w_[1:] += self.eta * xi.dot(error)
    self.w_[0] += self.eta * error
    cost = 0.5 * error**2
    return cost
  
  def net_input(self, X):
    """Calculate net input; regular dot product formula w^{T}x. But in this case there's no column vector 
    of ones for X so we aren't directly dot producting, but have to do it separately. It's equivalent mathematically though."""
    return np.dot(X, self.w_[1:]) + self.w_[0]

  def activation(self, ,X):
    """Compute linear activation; this is the linear activation function. It's an 
    identity function, meaning it doesn't do anything right now."""
    return X
  
  def predict(self, X):
    """Return a class label after our unit step/linear activation function."""
    return np.where(self.activation(self.net_input(X)) >= 0.0, 1, -1)
  
'''
Average cost goes down pretty quickly still, after 15 epochs, and the result 
is similar to the batch gradient descent Adaline algorithm that we made. Then if you
wanted to update your model, in an online learning situation, you'd just call partial_fit.

page: 100

'''