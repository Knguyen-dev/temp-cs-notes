# Modeling class probabilities with logistic regression

### What is logistic regression? Versus Perceptron?
Unfortunately, the Perceptron algo needs the classes to be perfectly linearly separable to converge. Logistic regression, is a model for classification (not regression), and it's good for linear and binary classification. You can modify it for multi-class classification using the OvR technique. 

---
### Odds, log-odds, logit
The **odds ratio** is written as $\frac{p}{(1-p)}$, where p is the probability of success/our target event happening. Let a positive class label be $y=1$. Then the **logit (log-odds)** is just the log of the odds ratio: $logit(p$ = log(\frac{p}{1-p}) .

$log(p(y=1 | x)) = w_{0}x_{0} + ... + w_{m}x_{m} = \sum_{i=0}^{m}{w_{i}x_{i}} = w^{T}x$

Logit takes input from 0 to 1 and maps it to a real number. With this we can express a linear relationship between feature values and the log-odds. 

$p(y=1|x)$: Probability that a particular sample is of class 1 given a vector of its x features. But we're 

- **NOTE:** Log here is the natural logarithm.

---
### Sigmoid function
We want the probability that a sample belongs to a belongs to a particular class, which is the inverse of our logit function. Okay so this makes sense because the logit function takes a probability and outputs a real number, a net input. It'd make more sense to get the inverse function, which takes the net-input that and then outputs a probability. Hopefully that makes sense. This yields the **sigmoid function ($\phi(z)$)**:

$\phi(z) = \frac{1}{1+e^{-z}}$
- $z=w^{T}x=w_{0}x_{0} +...+w_{m}x_{m}$
- $x_{0}w_{0}$: Remember that $w_{0}$ is the bias unit and $x_{0}$ is just 1. This makes our notation more compact as we don't have to add the vector separately.
- **Activation function:** This is the activation function, the Sigmoid activation function, as opposed to linear. 
- **$\phi(z)$:** Note that $\phi(z) = P(y=1 | x;w)$. This is the probability that a sample belongs to class 1, given the features, with the weights as parameters. If $\phi(z) = 0.8$, it means that the chance that this sample/row is "Iris-versicolor" flower is 80%. Then you can calculate $P(y=0|x;w) = 1 - P(y=1|x;w) = 0.2$, assuming there are only 2 outcomes for now. While the threshold gives the end all be all output, applications like weather forecasting or knowing the chance that a patient has a certain disease are still very important.

- **Threshold function:** The predicted probability from our activation function, $\phi(z)$, is converted to a binary outcome. If $\phi(z) \geq 0.5$, then outcome is 1 (we predict it's a versicolor flower), else 0. We reason that when $z \geq 0 \implies \phi(z) \geq 0.5$. So another variation of this is if $z \geq 0$, then we output 1, else 0.

As $x \implies \infty, \phi \implies 1$. Then $x \implies \neg \infty, \phi \implies \neg 1$. So we're going to take real numbers and map them to a value in the range $[0,1]$. Then the y-intercept is at $\phi = 0.5$.

---
### Weights of the logistic cost function
For Adaline, our cost function was the sum of squared error. We'd take the partial derivative at every weight, this composes the gradient, and then we minimize each derivative. The cost function for logistic regression is:

- **Likelihood function:** $L(w) = P(y|x;w) = \prod_{i=1}^{n}{P(y^{i} | x^{i};w)} = \prod_{i=1}^{n}{(\phi(z^{i}))^{y^{i}}(1-\phi(z^{i}))^{1-y^{i}}}$
- **Log-likelihood function:** $l(w) = log L(w) = \sum_{i=1}^{n}{\begin{bmatrix}y^{i}log(\phi(z^{i})) + (1-y^{i})log(1-\phi(z^{i}))\end{bmatrix}}$

This is the likelihood function. In practice, it's easier to maximize the natural log of this equation, which is called the log-likelihood function. Doing log just reduces the potential for numerical underflow. Then express $l(w)$ as a cost function that we can minimize with gradient descent.

- **Cost Function:** $J(w) = \sum_{i=1}^{n}\begin{bmatrix}-y^{i}log(\phi(z^{i}))-(1-y^{i})log(1-\phi(z^{i}))\end{bmatrix}$ 
- **Cost for a single sample:** $J(\phi(z),y;w) = -ylog(\phi(z))-(1-y)log(1-\phi(z))$
    
If $y=1$, that second term goes to zero. Else, $y=0$, the first term goes to zero. The main takeaway seems to be the idea that we penalize wrong predictions with increasingly large costs.

#### Gradient Descent Learning Algo for logistic regression
On page 124, they derive the gradient descent learning rule for logistic regression. And they show how the weight update in logistic regression via gradient descent, is equal to the equation used in our Adaline algorithm.

---
### Training a logistic regression model with Sci-kit learn
Sci-kit learn supports multi-class settings off the shelf, so OvR is built-in. You can see an example of them using it on page 127. The main takeaway is that we have multiple decision regions, which a given point being in a given region meaning that we predict it's class output.

The `C` parameter is new, and it's related to future topics about over-fitting and regularization.

Before that, you should be able to call a method like: 

```python 
predict_proba(X_test_std[:3, :])
```
Where the first row corresponds to the class membership probabilities that the first data point has. So all values in a row add up to one, and the probability $\geq 0.5$ would indicate that the given data row belongs to that certain class column. While that's a sci-kit learn specific method, the idea of how multi-class logistic regression would work remains the same. They're basically calculating the probabilities that a given row of data will belong in each class outcome, via a more complex sigmoid activation function. And yes the sum of the probabilities for all possible class outcomes for a given row will equate to 1.

### Lookahead: Stratifying 
Ensures the proportion of target classes in the training and testing set are good. For example, assume our dataset has 3 output classes with 50 samples each. A random split may assign more of one output class to the training set and fewer to the testing set. This can lead to bias. By stratifying we're ensuring the class distribution in both the training and testing, mirror the class distribution in the original dataset. 

Assume dataset contains 150 samples, divided equally amongst 3 classes (0, 1, and 2), so each class has 50 samples. Then by stratifying, this 1/3 split is preserved in the training and testing set. The data is split into 70% training and 30% testing:
- training set: 105 samples (35 samples from each class)
- testing set: 45 samples (15 samples for each class)

The original proportion of samples has 0.33 across all 3 classes, so that will stay that way in our training and testing data.

#### Visual: Classification chart (p110)
Our chart is separated into 3 separate regions/classes by 3 different decision boundaries. All based on the petal length and petal width (both standardized). However notice how the data isn't perfectly linear separable. It's impossible to draw a single straight line (in 2D), plane (in 3D), or hyperplane. In this case it had to use 3 lines, rather than one straight one, meaning it isn't linearly separable.

### Lookahead: Regularization
- **Over-fitting:** Common ML problem where a model performs well on training data, but doesn't generalize well to other data. We can reason that the model suffers from **high variance**, which can be due to having too many input parameters. You can think of this as over-learning or becoming too used seeing specific patterns in training data.
- **Under-fitting:** When the model isn't complex (good) enough to predict training data well, and therefore isn't good for new data either.
- **Variance:** The measure of consistency (or variability) of a model's predictions for a particular sample instance (row). Here we'd train the model multiple times on different subsets of the training data, and we'd look at a particular row and ask "How similar or difference are the predictions that the model gave, for this sample instance?".  Measure of error due to randomness.
- **Bias:** Measures how far off the predictions are from correct values generally if we rebuild the model multiple times on different training datasets. The measure of error that isn't due to randomness.

Through regularization, we can achieve a good balance between variance and bias. We'll use reg. for dealing with collinear data (highly correlated features), filtering noise, and preventing over-fitting. The idea is to add additional info/bias to penalize extreme parameter/weight values. The most common form of reg. is L2 reg. (aka L2 shrinkage or "weight decay"). It's written as:

$\frac{\lambda}{2}|w|^{2} = \frac{\lambda}{2}\sum_{j=1}^{m}{w_{j}^{2}}$, where $\lambda$ is the **regularization parameter**.

- **Note (Feature Scaling):** Regularization only works if you've standardized the features.

You'll regularize the logistic cost function by adding a regularization term. This term will help shrink the weights when training the model:

$J(w) = \sum_{i=1}^{n}\begin{bmatrix}-y^{i}log(\phi(z^{i}))-(1-y^{i})log(1-\phi(z^{i}))\end{bmatrix} + \frac{\lambda}{2}||w||^{2}$ 

Using $\lambda$, we can control how well we fit the training data whilst keeping the weights small. The higher the value of $\lambda$, the higher the reg. strength. In the `LogisticRegression` class, $C$ is related to an idea in **Support Vector Machines**. $C$ the inverse of $\lambda$; they're inversely proportional.