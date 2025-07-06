# Cross Validation (K-fold Cross Validation)

Cross-validation is a statistical method used to estimate the performance of ML models. Imagine you have a task and you have multiple models in mind that could be good candidates for the job. We can use cross-validation to estimate their performances, compare them to each other, and make a decision based on that. 

## k-Fold Cross-Validation
It's a resampling procedure primarily used to estimate a model's performance on unseen data. In ML, to see if a model fits your data, you'd split your data into a training and test set. Then you'd train the model on the training set and test it on the testing set. However we only evaluated our model once, what if that good result we got was by luck? For example what if we somehow selected the right training and testing set that makes this model look good, and most other configurations show that the model is bad. Ideally, we want to evaluate the model multiple times to be confident about it.

This procedure has a parameter $k$ that refers to the number of groups that your dataset will be split into. When $k=10$ that means we're doing 10-fold cross validation, and we're splitting the data into 10 (ideally equal) sized groups. It's popular since it's less biased and less optimistic of the model's skill than other methods such as a basic train/test split. Here's the workflow:

1. Shuffle the dataset randomly.
2. Split the dataset into $k$ groups
3. For each unique group:
    1. The current group is now a hold out or test dataset.
    2. The remaining groups form the training set.
    3. Fit a model on the training set and evaluate it on the test set.
    4. Retain the evaluation score, discard the current model (reset the parameters).
4. Summarize the skill of the model using the sample of model evaluation scores you recorded.

Here are some important considerations:
- Each observation in the dataset is assigned to a given group and it will stay in that group for the remainder of the procedure. It doesn't somehow move between iterations or between different models. 
- Do data preparation (scaling, encoding, imputation) and hyperparameter tuning inside the CV (Cross-Validation) loop rather than on the entire dataset. If you're doing it on the entire dataset, you're leaking data, your model "sees" data it shouldn't during training.
- The results of a k-fold cross-validation run are often summarized with the mean of the model's skill scores. You should also include the variance of skill scores.

## Configuration of k
A bad k-value can result in the mis-representation of the model's skill. Here are some common tactics:
- Representative: The value of k is chosen S.T. each train/test group is large enough to be statistically representative of the broader dataset.
- $k=10$: Fixing it to 10 or one that's been found through experimentation is generally good.
- $k=n$: Where $n$ is the size of the dataset, which would give each test sample an opportunity to be used in the "holdout" dataset. This approach is called **leave-one-out cross-validation**.

## Credits
- [Introduction to k-fold cross validation](https://machinelearningmastery.com/k-fold-cross-validation/)
- [Cross Validation Walkthrough - Rob Mulla](https://youtu.be/-8s9KuNo5SA?si=XIXU_wF9oBjK0Drn)