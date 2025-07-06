# Introduction to ML

## Types of ML Systems 
There are different types of machine learning systems:
- How are they "supervised"? (supervised, unsupervised, self-supervised, etc.)
- Whether it learns incrementally on the fly (online vs batch learning).
- Whether it works by comparing new data points to old ones or by detecting patterns in the data to build a predictive model (instance-based vs model-based learning).

## Supervised Learning
The idea is to give a training set of data to our algorithm that includes the solutions. This is called labeled data, so the algorithm makes its guess, compares it to the solution, and adjusts based on whether it got the answer correct.

A typical task is **classification**. E.g. whether an email should be marked as spam or not. Then when you test it, you give it a testing set (emails it hasn't seen before) to see if it can classify these new emails correctly.

Another typical task is to predict a target numeric value such as the price of a car, given a set of **features** such as (mileage, age, brand, etc.). This is called **regression**, and you train your system by giving it many examples of cars, features, and the target you're predicting (actual price of the car, the solution).

Some regression models can be used for classification and vice versa. ***Logistic regression*** is commonly used for classification.

## Unsupervised Learning
Here the training data is unlabeled, so the system learns without knowing the actual solutions. This is also because we don't know much about the data ourselves. We don't know its structure, any relationships and so we need help figuring those out. The idea is to discover patterns in the input data.

A typical task is called **clustering**. For example, let's say you have some data about your customers. We may run a ***clustering algorithm*** to try to detect similar groups of visitors, to put similar visitors into clusters. The algorithm may notice that 40% of visitors are teenagers who love comic books, whilst 20% are adults that like scifi. If you use a **hierarchical clustering** algorithm, you groups may be subdivided into smaller groups. You'll also see clustering in visualization algorithms, which will show data points and then those data points separated by various decision boundaries or just in separate clusters. 

A related task is **dimensionality reduction**, which is the idea of simplifying your data without losing too much valuable information. One approach is merging several correlated (statistically correlated) features into one. If a car's mileage is strongly correlated with its age, your algoirthm will merge them into one feature that represent the car's wear and tear.

This idea of transforming the features of the data, in hopes of creating useful features for ML inference is also called **feature extraction**.

Another important task is **anomaly detection**. An exmaple is detecting an unusual credit card transaction to prevent fraud, detect manufacturing defects, or just to detect outlires in your data.

## Semi and self-supervised learning 

**Self-supervised learning** is the idea of generating fully labeled data from unlabeled dataset. For example, we can have a large dataset of unlabeled images, mask a small part of each image and train a model to recover the original. The input is the masked image and the output is the original image, which are used as the labels/solutions.

The resulting model can be useful, you can repair damaged images, or erase unwanted objects from pictures. However the main goal is typically tweaking this model to do another useful task. For example, if your model knows can take a masked picture, and knows whether to put a cat's or dog's face on it to complete the picture, you can reason that your model can classify the difference between cats and dogs. With a little tweaking and some training on some labeled data, it can be changed into a prediction model.

This is similar to **transfer learning**, where we transfer knowledge from one task in ordre t oaccomplish another. You'll see this more with neural networks.

## Reinforcement Learning 

A learning system, called an agent, observe the environment and can perform actions to get rewards (or penalties). It must learn by itself the best strategy, called a policy, to get the most rewards over time. This policy will dictate what action the agent chooses in a given situation. The idea of, what moves maximizes the rewards on the chess board, and ultimately gives it the highest chance of winning.

## Batch (Offline) vs Online Learning

### Batch Learning
In **batch learning**, your system doesn't learn incrementally. You feed it all of your data all at once. This takes a lot of computing resources and time, so this is typically done offline first. First you train the system, and then it's launched into production without learning anymore, now just making predictions based on what it learned offline. This is called **offline learning**.

The data of **data drift or model rot**, is when overtime your model doesn't perform as well, because the world is changing. And to prevent this you'll need to retrain your model on new data. For example, you're probably not going to experience much data drift on a classifier that identifies cats or dogs, but a model that predicts the current stock market may experience a lot of drift in just a couple of months.

However remember some considerations: 
- Training using a full set of data can take many hours. So training a new system every 24 hours or weekly seems kind of annoying. If your system needs to adapt to rapidly changing info, then it needs a more reactive approach.
- Training is computationally expensive (network IO, memory, CPU) and could cost a lot of time and money. Doing it very frequently is probably the wrong move.

### Online Learning
In **online learning**, we incrementally feed the system new data in small groups called mini-batches. Learning is fast and cheaper, and it can learn on the fly. Here's the workflow:
- Train and test the ML model with some input data.
- Once it's at a good enough quality, we launch it into production.
- Then for each new data that it receives during production, it will also learn/adapt from that data.

Sometimes we want to train a model on a constrained environment. Here we can't just load in our entire dataset in memory. However we can stream that huge dataset into the algorithm, into manageable mini-batches.

A core idea of online learning systems is the **learning rate**, which is how fast your algorithm should adapt to new data. If you set a slow learning rate, the system will learn slowly, but it's less sensitive to noise and outliers.

One last risk is knowing that bad data can be given to your system whilst in production, that will decrease the quality of your algorithm. In that case, you should have monitoring setup, and even rollback features so that you can rollback your deployment to a previous model that worked.  

## Instance-Based vs Model-Based Learning

Most ML tasks involve making predictions, so given some training data, it needs to be able to predict (or generalize to) examples it hasn't seen before. While it's good that a model does well on the training data, the true test is whether it generalizes well to new data. The two main approaches to this are instance-based and model-based learning.

### Instance-based Learning 

Instance-based learning, like K-Nearest Neighbors, doesn't learn in the sense that it adjusts weights and whatnot during the training phase. Instead it **memorizes the entire training dataset**. Then wehn a new data point needs to be classified, it compares this new point directly to points in the training set, based on a similarity measure (e.g. distance). To review:
- Lazy Learning: No model is trained, no computations are done. The algorithm doesn't adjust internal weights or biases. All computations are done at prediction time.
- High Memory Usage: If you're deploying it, you need to store the entire training set to make predictions.

### Model-based Learning
You can also build a model to make predictions. You're not making decisions based off of some instances, but now a pretrained algorithm that divides your data into  decision boundaries.

### Example 1: Model Based Learning (p. 22)
We want to know if money makes people happy, so we we download a dataset that stores the GDP of a country and the life satisfaction in that country.

- Plot the data on a scatter plot.
- Realize that the data has a positive association that's linear. As the GDP increases, life satisfaction also increases. We'll "model" life satisfaction as a linear function of GDP per capita. This is the idea of **model selection**, and we just selected a linear model of life satisfaction with one attribute being GDP per capita. 
  - Equation: $\hat{y}_{i} = \theta_{0} + \theat_{1} \cdot x_{i}$, with $y$ and $x$ being life satisfaction and GDP respectively. 
- Our model has parameters $\theta_{0}, \theta_{1}$. We tweak these parameters to change the prediction line. Here we're setting up a linear regression problem, and so the best performing linear model would be one that reduces the sum of squared error. In ML, we define a **cost function** that measures how much error that our linear model, or just any ML model has when predicting stuff. Here your cost function would be one that sums the squared error, pretty simple.

Should be obvious but just like how a linear model (a line) can model/best follow our data, a ML model is just an algorithm that best predicts your data. The idea of training a model means running the algorithm on some data to find the **parameters (or weights)** that best minimize the cost function (error), the parameters that maximize the accuracy.

After you find the best parameters for your data, which is pretty simple in this case, you'd now test your model on new data. Based on that you could take more steps:
- Want your model to perform better? Include more attributes/features.
- You may collect better quality training data.
- Select a more powerful model such as a polynomial regression model.

In summary the machine learning workflow is:
- Study and analyze our data for patterns.
- Select a ML model that can best fit/follow/model the data.
- Train the model using some training data. Here the model updates its parameters to minimize the cost function, the amount of error.
- Finally, we see our model is decent, so we apply it on new data points (**this is called inference**) to see if it generalizes well.

## TLDR and Extra notes 
- Words target and label are generally synonyms in supervised learning. Target is more common in regression tasks and label in classification. But they're the thing that we want our model to succeed in classifying. Features are sometimes called attributes or predictors. 
- You'd probably do batch learning you're able to load in a large dataset in memory, or when you're not worried about your model needing to rapidly adapt to data in real time. Online learning is when you need your model to adapt to data very quickly, or when you're training a model in a constrained environment (e.g. IoT device) and need to stream mini-batches of that data to the model.
- Inference is the idea of testing our data on new cases, it makes new predictions.
- The general idea behind ML is to have some mathematical model that best fits/follows our data, and then "fine-tuning" that model by running it on our data/task specifically. During training it'll updating its parameters S.T. it minimizes error.
- In an ML project you gather a training set, feed it to your learning algorithm. In a model-based algorithm, it'll adjust some parameters to best fit the training data given. Finally we hope it'll be able to make accurate predictions on new data, data it has never seen before. In an instance-based algorithm, it'll just use the examples it has already learned, make a comparison based on similarity, and classify new data points that way. With instance-based learning, it can feel less computationally expensive because there is no training phase. This is very from other ML models that require intensive iterative training in order to refine the parameters/weights. But once a model-based algorithm is trained, the making predictions should be pretty fast.
- You can use parameters or weights interchangeably, they mean the same thing.