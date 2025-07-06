# Chapter 2: Training simple ML algos for classification



### Intro and premise
Let's learn about classification, and how perceptrons play a hand in that. So basically we have a graph of data, and we want to be able to classify that data. The way that we do this, is that we draw a decision boundary (**hyperplane**), and data points on one said of that decision boundary are classified as positive (in one class), and the data points on the other side of the decision boundary are considered negative.

Determining which sides of your decision boundary is actually simple, and you kind of get to choose that. The general idea is you'll have a vector lying on your hyperplane, and then a vector normal to your hyperplane. 


### Definition: Hyperplane
In 3D, a plane is expressed as a set of solutions (x,y,z) to the equation $ax+by+cz+d=0$, where $a,b,c \in \mathbb{R}$:
- $ax+by+cz+d > 0$: Any points (x,y,z) that satisfy this are on one side.
- $ax+by+cz+d < 0$: Any points here are on the under side. 
- $ax+by+cz+d = 0$: Here your point lies on the plane itself.

Given a vector normal to the hyperplane $\vec{n}$, and a known point $x_{0}$ lying on the hyperplane, we can model an equation for that hyperplane: 

- $\vec{x} = x_{p} - x_{0}$: Here we define a displacement vector from our known point to any other point on the plane. The intuition here is that since this is on the plane, this will be perpendicular to $\vec{n}$

You can reason $\vec{n} \cdot \vec{x} = 0$. The dot product of any given displacement vector with the vector normal to the plane is going to be zero. You can already intuit that the equation of the hyperplane encompasses all vectors in that hyperplane. You'd solve that dot product, which would result in a formula for a plane like above. To re-emphasize, this hyperplane is our decision boundary. The vector normal to the hyperplane, will later be known as a **weights vector**, which we will use to dot product other data points. So our hyperplane equation would look like $\vec{w} \cdot \vec{x} = 0$. If the dot product is positive, the data point is mathematically above the hyperplane, else below. 

Finally there's this idea of a **bias**. The bias is basically that constant $d$. By default, without any constants, your hyperplane will pass through the origin, you can even verify this mathematically through Desmos. By adding a constant $\vec{w} \cdot \vec{x} + b = 0$ will add a constant $d$ to our hyperplane equation. This will shift our hyperplane left, right, up, or down (depending on the weights). But how does this affect decisions? Well this gives us more control over our decision boundary:
- Let's say for a given piece of data: $w \cdot x = 0.8$
- Let's also say we don't want to classify this data as positive until the value is greater than 1. Then let $\beta = 1$. 
- Now $w\cdotx+b = 0.8-1 = -0.2$, so it's a negative classification now. So the bias is essentially just the threshold value. By default, if your hyperplane passes through the origin, then $\beta = 0$, meaning that you'll classify something as positive when the $w \cdot x >0$. However, if I set $\beta = 13$, then a data point is positive (mathematically above the hyperplane) when $w \cdot x > \beta$.


### Definition: Linearly Separable 
Two sets $M_{1},M_{2} \subset \mathbb{R}^{n}$ are **linearly separable**, if there exists a set $a_{1},...,a_{n}$ and $\theta$ S.T:

$\sum_{i=1}^{n}{a_{i}x_{i} > \theta} \forall x \in M_{1}$ and $\sum_{i}^{n}{a_{i}x_{i} \leq \theta} \forall x \in M_{2}$

- $\theta$: Is a threshold value. Meaning all $M_{1}$ will land above $\theta$ whilst all $M_{2}$ values will land below $\theta$.

### Perceptron Learning Algorithm: 
- Goal: Find $\vec{w}$, our weight vector; the parameters being the coefficients 
Then the algorithm:
- $M_{+}$: Set of observations with positive class labels.
- $M_{-}$: Set of observations with negative class labels.
- $\vec{w}$ = arbitrary vector of real numbers; these are weights (parameters)
- Repeat until all $x \in M_{+} \cup M_{-}$ are correctly classified:
  1. $\forall x \in M_{+}$:
    a. If $\vec{w} \cdot \vec{x} \leq 0$
      - $w = w+x$
  2. $\forall x \in M_{-}$:
    a. If $\vec{w} \ge 0$:
      - $w = w-x$



