## Support Vector Machines (SVMs)


### Basic idea
So basically instead of choosing a random hyperplane that separates the data, which there could be many solutions. We are picking the hyperplane that maximizes the margin between the data. This is defined as the distance separating the hyperplane (decision boundary) and the training samples that are closest to the hyperplane. The we'll draw secondary hyperplanes that line up with data points closest to the central hyperplane are called the **support vectors**. So we have a central hyperplane (**"decision boundary")** and then these two support vector boundaries that are parallel to it. 
We have the closest positive and closest negative, and the margin is the distance between those two. Decision boundaries with large margins tend to have lower generalization errors, whereas models with low margins are more prone to over-fitting

1. $w_{0}+w^{T}x_{pos} \geq 1$; So if a given row is below this, it's a positive classification, it's at or past the positive boundary.
2. $w_{0}+w^{T}x_{neg} \leq -1$; Same idea, but for the negative boundary

Note that the farther away the data point is from the support vectors, the more confident in our classification we are. As a result, this means that we aren't very confident about our prediction about data points that are near or on the support vectors.

We can quickly derive the margin: Subtract the equations from each other to get $w^{T}(x_{pos}-x_{neg}) = 2$. This can be normalized by length of w, which is $||\vec{w}|| = \sqrt{\sum_{j=1}^{m}{w_{j}^{2}}}$. That final equation is: $\frac{w^{T}(x_{pos} - x_{neg})}{||w||} = \frac{2}{||w||}$

The left side is known as the distance between the positive and negative hyperplanes, the thing we want to maximize. So we'd maximize under the constraint that the samples are classified correctly. Mathematically things look like this: 

1. $w_{0}+w^{T}x^{(i)} \geq 1$, if $y^{(i)} = 1$
2. $w_{0}+w^{T}x^{(i)} \leq -1$ if $y^{(i)} = -1$, where $1 \leq i \leq N$. Here $N$ is the number of samples/rows in our dataset.

The equations just say negative samples $y^{(i)}$ should fall into the negative hyperplane, whilst positive samples fall into the positive hyperplane. Things can be re-written more compactly as $y^{(i)}(w_{0}+w^{T}x^{(i)}) \geq 1 \forall i$. So yeah this is just the same constraints, but now in one equation. Finally we can refer to this as **hard-margin classification**, and your net-input shouldn't be between -1 and 1. In practice, it's easier to minimize $\frac{1}{2}||w||^{2}$.

---
### Dealing with a non-linearly separable case using "slack variables"
We're talking about when our data can't be separated by lines. So this is like the idea where your decision lines are curvy. This is idea of **soft-margin classification**. We introduce a "slack variable" to relax our linear constraints when we have non-linearly separable data. 
 - $w_{0}+w^{T}x^{i} \geq 1 - \alpha^{i}$ if $y^{i}=1$
 - $w_{0} + w^{T}x^{i} \leq -1 + \alpha^{i}$ if $y^{i} =-1$

Do this for the number of samples in the dataset of course. Now we need to minimize: 

$\frac{1}{2}||w||^{2} + C(\sum_{i}{\alpha^{(i)}})$

- **C:** A hyperparameter we can choose to control the penalty for misclassification. Large C values equate to large penalties, and smaller values lead to small penalties. This is very related to **regularization**, as decreasing C will increase bias and lower variance in our model. So yeah with this, you control the width of the margin, tune that bias-variance tradeoff, etc.

You can see a graph of this on p139-140.

---
## Logistic regression vs SVM

### A simple difference
Log. reg. is more prone to outliers as it tries to maximize conditional likelihoods. SVM just only cares about the points closest to the decision boundaries so it isn't prone. However log. reg. is simpler, making it easier to implement, but also  update, which is good when you want work with streaming data.

---
### Kernel SVMs
SVMs can be **kernelized** to solve nonlinear classification problems. An example of a nonlinear problem would be on p143. Here I can't really use 2 hyperplanes to separate the data accurately. Here's the trick:
  1. Create nonlinear combinations of the original feature graph.
  2. Transform that 2D dataset into a 3D data data set (projection to higher dimension). We'd have a mapping function $\phi$ to do this.

The math looks like $\phi(x_{1}, x_{2}) = (z_{1}, z_{2}, z_{3}) = (x_{1}, x_{2}, x^{2}_{1} + x^{2}_{2})$.

On p144 we see this in action. That double circular 2d plot can't be separated with a linear hyperplane. We used $\phi$ to make that 3D graph, and there exists a hyperplane that separates the data. We then train the data in this higher dimensional space. This is computationally expensive, so we use the **kernel trick** to handle that. Define a simple kernel function as:

$K(x^{i}, x^{j}) = \phi(x^{(i)})^{T} \phi(x^{(j)})$

One of the most widely used kernels is **Radial Basis Function (RBF)**, aka the **Gaussian kernel**:
- **Gaussian kernel**: $K(x^{i}, x^{j}) = exp(-\frac{||x^{(i)} - x^{(j)}||^{2}}{2\sigma^{2}}) = exp(-\gamma ||x^{(i)}-x^{(j)}||^{2})$
- **cut-off parameter:** $\gamma = \frac{1}{2\sigma^{2}}$, is the parameter to be optimized. The higher the value, the tighter the decision boundary is. However, because while the model fits the data very well, it's not going to generalize.

- **NOTE:** You can treat the kernel as a **similarity function** for two samples/rows. So it'll output a score between 1 (samples are the same) and 0 (very different samples). On p147 we see us do this.