import numpy as np
from sklearn import datasets
from sklearn.linear_model import LogisticRegression, Perceptron
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler


class LogisticRegressionGD(object):
  """Logistic Regression Classifier using gradient descent.
  Parameters
  ------------
  eta : float
    Learning rate (between 0.0 and 1.0)
  n_iter : int
    Passes over the training dataset.
  random_state : int
    Random number generator seed for random weight initialization.

  Attributes
  -----------
  w_ : 1d-array
    Weights after fitting.
  cost_ : list
    Sum-of-squares cost function value in each epoch.
  """
  def __init__(self, eta=0.05, n_iter=100, random_state=1):
    self.eta = eta
    self.n_iter = n_iter
    self.random_state = random_state
  
  def fit(self, X, y):
    """ Fit training data.
    Parameters
    ----------
    X : {array-like}, shape = [n_samples, n_features]
    Training vectors, where n_samples is the number of
    samples and n_features is the number of features. The data matrix
    y : array-like, shape = [n_samples], Vector of target values.
    
    Returns
    -------
    self : object
    """
    rgen = np.random.RandomState(self.random_state)
    self.w_ = rgen.normal(loc=0.0, scale=0.01, size=1+X.shape[1])
    self.cost_ = []

    for i in range(self.n_iter):
      net_input = self.net_input(X)
      output = self.activation(net_input)

      # Calculate the total actual - predicted error.
      errors = (y - output)

      # Updating the weights
      self.w_[1:] += self.eta * X.T.dot(errors)
      self.w_[0] += self.eta * errors.sum()

      # Note that we're computing the logistic cost instead of the sum of squared error cost.
      # We're following the formula from above, and calculating the logistic cos for the given iteration
      cost = (-y.dot(np.log(output))) - ((1-y).dot(np.log(1-output)))
      self.cost_.append(cost)
    return self

      
  def net_input(self, X):
    """Standard net input. Again conceptually the data grid's first column vector is a vector of 1s, which
    makes the dot product very compact w^{T}x, but the math we're doing here is equivalent."""
    return np.dot(X, self.w_[1:]) + self.w_[0]
  
  def activation(self, z):
    """Compute the probability using our sigmoid activation function"""
    return 1. / (1. + np.exp(-np.clip(z, -250, 250)))
  

  def predict(self, X):
    """Return class label after 'unit step'. basically  this does the math after
    the activation, using the probability to calculate the expected the class label."""
    return np.where(self.net_input(X) >= 0.0, 1, 0)

    # NOTE: Remember that we explained that this is equivalent to:
    # return np.where(self.activation(self.net_input(X)) >= 0.5, 1, 0)
  

iris = datasets.load_iris()
X = iris.data[:, [2,3]]
y = iris.target

# 1. Split data into training and testing
X_train, X_test, y_train, y_test = train_test_split(X,y, test_size = 0.3, random_state=1, stratify=y)

# 2. Logistic Regression primary works for binary tasks, and that's true for the model we made 
# Consider only the "Iris-setosa" and "Iris-versicolor" flowers (classes 0 and 1)
X_train_01_subset = X_train[(y_train == 0) | (y_train == 1)]
y_train_01_subset = y_train[(y_train == 0) | (y_train == 1)]
logReg = LogisticRegressionGD(eta=0.05, n_iter = 1000, random_state=1)
logReg.fit(X_train_01_subset, y_train_01_subset)

'''
The decision region plot is on p. 124, and it's clean. The data can be divided by a linear 
decision boundary.
'''

# Sci-kit-learn using logistic regression
lr = LogisticRegression(C=100.0, random_state=1)
lr.fit(X_train_std, y_train)