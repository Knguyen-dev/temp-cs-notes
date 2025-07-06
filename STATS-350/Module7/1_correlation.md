# Correlation 

## Bi-variate Distributions
Study the relationship between $X$ and $Y$, where both  are continuous random variables. To study the association, we use the joint probability structure of the pair $(X, Y)$. Remember that for a single (univariate) random variable $X$, it has a pdf $f : \mathbb{R} \rightarrow \mathbb{r}$, then denoted as $P(X \in [a,b]) = \int_{a}^{b}{f(x)dx}$. So the probability of $X$ lying within that interval $[a, b]$ is given by the area under the curve between $a$ and $b$.

We define the bivariate pdf $f: \mathbb{R}^{2} \rightarrow \mathbb{R}^{2}$ for a pair of random variables as:

$P(X \in [a,b], Y \in [c, d]) = P((X,Y) \in [a,b] \times [c, d]) = \int_{c}^{d}\int_{a}^{b}{f(x,y)dxdy}$

So the probability that $(X,Y)$ lies in the rectangle given by $[a,b] \times [c,d]$ is given by the volume under the graph of $f$ for that rectangle.

How we'll rarely need to look at the graph of a bivariate distribution (3D graph), instead we commonly look at contours, which are a 2D representation of the 3D graph. Kind of like an eagle eye view that allows us to also see that elevation. The contour of $f$ at some level $c$ is the set ${(x,y) \in \mathbb{R}^{2} : f(x,y) = c}$. So it's the set of all $(x,y)$ pairs such that $f(x,y) = c$, all pairs that result in the same f output value.

Visually contours look like ellipses, where we point on that ellipse is a pair $(x,y)$ S.T $f(x,y) = c$ or some $c$ value.

## Contours of Bivariate Normals and Population Correlation Coef.
If $X$ and $Y$ are normal random variables, the contours of their bivariate PDF are concentric ellipses centered at $(E(X), E(Y))$. The shape of the ellipses also provides some info about the relationship between the variables too.

- If the ellipse is very narrow, that's a strong association. Wider circles means wider associations.
- A perfect circle indicates the cour of two independent normal RVs.
- The shape and direction is similar to scatter plots, so you can easily see whether it's a positive or negative association.

### Population Correlation Coefficient
The population correlation coefficient for X and Y, also known as the "population product-moment correlation coefficient" is given by:

$\rho = \rho(X,Y) = E[(\frac{X-\mu_{X}}{\sigma_{X}}) (\frac{Y - \mu_{Y}}{\sigma_{Y}})]$

Now let's talk about some properties of the correlation coefficient:
- $\rho \in [0,1]$. 
  - The closer it is to 1, the stronger the positive linear association. 
  - The closer it is to -1, the stronger the negative linear association
  - If Y is completely determined by X, you'll see that the correlation is 1 or -1.
- If X and Y are independent, then $\rho = 0$, but this isn't true the other way around. So if $\rho=0$ they are uncorrelated, but you can't be sure they're independent.
- If X and Y are normal random variables and $\rho =0$, this implies X and Y are independent.

## Bivariate samples and Pearson's Correlation Coefficient
A bivariate sample is given by $(x_{1}, y_{1}), (x_{2}, y_{2}), ..., (x_{n}, y_{n})$, where each pair $x_{i}, y_{i}$ is draw nindependently from the same bi-variate distribution. A scatter plot is the typical graphical representation of a bivariate sample. You can use the smaple to estimate population parameters about the bi-variate distribution. Here are the estimates that you'll probably see when doing the math: $\mu_{x}, \mu_{y}, \sigma_{x}, \sigma_{y}, \rho, \bar{x}, \bar{y}, s_{x}, s_{y}, \hat{p}$.

Pearson's correlation coefficient, aka Pearson's product-moment correlation coefficient, is the plug-in estimate of $\rho$. It's given by:

$r = \hat{p} = \frac{1}{n-1}\sum_{i=1}^{n}{[\frac{x_{i}-\bar{x}}{s_{x}}\frac{y_{i}-\bar{y}}{s_{y}}]}$

### Example 1: Correlation
```R
# Given the parameters of the population (mu_{x}, mu_{y}, sigma_{x}, sigma_{y}, \rho), give me a sample 
# of 20 (x, y) pairs. The na contour is produced based on this sample that allows us to 
# visualize the association. Note that this isn't a default R functio so you'll have to download it from 
# the author's website.
data1 <- binorm.sample(pop=c(0,0,1,4,-0.9), n=20)
binorm.scatter(data1)

# Calculates pearson correlation coefficient
cor(data1)
```

