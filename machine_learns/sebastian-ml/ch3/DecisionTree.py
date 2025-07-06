
import numpy as np

class Node:
  def __init__(self, feature=None, threshold=None, left=None, right=None, *, value=None):
    """Constructor for node

    Args:
        feature (_type_, optional): The feature that this node was divided with; so the parent decided a feature to divide with and it resulted in nodes, this is one of those nodes. 
        threshold (_type_, optional): Numerical threshold that it was divided with which is defined when the feature is numerical
        left (_type_, optional): Left subtree that we are pointing to
        right (_type_, optional): Right subtree that we are pointing to
    NOTE: We use * to indicate all parameters after it must be specified as kwargs.
    """
    self.feature = feature 
    self.threshold = threshold 
    self.left = left 
    self.right = right 

    # The value (class value) of the tree, which is only defined if this is a leaf node. 
    self.value = None
  
  def is_leaf_node(self):
    """Returns whether a node is a leaf node.
    Returns:
        boolean: Whether the current node is a leaf node
    """
    # If the value isn't none, then that means we don't a class label. If we don't have 
    # a class label, then it's true that it isn't a leaf node yet.
    return self.value is not None

class DecisionTree: 
  def __init__(self, min_samples_split=2, max_depth=100, n_features=None):
    """_summary_

    Args:
        min_samples_split (int): If a node's number of data points is below this number, then we'll early stop the recursion.
        max_depth (int): If the level reaches or goes past this level, we'll stop
        n_features (int, optional): Number of feature columns in the dataset. This can be left optional, and then assigned using `fit()` method.
    """
    self.min_samples_split = min_samples_split 
    self.max_depth = max_depth 
    self.n_features=n_features
    self.root = None
  
  def fit(self, X, y):
    """Builds a decision tree based on the data given.
    Args:
        X: The data matrix.
        y: list containing the class labels
    """
    # If number of features wasn't already defined, decide it now.
    # Then call the helper function to recursive grow the decision tree.
    self.n_features = X.shape[1] if not self.n_features else min(X.shape[1], self.n_features)
    self.root = self._grow_tree(X,y)
  
  def _grow_tree(self, X, y, depth=0):
    """Helper function that recursively grows the decision tree. 

    Args:
        X: The data matrix that the current node has to process and split on
        y: list containing the class labels
        depth: Level that the current node is at in the decision tree.
    """

    # number of samples, number of features, and number of unique class labels left.
    n_samples, n_feats = X.shape
    n_labels = len(np.unique(y))

    '''
    Base Cases:
      1. If we reached maximum depth, early return.
      2. If all data points have the same class label, it's a pure leaf node so return.
      3. If the number of data points in our node is less than the minimum, early return to reduce overfitting.
    In this case we're returning and bubbling up a node that has the class label.
    '''
    if (depth >= self.max_depth or n_labels==1 or n_samples < self.min_samples_split):
      leaf_value = self._most_common_label(y)
      return Node(leaf_value)
    
    '''
    Recursive case, find the best attribute (and possibly numerical threshold) to split the data by:
    1. feature_indices: Is a list of indices corresponding to the features that will be considered for splitting the data.
      - Selects `self.n_features` unique indices from the range [0, n_feats-1] .
      - replace=False ensures an index isn't chosen more than once
      - Just know that this is commonly used in ensemble learning methods like Rando mFOrests to select a random subset of 
      features for training a decision tree.
    2. Find out the best feature to split on, and if possible the best threshold.

    NOTE: Isn't it weird that we try to choose n_features (the maximum amount) even though
    the number of features in n_feats is probably a lot less? Or does it just work out?
    '''
    feature_indices = np.random.choice(n_feats, self.n_features, replace=False)
    best_feature, best_thresh  = self._best_split(X, y, feature_indices)

    '''
    Step 3: 
    1. Split the data by the column indicated by the best_feature index, using that threshold.
    2. Then recursively create your left and right subtrees.

    3. After this is done, we should have gotten a node s owe're propagating up a leaf node.
    '''
    left_indices, right_indices = self._split(X[:, best_feature], best_thresh)
    left = self._grow_tree(X[left_indices, :], y[left_indices], depth+1)
    right = self._grow_tree(X[right_indices, :], y[right_indices], depth+1)
    return Node(best_feature, best_thresh, left, right)

  def _best_split(self, X, y, feature_indices):
    """Find the best split out of all attributes, and all possible thresholds."""
    best_gain = -1
    split_index, split_threshold = None, None
    '''
    Iterate over the feature_indices (each feature):
      - Now we're splitting on a specific feature associated index.
      1. Get all rows, but just select the feature column for those rows. So we just got the feature column itself.
      2. Get all unique values for this feature, let's call them thresholds (could be numerical or categorical)
      - Iterate through each attribute value:
        1. Calculate the information gain by splitting on this specific attribute value.
        2. If the higher gain by doing this is higher than our current maximum, then update our values.

    At this point we should have calculated the information gain for every attribute, and also every 
    value that the attribute could have taken. Return the best feature index and threshold/value for that feature to split on.
    '''
    for index in feature_indices:
      X_column = X[:, index]
      thresholds = np.unique(X_column)
      for t in thresholds:
        gain = self._information_gain(X_column, y, t)
        if gain > best_gain:
          best_gain = gain
          split_index = index
          split_threshold = t
    return split_index, split_threshold

  def _information_gain(self, X_column, y, t):
    """Calculates the information gain caused from splitting 

    Args:
        X_column: An np.array or just a list containing the row values for a single feature column.
        y (_type_): The list of class labels.
        t (_type_): The attribute value that we're splitting the data on. 

    Returns:
        _type_: _description_
    """
    '''
    # Split the parent node's data points by a specific column and threshold, to create the sub-children.
    So now we have two different np arrays containing items in the left and right nodes.

    Notice that we return 0 information gain when one of the lists are empty, which implies that everything 
    is in the other list. Intuitively, this makes sense. Imagine having your parent node of 100 rows, and then 
    splitting, only to get an empty child and the other still with that 100 nodes. We didn't filter anything down, we 
    didn't learn anything new from the split, and so we say that's zero information gained.

    Also note these are arrays of indices, row indices to be exact.
    '''
    parent_entropy = self._entropy(y)
    left_indices, right_indices = self._split(X_column, t)
    if len(left_indices) == 0 or len(right_indices == 0):
      return 0

    '''
    # Calculate the weighted average entropy of children; remember weighted average means "how many samples in one and how many in other"
    1. The len(y) is the total number of data points in the parent (number of data points in total)
    2. Calculate the number of data points in the left and right nodes respectively
    3. Calculate the entropy of the left and right children respectively, using their respective partitions from the list of class labels.
      Then multiply their entropies by their respective proportions to get the "weighted average". Sum those terms up.
    4. Finally we return information gain = parent_entropy - child_entropy, where child_entropy is the weighted average entropy of the children.
    '''
    n = len(y)
    n_l, n_r = len(left_indices), len(right_indices)
    e_l, e_r = self._entropy(y[left_indices]), self._entropy(y[right_indices])
    child_entropy = (n_l/n)*e_l + (n_r/n)*e_r
    return parent_entropy - child_entropy
    
  def _split(self, X_column, split_thresh):
    """Split a list of data based on a threshold. Return lists that contain the indices of which data points should be where."""
    left_indices = np.argwhere(X_column <= split_thresh).flatten()
    right_indices = np.argwhere(X_column > split_thresh).flatten()
    return left_indices, right_indices

  def _entropy(self, y):
    """Calculates the entropy for a given list of classifications"""
    '''
    1. Use bin counts:
    input=[1,2,3,1,2]
    hist=[0,2,2,1]; 
      - basically index = 0, the number 0 occurred 0 times in the input.
      - Index = 1, the number 1 occurred twice in the input
      - Index = 2, the number 2 occurred twice.
      - Index = 3, the number 3 occurred once, and so on and so forth. 
    
    2. Then we basically calculated the p_{i} for every class label
    3. Then calculate the entropy using the formula
    '''
    hist = np.bincount(y)
    probability_list = hist / len(y)
    return -np.sum([p * np.log(p) for p in probability_list if p > 0])

  def _most_common_label(self, y):
    """Returns the element from y that occurs the most"""
    # Create a dictionary with form {label: label_count}
    label_dict = {}
    for label in y:
      if y in label_dict:
        label_dict[y] += 1
      else:
        label_dict[y] = 1
    
    max_label = None
    max_label_count = 0
    for label in label_dict:
      # for the first iteration just assign value
      if max_label is None:
        max_label = label
        max_label_count = label_dict[label]
        continue

      # if we found a new maximum, update our variables
      if label_dict[label] > max_label_count:
        max_label = label
        max_label_count = label_dict[label]
    
    return max_label

  def predict(self, X):
    """Given a data matrix of testing data, the decision tree model will make predictions on that data.
    Args:
        X: Data matrix

    Returns:
        Array: An array of predictions on the class labels
    """
    # For every row/observation in the data matrix X, traverse the tree, put that row of data 
    # in the decision, let it go down it and we'll be returned the predicted class label.
    return np.array([self._traverse_tree(x, self.root) for x in X])

  def _traverse_tree(self, x, node):
    """Traverses the decision tree. This assumes the decision tree already exists though.

    Args:
        x (_type_): A row of data (a single observation).
        node (_type_): Current node we're at.

    Returns:
        Even though it's recursive, it's going to return a node value. 
    """
    '''
    Base case:
      - If it's a leaf node, we're going to return the class label for that node
      - If the value of the feature we're splitting on, is lower than our threshold that we decided for best information gain, then
        make this data point travel to the left child. 
      - Else travel to the right child.
    '''
    if node.is_leaf_node():
      return node.value()
    if x[node.feature] <= node.threshold:
      return self._traverse_tree(x, node.left)
    return self._traverse_tree(x, node.right)