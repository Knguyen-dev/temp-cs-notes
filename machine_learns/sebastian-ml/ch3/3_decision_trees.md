# Decision Trees (p. 149)

### Pre-knowledge: 
How easily a person can understand why an ML model makes a particular decision. A highly interpretable model allows us to more easily explain the predictions in a way that makes sense. You can easily see how features impact the output, or can easily visualize the decision-making process. On the other hand, a **black-box** model (like deep neural networks) are harder to understand. [Introduction to decision trees](https://youtu.be/ZVR2Way4nwQ?si=JmJMiDKYHQ45SJAu)

---
### Decision trees simply explained: 
A classifier algorithm that's good for when we care about interpretability. Think of the model as making decisions based on a series of questions. For example, "Is sepal width $\geq$ 2.8cm?", and then similar questions. All samples that meet the condition go to the left node, which can be a decision or leaf node. We keep splitting until we get a tree of nodes with all pure leaf nodes, which just means all leaf nodes have nodes with the same class labels. This can result in a really large tree, so you need **prune** the tree, which means you're setting a maximal depth of the tree. With decision trees, the goal is finding the best predicates. So what are the best parameters, the comparison operation, and value we compare it to, S.T. we get the highest accuracy without the tree being too big.

---
### Maximizing Information Gain (IG)
To do this, we'd choose a parameter to split the nodes by. Split the nodes with a parameter that gives us the highest **information gain** (IG). So calculate the IG for each attribute that you haven't already used, and then split using the attribute with the highest IG. To calculate IG, we need to calculate **entropy**. Entropy measures the level of impurity in a collection of samples, how uncertain things are:
  - S: Collection of training samples.
  - $p_{+}$: Proportion of positive samples,   
  - $p_{-}$: Proportion of negative samples

The value of entropy can be from `[0, 1]`, where 0 means a given collection is pure, all samples have the same class label. An entropy of 1 means that the proportion is half and half (maximum impurity). The formula for entropy (Shannon entropy) is as follows:

**Binary Entropy Formula :** $E(S) = -p_{1}log_{2}({p_{1}}) - p_{2}log_{2}({p_{2}})$

So here $S$ is the dataset, collection of rows that we're analyzing. Let's say we were predicting whether they allowed the tennis court to be opened on a given day, with the features being various whether factors. So here $p_{1}$ could represent the proportion/probability for when the court was open. Then $p_{2}$ could be the proportion/probability where it wasn't open. So there are only two possible values for the class labels, giving us two terms to sum up. This makes it pretty easy to **calculate the entropy for our dataset**.  However, not all classification tasks will have your set of class labels be just two possible values. It could be that the class label we're predicting could have 3 or more values. So the entropy of a dataset is generalized to handle $n$ class labels by summing up $n$ different probabilities:

**General Entropy formula:** $H(p)= -\sum_{i=1}^{n}{p_{i}log_{2}{(p_{i})}}$

- $p_{i}$: Probability of the i-th category (class label) occurring.
- $n$: The total number of possible values in the set of class labels.

After we calculate entropy, we can then calculate the information gain, which tells us how much that entropy (uncertainty) is reduced. By maximizing IG, we're minimizing uncertainty. Now that we know how to calculate the entropy for a dataset, we can now calculate the information gain:

$IG(S,A) = H(S) - \sum_{v_{Values(A)}}{\frac{|S_{v}|}{|S|} \cdot H(S_{v})}$
- $S$: Our dataset. 
- $H(S)$: The entropy of the dataset (often the parent). So do class label calculation for all data or parent node data.
- $H(S_{v})$: The 
- $A$: Attribute/feature we're calculating information gain for.
- $v$: A particular value for attribute A
- $|S_{v}|$: Subset of $S$ where $A=v$. So the subset of rows in the dataset where attribute $A$ equals some value $v$.
- $|S|$: Number of rows in our dataset

One more thing. The decision tree building algorithm is recursive. We continue splitting nodes recursively until we reach one of these conditions:
  1. All data points in the node belong to the same class (pure leaf node).
  2. No more unpicked features to split all (we've used all attributes).
  3. A predefined stopping condition is met. This could mean reaching a max depth or a node contains fewer than a minimum number of samples.

You should also notice that information gained is calculated as the difference between the parent's entropy, and the sum of the children's entropy. That's what the formula is also trying to describe.


### Algorithm for building a decision tree 


## Practice problems

#### Example 1: Entropy and Information Gain Calculations
We have a dataset of days, and some features such as the outlook = {Sunny, Overcast, Rain}, temp = {Hot, Mild, Cool}, humidity = {High, Normal}, and finally wind = {Weak, Strong}. THen there's the class label that we are predicting which is play_tennis = {yes, no}. Let's calculate the information gained if we split the nodes based on the Wind. We can reason:
- $S = [9_{yes}, 5_{no}]$; So 9 rows had yes for play_tennis with 5 for no.
- $S_{weak} = [6_{yes}, 2_{no}]$; Given that the wind is weak, 6 days had tennis be played and 2 no. We narrowed the scope.
- $S_{strong} = [3_{yes}, 3_{no}]$; Given that the wind is strong here are the stats. Okay we're ready to calculate the information gain with respect to the wind feature
1. $IG(S, wind) = E(S) - |\frac{S_{weak}}{S}|\cdot E(S_{weak}) - |\frac{S_{strong}}{S}| \cdot E(S_{strong})$
2. $IG(S, wind) = E(S) - |\frac{8}{14}|\cdot E(S_{weak}) - |\frac{6}{14}| \cdot E(S_{strong})$
  a. $E(S)= -|\frac{9}{14}|log_{2}{\frac{9}{14}} -|\frac{5}{14}|log_{2}{\frac{5}{14}} = 0.94$
  b. $E(S_{weak})= -|\frac{6}{8}|log_{2}{\frac{6}{8}} -|\frac{2}{8}|log_{2}{\frac{2}{8}} = 0.811$
  c. $E(S_{strong})= -|\frac{3}{6}|log_{2}{\frac{3}{6}} -|\frac{3}{6}|log_{2}{\frac{3}{6}} = 1$
3. $IG(S, wind) = 0.94 - (\frac{8}{14})(0.811) - (\frac{6}{14}(1)) = 0.048$

But then you would calculate the information gain for all other features. We were lucky:
- For wind there was only two possible values "strong" and "weak", which meant we only had to calculate entropy for those 2 subsets of the data.
- As well as this, there were only 2 class labels, which made calculating $E(S_{v})$ really easy since we only had to sum up 2 terms.

Reviewing, we calculated the information gain for wind. Then you'd just have to calculate the information gain for all the other features. Then you split the rows using the feature that has the highest information gain, the row that reduces randomness the most. This is a pretty arduous calculation, which is why we have computers do them.


---
#### Example 2: Walking through a decision tree
- **Decision Node:** Contains a decision to split the data.
- **Leaf Node:** Helps us decide the class of a data point
1. Start at the root, and we have 100 samples of data. The root node has a condition $x_{0} \leq -12$. All samples that satisfy this condition go to the left child node, and then the samples that don't will have to go to right child node. Let's say the left child contains samples will the same class labels, that means it's a pure node.
  - **NOTE:** This draws a vertical line at $x=-12$, and any point laying on this line and to the left meets this condition.
2. Now on right child, which is a decision node with condition $x_{0} \leq 9$. Again all rows that meet this condition, will go on to the left child node (decision node), and the rest go to the right child (leaf node). Let's say the right child is pure, but the left child is still impure. 
  - **NOTE:** Draws a line at $x=9$, and anything to the left or on the line meets the condition of this decision node.
3. Our left child node ($x_{1} \leq 9$). Now let's filter things, and let's say this results in two pure child nodes. Now at all our leaf nodes are pure, we don't need split our child nodes anymore.
  - **NOTE:** Since it's $x_{1}$, you'll have a horizontal splitting like at $y=9$. Anything below or on this line meets the condition.
Let's say we have new data (15,7). It wouldn't meet the conditions for the first two decision trees, and it's placed in a child node for samples with the class label "red". Then we can classify this new data point as "red". Visualizing this we created 2 regions, with each region containing only data points for that class label.


### Extra stuff about information gain:
We want to maximize information gain at each split:
- **Theoretical IG:** $IG(D_{p}, f) = I(D_{p}) - \sum_{j=1}^{m}{\frac{N_{j}}{N_{p}I(D_{j})}}$
- $f$: The feature we're splitting on e.g. sepal width.
- $D_{j}$: Dataset of the jth child node.
- $D_{p}$: Dataset of the jth child's parent.
- $N_{p}$: The total number of samples at the parent node.
- $N_{j}$: The number of samples in the jth child node.
- **Impurity Measure:** $I$ is our impurity function.

Information gain is just the difference between the impurity of the parent node, and the sum of impurities of the children. For simplicity, most libraries do **binary decision trees**, so it's just two child nodes when you split. 

- **Typical/Practical IG:** $IG(D_{p},f)=I(D_{p})-\frac{N_{left}}{N_{p}I(D_{left})-\frac{N_{right}}{N_{p}}I(D_{right})}$

---
### Final note: Random Forest Models
An **ensemable** of decision trees. The idea is to take the average of multiple decision trees that suffer from high variance. As a result, we can build a model that has better generalization performance and is less susceptible to outliers. Here's how it works:
1. Draw a random bootstrap of sample size n. (Choose n samples from the training set with replacement).
2. Create a decision tree from the bootstrap sample:
  a. Randomly select d features without replacement. 
  b. Split the node using the feature that's best according to your objective function.
3. Repeat steps 1-2 k times.
4. Aggregate the prediction of each tree, and we'll assign the class label using **"majority vote"**. The class label that is most frequently predicted is chosen as the final output. So if 3 trees labeled it as spam, and 2 didn't, we're going to go with the row being spam.

#### Other notes about random forests
- You don't need to scale the features
- Offers less interpretability
- Less hyperparameter tuning, only tune the number of trees $k$. Typically, more trees means better results, but at the cost of performance.
- With a single decision tree, you're going to use most of your features, if not all. With a random forest, each tree gets a random set of features to compare on.
- Dimensionality reduction can be used to optimize hyper-parameters, but in practice you'll probably never optimize one.
- For most random forest implementations, the bootstrap sample is equal to the number of samples in the original training set. For number of features $d$, we choose a value that's smaller than the total number of features in the training set. This is usually $d = \sqrt{m}$, where m is the number of features in the training set.


## Credits:
https://youtu.be/ZVR2Way4nwQ?si=Q0dJDjfQyZtmGj1H