## Using R to Study Association

### Drawing Ellipses
```R
binorm.ellipse <- function(pop) {
  #
  #  This function plots the concentration ellipse of a
  #  bivariate normal distribution.  The 5 parameters are
  #  specified in the vector pop in the following order: 
  #  mean of X, mean of Y, variance of X, variance of Y, 
  #  correlation of (X,Y).  Example: pop <- c(0,0,1,4,.5)
  #
  ndots <- 628
  m <- matrix(pop[1:2], nrow = 2)
  off <- pop[5] * sqrt(pop[3] * pop[4])
  C <- matrix(c(pop[3], off, off, pop[4]), nrow = 2)
  E <- eigen(C,symmetric=TRUE)
  a <- 0:ndots/100
  X <- cbind(cos(a), sin(a))
  X <- X %*% diag(sqrt(E$values)) %*% t(E$vectors)
  X <- X + matrix(rep(1, ndots + 1), ncol = 1) %*% t(m)
  xmin <- min(X[, 1])
  xmax <- max(X[, 1])
  ymin <- min(X[, 2])
  ymax <- max(X[, 2])
  dif <- max(xmax - xmin, ymax - ymin)
  xlim <- c(m[1] - dif, m[1] + dif)
  ylim <- c(m[2] - dif, m[2] + dif)
  par(pty = "s")
  plot(X,type="l",xlab="x",ylab="y",xlim=xlim,ylim=ylim)
  title("Concentration Ellipse")
}


binorm.sample <- function(pop,n) {
  #
  #  This function returns a sample of n observations drawn
  #  from a bivariate normal distribution.  The 5 parameters
  #  are specified in the vector pop in the following order: 
  #  mean of X, mean of Y, variance of X, variance of Y, 
  #  correlation of (X,Y).  Example: pop <- c(0,0,1,4,.5)
  #  The sample is returned as an n-by-2 data matrix, 
  #  each row of which is an observed value of (X,Y).
  #
  m <- matrix(pop[1:2], nrow = 2)
  off <- pop[5] * sqrt(pop[3] * pop[4])
  C <- matrix(c(pop[3], off, off, pop[4]), nrow = 2)
  E <- eigen(C,symmetric=TRUE)
  Data <- matrix(rnorm(2 * n), nrow = n)
  Data <- Data %*% diag(sqrt(E$values)) %*% t(E$vectors)
  return(Data + matrix(rep(1, n), nrow = n) %*% t(m))
}


binorm.estimate <- function(Data) {
  #
  #  This function estimates bivariate normal parameters.
  #  Each row of the n-by-2 matrix Data contains a single
  #  observation of (X,Y).  The function returns a vector 
  #  of 5 estimates: mean of X, mean of Y, variance of X, 
  #  variance of Y, correlation of (X,Y). 
  #
  n <- nrow(Data)
  m <- c(sum(Data[, 1]), sum(Data[, 2]))/n
  v <- c(var(Data[, 1]), var(Data[, 2]))
  z1 <- (Data[, 1] - m[1])/sqrt(v[1])
  z2 <- (Data[, 2] - m[2])/sqrt(v[2])
  r <- sum(z1 * z2)/(n - 1)
  return(c(m,v,r))
}


binorm.scatter <- function(Data) {
  #
  #  This function plots a scatter diagram of the data
  #  contained in the n-by-2 data matrix Data.  It also
  #  superimposes the sample concentration ellipse.
  #
  ndots <- 628
  xmin <- min(Data[, 1])
  xmax <- max(Data[, 1])
  xmid <- (xmin + xmax)/2
  ymin <- min(Data[, 2])
  ymax <- max(Data[, 2])
  ymid <- (ymin + ymax)/2
  dif <- max(xmax - xmin, ymax - ymin)/2
  xlim <- c(xmid - dif, xmid + dif)
  ylim <- c(ymid - dif, ymid + dif)
  par(pty = "s")
  plot(Data,xlab="x",ylab="y",xlim=xlim,ylim=ylim,
       pch=".",cex=2)
  #
  #  Value of cex sets size of plotting symbol.
  #
  title("Scatter Diagram")
  v <- binorm.estimate(Data)
  m <- matrix(v[1:2], nrow = 2)
  off <- v[5] * sqrt(v[3] * v[4])
  C <- matrix(c(v[3], off, off, v[4]), nrow = 2)
  E <- eigen(C,symmetric=TRUE)
  a <- 1:ndots/100
  Y <- cbind(cos(a), sin(a))
  Y <- Y %*% diag(sqrt(E$values)) %*% t(E$vectors)
  Y <- Y + matrix(rep(1, ndots), nrow = ndots) %*% t(m)
  lines(Y)
}


binorm.regress <- function(Data) {
  #
  #  This function plots a scatter diagram of the data
  #  contained in the n-by-2 data matrix Data.  It also
  #  superimposes the sample concentration ellipse and 
  #  the regression line.
  #
  ndots <- 628
  xmin <- min(Data[, 1])
  xmax <- max(Data[, 1])
  xmid <- (xmin + xmax)/2
  ymin <- min(Data[, 2])
  ymax <- max(Data[, 2])
  ymid <- (ymin + ymax)/2
  dif <- max(xmax - xmin, ymax - ymin)/2
  xlim <- c(xmid - dif, xmid + dif)
  ylim <- c(ymid - dif, ymid + dif)
  par(pty = "s")
  plot(Data,xlab="x",ylab="y",xlim=xlim,ylim=ylim,
       pch=".",cex=2)
  #
  #  Value of cex sets size of plotting symbol.
  #
  title("Regression Line")
  v <- binorm.estimate(Data)
  m <- matrix(v[1:2], nrow = 2)
  off <- v[5] * sqrt(v[3] * v[4])
  C <- matrix(c(v[3], off, off, v[4]), nrow = 2)
  E <- eigen(C,symmetric=TRUE)
  a <- 0:ndots/100
  Y <- cbind(cos(a), sin(a))
  Y <- Y %*% diag(sqrt(E$values)) %*% t(E$vectors)
  Y <- Y + matrix(rep(1, ndots + 1), ncol = 1) %*% t(m)
  lines(Y)
  x <- xlim[1] + (2 * dif * (0:ndots))/ndots
  slope <- v[5] * sqrt(v[4]/v[3])
  y <- v[2] + slope * (x - v[1])
  Y <- cbind(x, y)
  Y <- Y[Y[, 2] < ymax,  ]
  Y <- Y[Y[, 2] > ymin,  ]
  lines(Y)
}


binorm.resid <- function(Data) {
  #
  #  This function computes residuals from simple linear
  #    regression.  The n-by-2 matrix Data contains x in 
  #    the first column and y in the second column.
  #  It returns an n-by-2 data matrix that contains x in
  #    the first column and the residual errors in the 
  #    second column.
  #
  v <- binorm.estimate(Data)
  slope <- v[5] * sqrt(v[4]/v[3])
  yhat <- v[2] + slope * (Data[,1] - v[1])
  resid <- Data[,2] - yhat
  return(cbind(Data[,1],resid))
}


# Drawing 3 ellipses
binorm.ellipse(pop = c(0, 0, 1, 1, 0.95))
binorm.ellipse(pop = c(0, 0, 1, 1, 0))
binorm.ellipse(pop = c(0, 0, 1, 1, -0.5))
```

