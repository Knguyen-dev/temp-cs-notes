# Challenges in ML

The main things we do is select a model and train it on some data. And those 2 choices are the main things that can go wrong.

## Insufficient Quantity of training data
It takes a lot of data for most ML algorithms to get decent results. For simple tasks, thousands of examples. For complex tasks relating to images or speech, you may need millions of examples, unless you can reuse parts of an existing model.

Studies have shown that given enough data, a simple algorithm can perform just as well as a complex one. However, having small-medium sized datasets is very common, it's not always easy or cheap to get large amounts of data, which is why algorithm selection still matters a lot.

## Non-representative and poor data
You need to make sure that the your training data is representative of the data that you're trying to predict. For example, if you have an image recognition mobile app that is supposed to identify different types of flowers in a forest, your training set should be a collection of images of flowers taken on that app. That's representative of what the actual data will look like, what your model will be tested on in production. A non-representative set of data would be having photos of those flowers in a store, in a flower pot, in a photoshop wallpaper, etc. Those aren't representative of what the real data will actually look like.

If our sample is too small, you'll have sampling noise, where outliers can have more of an effect on your data. Or if you have a large sample but the way you picked that sample wasn't random, then you'll still have **sampling bias**.

If your data is full of errors, outliers, and noise, this makes it harder for your system to correctly detect any underlying patterns and actually perofrm well. Most of the time, data scientists are cleaning training data:
- We may discard outliers or try to modify them.
- If some instances/rows are missing a few features (e.g. 5% of customers don't have their ages filled out), we have to decide on a plan:
- Ignore the attribute entirely, 
- Ignore the instances that are missing said attributes.
- Fill in the missing values with something like a median.
- Train one model with the feature and one without it to see how they compare.

## Irrelevant Features
A part of the process is making sure our system has enough relevant features to help it differentiate data points and find a pattern in the data. An important step in ML is called **feature engineering**, where we ensure that we've identified the important features that the model should train on. Here's the workflow:
- Feature selection: Selecting the most useful features to train on among existing features.
- Feature extraction: Combining existing features to produce a more useful one. We saw this earlier with dimensionality reduction, and dimensionality reduction algorithms will help.
- Creating new features by gathering new data.

## "Noise" in Data Science
This kind of ties in to the previous sections, but here we go. In data science, **noise** refers to random or irrelevant variations and errors that obscure the true underlying patterns with the data. It's like static, it's extra parts of a signal that doesn't convey any meaningful information. Nosie makes it harder for ML models to learn accurate relationships, and leads to poor performance. Let's explain various types of noise:
- **Measurement error:** Inaccuracies in the data collection process due to faulty sensors, imprecise instruments or human error.
  - E.g. a thermometer consistently reading 2 degrees higher or fluctuating.
  - A person typing "100" instead of "1000" for a price.
  - A microphone picking up background chatter in an audio recording for speech recognition.
  - There's also the idea of data transmission errors, or maybe some external factors in the environment that interferes with sensors or something, but you get the idea.
- **Irrelevant Features/Dimensions:** This is sometimes considered a form of noise. Attributes that have no bearing on the target variable, but are still present in the dataset when training. It's not an error in the traditional sense, but it adds complexity and can confuse a model. 
  - Including a customer's shoe size when trying to predict if they'll purchase a new phone. It's an irrelevant attribute, it's probably not going to influence whether the customer purchases the phone or not.
  - The color of a car when predicting its fuel efficiency.
  - The timestamp down to the milisecond when predicting monthly sales trends.

There's also the idea of **sampling noise**, which is the random variations or discrepancies that are present in the smaple you draw from the population. It's just the random variation inherent to the sample.

### Example: Sampling Noise
Imagine you want to know the average height of all adults in your country. You take a sample, try to do an SRS to avoid any sampling bias, (when we pick a sample in a weird way that influences results unfairly). The average height in the sample probably not be exactly the same as the true average height of the entire population. It'll be close, but there will be some slight random difference, and that difference is just the sampling noise. Here are some key characteristics:
- **Inherent to sampling:** An unavoidable consequence of working with a subset of data rather than the full population.
- **Random Fluctuation:** Reflects the natural random variation that exists when doing samples. Remember that $\bar{X}_{n}$ won't always equal $\mu$.
- **Reduces with Sample Size:** As sample size increases $E(\bar{X}) = \mu$, meaning as the sample size gets larger, we have a higher chance of not deviating from the population mean.

## What is overfitting? How do we avoid it?
Let's say you have a bad experience with one taxi driver in a foreign country. Then you say that all taxi drivers there are bad. This is called overgeneralizing. In ML, we call this overfitting, when our model performs well on the training data, but doesn't generalize well meaning it doesn't do well on new data.

Complex models like neural networks can detect subtle patterns, but if our training set is noisy or too small, the model will pick up on patterns based on that noise. As a result, it's going to recognize patterns specific to the idiosyncrasies in your sample, and it's not going to generalize well to new data. It simple fits too well to the sample data and doesn't fit well to new data, it overfits, hence the name. For example, in our life satisfaction model, say we include more attributes including useless ones like the country's name. Here a complex model could detect a pattern like "all countries with a w in their name have a life satisfaction greater than 7". Obviously this is a pretty bad pattern that was learned by chance, but a model doesn't have a way to tell whether a pattern is actually significant or just a product of random noise. 

Overfitting happens when a model is too complex relate to the amount and noisiness of the training data. Here are some solutions:
  - Simplify the model by selecting one with fewer parameters (e.g. using a linear model rather tha an high-degree polynomial model). 
  - Gather more training data.
  - Reduce the noise in the training data. Done through fixing data errors, removing outliers, or reducing the number of features in the training data as well (dimensionality reduction). You can also try to "constraint a model".

## Constraining a model: Regularization and Hyperparameters 

**Regularization** is the idea of constraining a model to make it simpler in order to reduce the risk of overfitting. The linear model $\hat{y}_{i} = \theta_{0} + \theta_{1}x_{i}$ has 2 parameters, 2 **degrees of freedom** that allow the model to fit the data. If you fix $\theta_{0} = 0$, now we have only 1 degree of freedom, one parameter to move the direction of the line to fit the data. We have a harder time fitting the data like this though.

The idea is to only only small changes to $\theta_{1}$, creating a model that's between 1-2 degrees of freedom. Simpler 2 degrees, but more complex than 1. Whilst the model may not fit the training data as well without regularization, it may generalize a lot better to new data. The motivation for this is finding the balance between fitting the training data well and generalizing well.

The "amount" of regularization to apply during the learning process is controlled by a **hyperparameter**. A hyperparameter is s parameter of your algorithm that doesn't change during the learning process, but rather it's a constant that you fix beforehand. If you set the regularization hyperparameter to be large, your slope is going to be close to 0, so it won't overfit but it's not going to fit at all with the training data. Hyperparameter tuning is an important aspect of the ML workflow.

## What is underfitting?
It's the opposite of overfitting. It's when your model is too simple that it isn't able to learn the underlying patterns of the data. Here's how you fix this:
- Select a more powerful model, a model that has more parameters.
- Feed better features to the learning algorithm (feature engineering)
- Reduce the constraints on the model e.g. reducing the regularization hyperparameter.

## TLDR
- If the training set is small, not representative of the population, noisy, or has a lot of irrelevant features, it won't perform well. The can't be too simple (in which case you'll underfit) nor too complex (in which case you'll overfit).