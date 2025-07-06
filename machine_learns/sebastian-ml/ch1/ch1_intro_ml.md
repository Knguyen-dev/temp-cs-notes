# Chapter 1: Teaching Computers how to learn

---
## Types of Machine Learning:

### Supervised learning

#### 1. Classification
The goal is to assign categorical, unordered labels to new data. Our model takes in data, learns, then makes a prediction on the label of new data, but the labels are discrete and finite. This example is a binary classification problem since there are only two categories. E.g. whether an email should be marked as spam or not. There's also **multiclass classification** where the model is going to handle learning and outputting multiple labels. For example, hand written character recognition. The algorithm will be able to recognize characters it's seen in the training data, but won't be able to handle digits that weren't in the training data. 

Essentially you feed labeled training data into an ML algorithm. This creates the model that learned from that specific data. Then you input data into that model so that it can predict things. 

- **Decision boundary:** Imagine the data for cats and dogs is on a scatter plot. The decision boundary is the rule, or line that the ML model learns that separates/distinguishes the data.

---
#### 2. Regression:
This is the prediction of continuous data. We're given input/predictor/explanatory variables (features), and we continuous response/target variable. 

For example, assume we're interested in predicting the math SAT scores for our students. If there's a relationship between time studying for the student, and the final scores, we could use existing data to train a model that uses study time to predict teh test scores of future students.

Given the x and y variables, what is the equation of the line that best fits the data. More specifically, what is the line that minimizes the mean squared error (how much we're off by) between the sample points and the fitted line. With this new equation, specifically the parameters of the eq. (slope and intercept) we can use this to confidently predict the outcome variable of new data.s


---
### Unsupervised learning
Here we're dealing with data that's unknown. We don't know its structure, any relationships, it doesn't have any labels, and so we need help figuring those out. The idea is to discover patterns in the input data without any labels (no feedback, so it doesn’t know whether it’s right or wrong). The two tasks here are clustering, and compression.

#### 1. Clustering
An EDA technique that allows us to organize data into subgroups/clusters without having prior knowledge of their relationships. So like data cluster share a similarity, but are more dissimilar to objects in other clusters. For example, imagine a scatter plot, and we separate/highlight the data points with certain colors to indicate what cluster they're in.


#### 2. Compression (Dimensionality Reduction)
Sometimes, we feel that the original data is far too complex, with too many aspects/columns/dimensions. This causes issues when storing data, and then the computational performance of some ML algorithms with complex data, etc. So we use dimensionality reduction as a technique in data cleaning/pre-processing to remove noise and unnecessary parts of the data. This results in more simple data that keeps the most relevant stuff for our task.


---
### Reinforcement Learning: 


Create a system (agent) that improves its performance based on interactions with the "environment". In this environment we have a **reward signal**. Here the feedback isn't the universal truth, but we're just seeing how well the action was measured. 

Anyways the agent learns a series of actions that maximizes the reward via trial and error or just planning. An example is a chess engine, where the agent decides on a series of moves depending on the state of the board (environment). The reward is defined as **win** or **lose** when the game ends.

The outcome of each move can be though of as a different state in the environment. If it was a good move, that's a positive reward, else give the agent a negative reward to teach it that moves like that are bad. So removing an opponent's piece is good and threatening the queen is good. Losing a piece is bad.

Anyways reinforcement learning is beyond the scope of the notes, as we're just going to be focusing on the former 2 types of ML.


## Basic terminology:
- features: Input columns, the predictor columns in the data.
- samples: Instances, observations, rows of data.
- class label: The column, or target variable being predicted.
- Data matrix $X$ and vectors: A matrix $X \in R^{150 \times 4}$ means we have 150 rows, with each row having 4 feature columns. Matrices are upper case, vectors are lowercase $x$. So $x^{i}$ talks about the ith row vector in the matrix, or ith row in the data. Then $x^{25}_{1}$ is talking about the first column value in the 25th row of data. In an example where we're analyzing flowers, this contains sepal length and width, petal length and width.
- Target variable $y$: This is a column vector that contains all your predicted values. 

---
### Building an ML system workflow

It's about pre-processing, learning, evaluation, and prediction

#### 1. Pre-processing 
1. Keep useful features such as the flower's color, height, length and width. 
2. We should have the features be on the same scale from [0, 1] which prevents some features 
from completely dominating others due to them being numerically larger (normalization). 
3. Some data is highly correlated and redundant. We use dimensionality reduction to compress and skim the fat.
4. Finally we'll need to ensure the ML model not only performs well on the training dataset, but also generalizes well to new data. The idea is to randomly divide the dataset into a training set and testing set. Use the training set to train and optimize the model, and then at the very end, use the testing set to evaluate the final model.

---
#### Training and selecting a predictive model
No one model is superior to all others. There are many different tools with different biases, so it's a good idea to know the theory and also select a couple of different models and compare their results. First we need to decide on a measure of performance, and a common one is classification accuracy, the proportion of data S.T. the model predicted the target variable correctly.

"How can we be sure which model performs well on the final test data and real world data, if we aren't using that data when training or selecting models, but just keeping it at the end." - Different cross-validation techniques are used to divide the training data into more training and validation (small evaluations) data set to best estimate the generalization performance of a model.

Also don't expect the default parameters provided by libraries to be optimal for our specific task or project. We'll make use of "hyperparameter optimization techniques". Again **hyperparameters** are parameters that aren't learned from the data, but fixed numbers and things that influence our model, and we can change and tune them in hopes of improving performance. Just worry about this later. 

---
#### Testing and predicting new data
After we're done, we use the test data to see how well our "fitted" (trained) model performs on new data. 