### Scatter plots and ellipses from simulated data
From the previous section, we created plots from known bi-variate normal distributions.
```r
# 1. Obtain 1000 observations from a bi-variate dist. of two indep. standard norm. RVs (\rho = 0)
# 2. Use rnorm to get random observations from standard norm.
# 3. Use Trosset's function binorm.scatter() to create a scatter plot
x = rnorm(1000)
y = rnorm(1000)
cor(x,y) # -0.0159
binorm.scatter(cbind(x,y))
```
Pearson's correlation coefficient $r=-0.016$ is very close to 0. There's no particular pattern, and the contour reinforces this as it's pretty much a circle, showing a lack of association. You

### Example: BGS Girls' Height and Weight 
Let's study teh relationship between the heights and weights when children turn 18
```R
install.packages("alr4")
data1 = alr4::BGSall
weight = data1$WT18
height = data1$HT18

# Weight almost seems normal, but there are noticeable deviations of from the line at the tails.
# Potential outliers at upper tail.
qqnorm(weight)
qqline(weight)

# Height seems really approx. normal; slight deviations.
qqnorm(height)
qqline(height)

# Scatter plot seems to have a positive linear association between weight and height
# Note: We'll use Trosset's binorm.scatter function to find the contour for this data
plot(weight, height)

# Contour seems to reinforce a positive linear association.
binorm.scatter(cbind(weight, height))

# Then calculate the Pearson's correlation coefficient. We do this manually first by calculating 
# other params:
xbar_weight = mean(weight)
ybar_height = mean(height)
s_x = sd(weight)
s_y = sd(height)
n = length(weight)

# just treat z as the things that need to be multiplied
z_x = (weight - xbar_weight) / s_x
z_y = (height - ybar_height) / s_y

r = sum(z_x * z_y) / (n-1) # 0.6235
```
