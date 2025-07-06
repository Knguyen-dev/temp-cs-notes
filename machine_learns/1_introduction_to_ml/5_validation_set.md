# Train, Test, and Validation Sets

## Why We Split Data Into Three Sets
When building ML models, we need to ensure they perform well on new, unseen data, rather than just memorizing the training examples (generalization). To achieve this the common technique is to split our data into three distinct sets, each serving a specific purpose

### Training Set (70% of data)
- **Purpose:** Teach the model patterns in the data.
- **What happens:** The model's parameters (weights) are updated based on this data.
- **Labeled:** The data is labeled as the model learns from both inputs and correct outputs.

### Validation Set (10% of data)
- **Purpose:** Monitors training progress. If the model does bad, then maybe we need to tune those hyperparameters.
- **What happens:** Model performance is evaluated, but no parameter updates occur when using the validation set.
- **Labeled:** We still need to know the ocrrect answers to measure performance.

The validation set is the "reality check" or verification on whether your model is performing decent or not. Remember this set should be a set of data that your model has never seen before, so if it does well on this, that's at least a good indicator that it may do well on the testing set and of course new unseen data that we don't even have.

This set is also a critical part of hyperparmaeter tuning, as here if a model doesn't do well on the validation set, that's an indicator that maybe we can fix this be tuning the hyperparameters (e.g. changing regularization).

### Test Set (20% of data)
- **Purpose:** The final test or evaluation on our completed model. 
- **What happens:** It's the one-time assessment after all training (train + validation) is complete. 
- **Labeled:** Typically you keep it labeled, but you can have it unlabeled to simulate real world conditions.


## Training Workflow

### Step 1: Training Phase 
1. Model processes our entire training data.
2. It makes predictions.
3. Compare those predictions to actual labels, basically comparing answers.
4. Updates parameters to reduce errors.
  - **Note:** Typically if it makes an incorrect guess specifically, it updates parameters. If it's correct there's no need to change. That's typically anyways, I don't know about more advanced ML models.
5. Repeats this for multiple epochs.

One **epoch** is just one iteration where it goes through the entire data and updates itself. Usually an ML model goes through the training data multiple times/iterations/epochs.

### Step 2: Validation Phase (happens after each epoch)
1. Model makes predictions on validation set.
2. Performance is measured (accuracy, loss, etc.)
3. No parameter updates as this is purely for evaluation.
4. The results guide decisions about:
    - Hyperparameter adjustments (learning rate, regularization, etc.)
    - When to stop training (early stopping). This helps us also avoid overfitting.
    - Architecture changes, moving to a different model, etc.

### Step 3: Testing Phase (final step)
1. Model is evaluated on the test set only once.
2. This gives an unbiased estimate of real world performance.
3. If the results are poor, you can't tune your hyperparmaeters based on the test performance, that invalidates the test.

## What the validation set does for us?
It helps us solve two main issues:
1. **Generalization:** If the training set and validation set scores are good, the model likely isn't overfitting. The model has never seen the data in the validation set, so having a good score means it generalizes well to new data.
2. **Overfitting:** If the training accuracy is high, but validation accuracy is low, it means the model is memorizing training examples rather than learning patterns (overfitting). As a result we need to reduce the model's complexity by applying more regularizatio nor getting more training data.
3. **Underfitting:** If the training accuracy and validation set accuracy are both low, we're underfitting.

Without proper data splitting:
- You might think your model is good when it's actually just memorizing.
- You could accidentally tune your model to perform well on your test data, making your perofrmance estimates overly optimistic.
- Your model may just fail horribly when deployed in production.

The validation set is just an early warning system that tells us during training whether our model is generalizing well to new data or just memorizing the training examples.

## Credits
- [Why we split data into train, test, and validation sets - Misra Turp](https://youtu.be/dSCFk168vmo?si=OgS6nbHCAaaTYs0p)
- [Train, Test, and Validation Sets explained](https://youtu.be/Zi-0rlM4RDs?si=T3z1v4mF_cWd9hke)