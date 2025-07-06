# Testing and Validating
To train and test our model, we split our data into two sets a training and a testing set. You train your model on the training set, and you test it using the testing set. You can define the error rate on new data points (new cases) as **generalization error**, and this is how you can evaluate how well your model performs on instances it has never seen before.

It's common to have 80% of your data set aside for the training set and the remaining 20% for the testing set. However this does depend on the large of the dataset. A dataset containing 10 million instances may set 1% for the testing set (100,000 instances), which is likely more than enough to be a good estimate of the generalization error.

## Hyperparameter Tuning and Model Selection

If you're hesitating between 2 types of models (e.g. linear vs polynomial), one option to decide between them is to train and compare how well they perform on the test set. Say the linear model generalizes better but we want to use regularization to further avoid overfitting. One way to choose the value of your regularization hyperparameter is to train 100 different models using different values. Say we find the model that produces the lowest generalization error, but when we launch it in production it performs noticeably worse than expected. The reason this happens is because we only tested and measured on that one specific test set.

A common solution to deciding betweeen different models and also choosing different hyperparameter values for a given model is called **holdout validation**. Let's see the workflow:
1. You set aside a portion of the training set specifically for evaluating several candidate/potential models. This is called the **validation set**, and you're using this for testing only.
2. Then create a new training set called the "reduced training set", which is just the full training set minus the validation set.
3. For all candidate models, train them on the reduced training set. Then test them on the validation set. Select the model with the best performance on the validation set.
4. The holdout validation process is finished, so now we can train our best model (with the best hyperparameter) on the full training set, and that gives you the final model.
5. Test the final model to get an estimate of its generalization error.

The motivation of holdout validation is to provide an intermediate set that does our model selection and hyperparameter tuning, leaving a final test set for an unbiased estimate of the generalization error.

This works well but ensure the validation set is large enough to reduce the risk of selecting a suboptimal model. Again this is for comparing different models and selecting the best hyperparameters for a given model. However make sure the validation set isn't too big such that your reduced training set is a lot smaller than the full training set. This is bad, and that makes sense because remember since your final model will be trained on that full training set. It's like selecting the fastest sprinter, to perform in a marathon. 

## Data Mismatch

Let's say you're making a flower ID app. You can download millions of flower pictures on the internet, but in production the representive data is going to be images of flowers taken on the app. You need to ensure that your validation and final testing set consists only of representative data. So train your modeles on the web pictures. Then after testing on the validation set, you'll observe bad performance yet you won't know whether this is due to your model overfitting the training set or whether it's due to the mismatch in data.

To solve this:
- Reserve a portion of the web pictures in another set (train-dev set). 
- After the model is trained on the reduced training set, evaluate your model on the train-dev set. 
  - If the model performs poorly some kind of overfitting happened rather than a data mismatch because we trained and tested it on the web images. In this case regularize/simplify the model, get more training data, and clean the training data.
  - If the model performs well, then try testing the model on the validation/development set (the one with representative images). If this performs poorly, you know that it's probably due to the data mismatch since the photos are different. You can solve this by trying to make the web images look more like the pictures taken by the mobile app, then try the training process again.

## No Free Lunch THeorem
Rather than evaluating every model, we make reasonable assumptions about our data, and only evaluate a few models. For simple tasks you may evaluate linear models with various levels of regularization, but for complex problems you may start by evaluating neural networks.

## Exercises

- Let's say you want to segment your customers into groups. If you don't know how to define the groups, then use a clustering algorithm (unsupervised learning) to segment your customers into clusters of similar clusters. However if you do know the groups that you'd like to have, feed your data into a classification algorithm (supervised learning), and it'll classify your customers into these groups.
- Online learning is when a system can learn incrementally. So it's deployed and learns on the data whilst it's deployed, always changing. 
- Out-of-core algorithms are used when a large about of data can't be loaded in memory, so we split that data up into mini-batches. It's commonly used with online learning as your model will learn from said minibatches.
- Model-based algorithms are trying to optimize their parameters S.T. they minimize the cost function. We can also apply a penalty on how fast it learns which is called regularization hyperparameter.
- A validation set is used to compare models. It makes it possible to select the best model and best hyperparameters. So model and hyperparameter selection if you have multiple candidates and hyperparameters.
- **cross-validation:** Still need to review this because we barely covered it lmao.

- You use the train-dev set whenever we have a risk of data mismatch between the training data and the data used in the validation/test datasets. 
- If you tune your hyperparameters using the test set, you risk overfitting on the test.


