# Adaptive Linear Neurons (Adaline)

### What is it and how is it different?
Considered an improvement on the perceptron algorithm. It talks about defining and minimizing continuous cost functions, which are foundational ideas in later more advanced ML algorithms.

The main difference is that the weights are updated based on a **linear activation function** rather than a **unit step function**. This is defined as $\phi(w^{T}x) = w^{T}x$, so rather than outputting 1 or -1, it outputs the dot product unchanged. The linear activation function is used for updating the weights. We'll still use a threshold function to classify something and make predictions.

---
### Basic steps:
1. Use net input function to calculate the net input for a given sample.
2. Use activation function, which updates the weights. It compares the true class labels with the continuous output to calculate the error and update the weights based on that.
3. Then use threshold function to output the predicted classification our model thinks. 


## Using gradient descent to minimize cost
The **objective function** is a function we want to optimize in supervised machine learning. In many situation, this will be a **cost function**, a function that measures how incorrect our ML model is, and we want to learn the weights and parameters S.T. this function outputs the smallest value. In english, so that it is the least wrong. Let's define a classic cost function, a sum of squared errors:

$J(w) = \frac{1}{2}\sum_{i}{y^{i}-\phi(z^{i})}$
- $\frac{1}{2}$: Makes it easier to derive gradient.
- $\phi(z^{i})$: This is our net input we get after calling our **linear activation function** on $z^{i}$. Our net input is just $w^{T}x = w_{0} + w_{1}x_{1} + ... + w_{m}x_{m} = \hat{y}^{i}$. Here it's just your predicted value, with the intuition being a linear combination of your inputs and some weights has to predict the output value.

Since it's a continuous activation function, we can derive it. It's a convex cost function, so we know there's a local minimum $J_{min}(w)$, making it easy to use gradient descent to find that local minimum, or more specifically the weights at $J_{min}(w)$. Again the gradient of a function points in the direction where the derivative increases, so we move in the opposite direction:
- $w := w + \delta w$
- $\delta w = -n \nabla J(w)$

You compute the gradient of the cost function by doing the partial derivative W.R.T some weight:

$\frac{\delta J}{\delta w_{j}} = -\sum_{i}{y^{i} - \phi(z^{i})}x_{j}^{i}$

$\delta w_{j} = -n\frac{\delta J}{\delta w_{j}} = n\sum_{i}{(y^{i} - \phi(z^{i}))x_{j}^{i}}$

- **NOTE:** You may ask, why are we differentiating with respect to weight? Why not with respect to like x or something? Well our goal is to find the weights S.T. J's value is minimized. The best way to do this is to analyze how some $w_{j}$ affects J's value.
- **NOTE 2:** The update on weights is calculated based on all samples here, as opposed to updating the weight after each sample like we did in the Perceptron algorithm. This is also why we call this **batch gradient descent**.

---
### Hyper-parameters: An introduction
Our hyper-parameters are parameters that we fix to the ML model rather than them being constantly changed due to learning. So for the Perceptron and Adaline algorithms, the hyper-parameters are the learning rate and number of iterations. And of course there are various different techniques we can use to find the best values.

If you make the learning rate really small $n=0.0001$, the idea is that you'll need a lot more epochs/iterations for your ML model to converge on a certain answer/performance.

---
### Improving gradient descent with feature scaling
Gradient descent benefits from **feature scaling**, in particular **standardization**. This is the idea of giving the idea the property of a standard normal dist, allowing the gradient to converge more quickly. All that's explained is that we have to go through fewer steps to find the minimum of the cost function.

So by doing this, each feature has a mean of 0, and a standard deviation of 1. Here's the formula:

$x_{j}' = \frac{x_{j} - \mu_{j}}{\sigma_{j}}$

- $x_{j}$: A vector containing the jth feature value for all training samples.  

--- 
### Large-scale ML and stochastic gradient descent

#### What is it?
Assume we have a very large dataset with millions of data points. Doing batch gradient descent will be computationally expensive, so it may not work well since you probably don't want to run a million data points each iteration. 

So instead of updating weights based on the sum of the error values over all samples, update the weights incrementally for each training sample. Aka "approximation of gradient descent".  
- **Random:** Make sure training data is presented in a random order, and we shuffle the data for every epoch to prevent **cycles**.
- **Cycles:** Refers to repetitive patterns relating to the ordering of the data, which interfere with optimization. If data is presented in the same order for every iteration, the model may just patterns or correlations inherent to that ordering. In english, you can think of this as the computer thinking "Oh hey I can just classify the images based on order. Images 1 to 5 are always cats. Then images 6-10 are always dogs.". A simplified way of thinking about it, but it causes a case where the model oscillates in its performance, rather than converging at a minimum cost.
- **Shuffling:** Data is displayed in a random order, which helps prevent teh data from learning in a repetitive way, leading to better generalization and faster convergence.
- **learning rate (n):** Rather than be fixed, this is now adaptive and decreases over time. Computed with $n = \frac{c_{1}}{[N] + c_{2}}$. Where $c_{1},c_{2} \in \mathbb{R}$ and $N$ is the number of iterations.

- **Updating weights:** So $\delta w = n\sum_{i}{y^{i} - \phi(z^{i})}x^{i} = n{y^{i} - \phi(z^{i})}x^{i}$ 

- **Online Learning:** Stochastic gradient descent is good for online learning. This is the idea where your model is learning is on the fly as new data is coming in. Such as a model learning customer data in a web application.

---
#### Limitation with stochastic gradient descent
It should be noted it doesn't reach the global minimum, but it gets very close. Then the usage of an adaptive learning rate allows you to polish things.

---
#### Mini Batch Learning 
A compromise between batch gradient descent and stochastic gradient descent. You do batch gradient descent to smaller subsets of training data, e.g. 32 samples at a time. As a result, weights are updating quicker, converging quicker.