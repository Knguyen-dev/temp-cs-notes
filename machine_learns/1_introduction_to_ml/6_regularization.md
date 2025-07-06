# Regularization

In general, **regularization** is a process that converts an answer to a problem, into a simpler answer. For our sakes, it's the idea of adding constraints to a model to prevent overfitting. Think of it as putting "guardrails" on your model to prevent it from memorizing the training set.

## Intuition: Degrees of freedom

Consider a linear model: 

$\hat{y} = \theta_{0} + \theta_{1}x$

This model has **2 degrees of freedom**. It can adjust both $\theta_{0}$ (intercept) and $\theta_{1}$ (slope) to fit the data. If we fix $\theta_{0} = 0$, we now only have **1 degree of freedom**, making the model simpler, but potentially too restrictive.

Regularization creates a middle ground, allowing the model to have 2 parameters but penalizes large parameter values. This effectively gives us soemthing between 1 and 2 degrees of freedom. It's more flexible than a single parameter, but more constrained than 2. Let's talk about why this even works:
- Large parameter values often indicate overfitting, as the model is working too hard to fit every training example.
- Small parameter values tend to create smoother more generalizable models.
- I guess you can think of it as penalizing complexity and forcing the model to find simpler patterns that are more likely to generalize.


TODOS:
- Math of regularization
- Standard vs Regularized Loss functions 
- Types of Regularization
- How hyperparameters control regularization strength


## Mathematics of Regularization

I haven't gone through a lot of math yet in my notes so I'll go through it now. 




### Standard vs Regularized Loss Function
Without regularization, our loss function is:



- This is just the mean-squared error. We want to minitmize this 
-


### Types of Regularization
- explain l1, l2, and elastic net 
## How Hyperparameters Control Regularization Strength




There are two types of regularization:  explicit and implicit. We'll be focusing on the latter as it's ubiquitous in ML. In ML, a key challenge we face is making models generalize to unseen data, data that it hasn't seen before in training. It's crucial for addressing overfitting as the goal is that the model learns the broader patterns within the data rather than memorizing it. Let's look into some techniques.

## Early Stopping
The idea of stopping trainnig when the validation performance deteriorates. This prevents overfitting by halting the model before memorizing training data. We hinted at this in our section about the validation set.

## L1 and L2 Regularization
We would add penalty terms to the cost function:
- **L1 regularization (LASSO):** It adds a panelty based on the absolute value of the coefficient of the weights.
- **L2 regularization (Ridge Regularization):**

## Dropout


## Credits