#### Algorithm Explained
Assuming our data is linearly separable, so we know that we can separate the data with some hyperplane equation that has certain parameters. Our hyperplane is constantly tilting or moving up or down, until everything is correctly classified. If $x \in M_{+}$ it means that it's above the hyperplane, or equivalently $w \cdot x \ge 0$. Again, if $x \in M_{-}$, then we'll say it's below the hyperplane. Now we loop:
- For all positive values, do a dot product on it. If our dot-product is non-positive, a non-positive class label, we're underestimating the real score. As a resulst, we'll increase the weights vectors in hopes that next time, we won't underestimate thing 
- For all negative values, do the dot product. If it's positive, we're overestimating things so subtract from the weights vector in hopes of not overshooting next time. 

sWe continue doing this loop until we classify all samples correctly. Since we know that our data is linearly separaabley, we know there is some hyperplane that classifies all data points correctly.

#### Example 1
- $M_{+} = (0, 1.8), (2, 0.6)$
- $M_{-} = (-1.2, 1.4), (0.4, -1)$
- initial weight vector $\vec{w} = (1,1)$
- straight line: $\vec{w} \vec{x} = 0$

Begin the algorithm:
1. w(-1.2, 1.4) = 0.2 > 0, but this data point belongs in the negative pile. That's a misclassification, as we overestimated things, so we'll subtract from the weights vector
  - w = w - (-1.2, 1.4) = (2.2, -0.4)

Then continue with the negative pile. Then begin on the positive pile. After you've looped through both piles that's one iteration. You keep iterating until all four of those data points are correctly classified. Remember our weights vector is perpendicular to this hyperplane, so as $w$ changes in direction, the plane that it's perpendicular from changes. And if we get the perfect weights, the plane should separate data points from both class labels.

Then when you test it, the weighted sums of the inputs go int othe perceptron. Then the perceptron applies an activation function to it, converting that value to a number from 0 to 1.


 
### Multi-Layer Perceptron
A common neural network model, where we use many perceptrons to do more advanced classifications. With a single perceptron, we have many inputs and give us one output. But in many machine learning tasks, we have many inputs and many outputs. The common example is that you want each each perceptron output to give you a probability of it being a given digit.

With simple perceptrons, they can only solve problems where the data is linearly separable. Where there exists a boundary that allows us to separate the data using a hyperplane. However, most interesting problems aren't linearly separable. While a simple perceptron can solve OR and AND, it can't solve XOR because XOR isn't linearly separable. 

However a multilayer perceptron can solve XOR. You can take the perceptron for AND and the perceptron for OR and derive a result from them. Because remember that XOR = OR and NOT AND. So if you have their results, combining them together 

 




---







## Perceptron Math: Deeper Dive
In early days, the neuron would have a binary output. Given many inputs, if we reached a certain threshold, we would output something. This can be related to a binary classification task, where we have two possible output classes 1 and -1.

- Perceptron decision function: If the net input if a particular sample/row of data $x^{i} \geq \theta$ we classify it as 1, otherwise -1. 
- Net input $z$: Where $z = w_{0} + w_{1}x_{1} + ... + w_{m}x_{m} = w_{0} + w^{T}x = w_{0} + \sum_{j=1}^{m}{w_{j}x_{j}}$.
- weight vector w
- input vector x
- unit step function: $\phi(z)$, outputs 1 if $z \geq \theta$ and -1 otherwise. The decision function is a variant of this.

Let $w_{0} = -\theta, x_{0} = 1$. Then we can re-write $z = w_{0}x_{0} + ... + w_{m}x_{m} = w^{T}x$. Now our decision function is $\phi(z)$, which outputs 1 if $z \geq 0$ and -1 otherwise.
- NOTE: $w_{0} = -\theta$, the negative threshold/weight is usually called the **bias unit**. Since we compacted it with vector multiplication, we can say our threshold is 0 rather than $\theta$. Also putting it as $w_{0}$ and letting $x_{0}=1$, compacts the computations, and is equivalent to what we've done above. 

