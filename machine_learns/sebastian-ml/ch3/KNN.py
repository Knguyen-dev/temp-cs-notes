"""
##

## Credits: 
- [KNN from scratch](https://youtu.be/rTEtEy5o3X0?si=O8PLFrhdmvQz9DC9)
"""
import numpy as np
from collections import Counter

def euclidean_distance(x1, x2):
  return np.sqrt(np.sum((x1-x2) ** 2))

class KNN:
  def __init__(self, k=3):
    self.k = k

  def fit(self, X, y):
    self.X_train = X
    self.y_train = y

  def predict(self, X):
    predictions = [self._predict(x) for x in X]
    return predictions
  
  def _predict(self, x):
    # Compute the distance for the current x to every other value in X_train
    distances = [euclidean_distance(x, x_train) for x_train in self.X_train]

    # Sort the distances array, now the distances should be in ascending order, then get the first k values
    # which also means "get the k closest neighbors"
    k_indices = np.argsort(distances)[:self.k]

    k_nearest_labels = [self.y_train[i] for i in k_indices]

    # Majority vote

    most_common = Counter(k_nearest_labels).most_common()
    return most_common[0][0]
  