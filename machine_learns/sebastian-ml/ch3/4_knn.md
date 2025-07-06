# K-nearest neighbors (lazy learning algorithm)

### What is it? Why lazy?
The typical example of a **lazy learner**. It's lazy because it doesn't learn some discriminative function form the training data, but it memorizes the training dataset.

ML algos are either parametric or nonparametric:
- **Parametric models:** Estimates parameters (weights) from the training data, and ultimately learns a "function" that can classify new data points. Examples are perceptrons, logistic regression, and linear SVM. The number of parameters (features in this case) are fixed, the number doesn't grow, but only the values of the parameters change.
- **Nonparametric models:** Can't be described using a **fixed set** of parameters, but rather the number of parameters grow with the training data. Examples are decision trees and kernel SVM. For decision trees, this makes sense as the number of actual features you use will likely depend on how many times you're allowed to split the child nodes, it's not guaranteed to be something fixed. Also random forests will randomly assign sets of features.

More specifically, it's a subcategory of nonparametric models called **instance-based learning**. These types of models memorize the training dataset, and **lazy learning** is generally linked with no cost during the learning process.

---
### How does it work?
1. Choose the number of $k$ and a distance metric (e.g. Euclidean distance).
2. Find the k-nearest neighbors of the sample that we want to classify. Given a datapoint, find the other data points closest to it.
3. Assign the class of the datapoint via majority vote. Say k=5, if the 5 closest data points are majorly class "A", then we assign that new datapoint as class A. 

---
### How it works conceptually
We're splitting the graph into cells, drawing boundaries. There are different types of boundaries. Like we have 


---
### Review
- **No training phase:** It's not learning the data. It's storing the entire dataset and waits until a new sample needs a classification
- **Time Complexity:** To classify a new sample, it compares it to all stored samples. The worst case is O(n), where n is the number of training samples. Though you can use special trees to reduce this to log(n).
- **k:** The crucial hyperparameter to find a balance between over-fitting and underfitting
- **Euclidean distance:** The typical distance measure.

### Curse of dimensionality (Preview):
As the number of features (dimensions) increases, the data points become more spread out in higher dimensional space. This is a problem for many ML models, as it causes overfitting. When we can't use regularization, select only useful features and use dimensionality reduction techniques.