---
### Perception Learning Rule: Basic Idea of the Algorithm
Mimic the human brain, a neuron fires or doesn't. Here are the steps:
1. Initialize weights to 0 or small random numbers.
2. For each training sample $x^{i}$ compute the output value $\hat{y}$. Then update the weights.

Here's info associated with updating weights:
- $w_{j} := w_{j} + \delta w_{j}$.  
- $\delta w_{j} = n(y^{i} - \hat{y^{i}})x_{j}^{i}$
- n: This is the learning rate, a constant in range [0.0, 1.0].
- $y^{i}$: The true class label for the ith sample/row of data.
- $\hat{y}^{i}$: Predicted class label for the ith sample.

- **NOTE about notation:** := is mathematical notation to indicate the left-hand variable is being updated/reassigned to a new value.

In the case we predict things correctly, then $\delta w_{j} = 0$, the weights won't change. However, in the case of a wrong prediction, the weights are pushed in the direction
of the positive or negative target class. For example $\delta w_{j} = n(1 - (-1))x^{(i)}_{j} = n(2)x^{(i)}_{j}$, here the actual was positive 1, so our change in weight is going to be positive.

Let $y^{i} = 1, \hat{y}^{i}_{j} = -1, n=1$. Assume that $x^{(i)}_{j} = 0.5$. Here we misclassified the sample as -1, and we want to increase the $w_{j}$ by 1 so that the net input $x^{i}_{j} \times w^{i}_{j}$ is more positive next time we encounter this sample as it's going to be more likely to be above the threshold.

So the amount weight is updated is proportional to the value of the multiplicative factor $x^{i}_{j}$. So if you have $x^{i}_{j} = 2$, so column j for the ith row is 2, then we change the weight by a larger extent in order to classify the sample correctly next time. In essence, you multiply by the input feature so that the update scales proportionally to the feature's value.

--- 
### Limitations for Perceptrons
The convergence of the perceptron model is only guaranteed if the two classes are linearly separable and the learning rate is small. This just means your model can only converge on an answer, only if the data can actually be separated by a line (decision boundary). If your data can't be separated by a decision boundary, which probably will happen, you definitely need some conditions.

Set a maximum number of passes or times we go over the data set (epochs). You can set a threshold for the number of misclassifications we tolerate before stopping, which prevents us from constantly updating the weights.

--- 
### Review
1. Use net input function to calculate $z = w_{0}x_{0} + ... + w_{m}x_{m}$.
2. Use threshold function: $\phi(z)$ which will output 1 if $z \geq 0$ and -1 otherwise. Here {1, -1} are our class labels for this sample/situation. $\theta=0$ is our threshold since we have $-\theta$ in our weight vector.
3. This calculates the predicted value, our output, and during the learning phase it calculates the error and updates the weights based on our errors.

## Multi-class classification
We can extend our perceptron algorithm, and use the "One-versus-All" (AKA One-versus-rest). This allows us to transition from a binary classifier to a multi-class classifier. So now we're not using prediction whether the outcome is either 1 or -1, but now there are multiple possible outcomes, as we can see with the flower dataset. 

---
### Implementation and steps
Extract the first 100 class labels that correspond to the 50 "Iris-setosa" and 50 "Iris-versicolor". Convert them to integer class labels 1 (versicolor) and -1 (setosa).
Then put these in your y-vector. 

Extract the respective sepal length (first column) and petal length (third column) of those 100 training samples and create your data matrix x. The graph is just a scatter plot showing the 
relationship of the petal length and sepal length for two different flowers. The data can be separated with a linear decision boundary to classify whether a flower is a setosa or a verisicolor flower.

---
### Where the decision boundary comes from 
It's the set of points where the net input is exactly 0.

So it's z = w^{T}x + w_{0} = 0. So let's just solve this for 2 features $x_{1}, x_{2}$. It would look like 

w_{1}x_{1} + w_{2}x_{2} + w_{0} = 0. You can then solve this for one of the x values, which gives you the equation for the decision line, including the slope and y-intercept.

---
### The bad thing about convergence
The perceptron learning rule/algorithm converges if two classes can be separated by a linear hyperplane (the line in 2d space or a plane in 3d space). 
But if data can't be separated by a linear decision boundary, then your weights won't ever stop updating, unless you set a maximum number of epochs/iterations.