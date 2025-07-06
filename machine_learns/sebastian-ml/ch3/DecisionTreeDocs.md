# Decision Tree Example 

### How it works again?
Lets say you have a dataset of houses and we want to create a model that predicts whether 
or not that house is affordable or not.

1. Split by neighborhood: All houses in the east neighborhood go to the left child whilst 
houses in the west neighborhood go to the right child. At least neighborhood is a categorical variable, so it 
makes it easy on us. Now let's say the child containing the west neighbor is a pure leaf node. That's great, we actually don't need 
to split on that node anymore, so leave it be. Now assume the child containing east houses isn't pure, so there are some nodes with "yes" for is_affordable and 
some with no.

2. Split by # of rooms: So since this is a numerical column, we can't really split on every number. Like the number of rooms could be in the range [1, 7] and having 
  a branching factor of 7 is not going to work. Use some condition like "# rooms > 5", so all the houses with the number of rooms greater than 5 will go to 
  one child, and the houses that don't meet that condition go to another child. At this point our tree should have 2 levels. Now let's say that luckily these both 
  become pure leaf nodes, and now we can stop our recursive tree building process

#### What needs to be decided?
It's not that easy:

- Split feature: How do we decide whether to split on neighborhood or # of rooms first? One could be "better" than the other.
- Split point: For numerical columns, how do we pick the exact threshold to say "everything greater than this point goes to the left child and all else goes to the right child"?
- When to stop splitting: For a larger dataset, trying to build a tree to make everything a leaf node just is going to be computationally expensive, and overall not a possible or 
  really an accurate way of doing things. So when do we decide to stop splitting?

#### Rough Algorithm for training a decision tree
- Calculate the information gain with each possible split.
- Divide the set of data by the feature and value that yields the highest IG.
- Divide the tree and do the same for all created branches
- Keep doing this process until a stopping criteria is reached.

#### Rough algo for testing and predicting 
- Follow the tree until you reach a leaf node
  - If leaf node is pure, that's easy return the class label of the leaf node
  - If the leaf node isn't pure, return the class label that the majority of the data points have
    for that leaf node.

### The math
- Information gain: IG = E(parent) - [weighted average] * E(children)
- Entropy = E(X) = Sum (p(X) * log_{2}(p(X))), where p(X) = #x / n 
- Stopping Criteria:
  - Max depth: The decision tree can't grow past a maximum depth
  - Min number of samples: If there are less than m samples within the node, then we'll stop any further splitting.
  - Min impurity decrease: If the amount of impurity of change/reduction is less than some threshold, then we can stop it early
    with the reasoning being "hey we're not really getting any more accurate".

  - Num features: Defines the number of the features you're allowed to use before you stop. It's more of a way 
  to add randomness to the features that are used. This is more useful when you build a random forest.




### Algorithmic Flow 
Given a node of data points X and labels y, and a depth.
- Base cases:
  - If max depth is reached
  - If the current node is a pure leaf
  - If the current node has less than the minimum threshold
  - In all of these cases return the most common label in the leaf node, and return the leaf node to be in our decision tree.
- Calculate the best feature to split on and the threshold or value we need to split on. This involves iterating over all features that haven't been picked yet and their possible thresholds, calculating the information gain.
- Now we have the best feature and threshold, we will split the data into child nodes. Then we'll recursively do this process again.
- After recursion is done, we would have a node value to connect from, so use those and return a node up.


## Credits:
- [How to implement Decision Trees from scratch with Python](https://youtu.be/NxEHSAfFlK8?si=-80NWT1cu0m2g1DU)
- [How to implement Random Forest from scratch with Python](https://youtu.be/kFwe2ZZU7yw?si=iJ_n7tKyhXoNg1ea)
