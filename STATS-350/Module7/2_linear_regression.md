# Linear Regression

## The Linear Model

### The Mean and Variance Functions
Let $Y$ be the response and $X$ be the predictor. Given $X=x$ (X takes on some value), $Y$ will take on some values, denoted as $(Y | X=x)$. That expression is a random variable, remember that. We assume there's a linear relationship between $Y$ and $X$ if the $E(Y|X=x)$ can be represented by: 
- **Mean function:** $E(Y|X=x) = \beta_{0} + \beta_{1}x$. Where $\beta_{0}$ (intercept) and $\beta_{1}$ (slope). It is the true/ideal relationship between X and Y. Its parameters are typically unknown, so you typically don't know the true relationship and can't make perfect predictions.
  - $E(Y|X=x_{i})$: The **true expected value** of $Y$ for a given $X=x_{i}$. If you could gather an infinite number of houses that all had the exact same square footage $x_{i}$, then $E(Y|X=x_{i})$ would be the average price of those houses. So it's the average value of $Y$ when $X=x_{i}$. Again $E(Y|X=x_{i})$ is a random variable, so can deviate from the expected value! 
- **Variance function:** $Var(Y|X=x) = \sigma^{2}$. Since $(Y|X=x)$ is a random variable, there'll be some variation in the observed values. In our model, we'll assume this variation is constant regardless for any $X=x$.

The **goal** is get a random sample, and use it to estimate the parameters of the mean and variance functions. In linear regression, we're dealing with scatter plots, 2 different axes (X and Y) coordinates. A random sample would be pairs of $(y_{i}, x_{i})$. Here's some math for the **linear model**:

- $y_{i} = E(Y|X=x_{i}) + e_{i} = \beta_{0} + \beta_{1}x_{i} + e_{i}$

  - Again, $y_{i}$ is the actual observed value of the dependent variable for the $i$-th data point. Whilst your mean function is the true relationship, your linear model is just some theoretical framework/setup that describes how individual data points are generated. It claims that each observed $y_{i}$ is a combination of the true underlying mean plus some random error. It isn't a "line" that you can draw on a scatter plot, but an idea.
  - $e_{i}$: The true error term calculated by $e_{i} = y_{i} - \hat{y}_{i}$. It's a random variable that represents the deviation of the actual observed value ($y_{i}$) from the true expected value $(E(Y|X=x_{i}))$. In general $e_{i}$ can exist due to factors that influence $Y$ that aren't included in our model (e.g. neighborhood quality, specific renovations, etc. beyond just square footage). 
  - $\hat{y}_{i}$: This is our model's prediction or estimated expected value that was calculated using estimated regression coefficients ($\hat{\beta}_{0}, \hat{\beta}_{1}$). The equation to calculate this is: $\hat{y}_{i} = \hat{\beta}_{0}+\hat{\beta}_{1}x_{i}$. Essentially, $\hat{y}_{i}$ is our best guess for $E(Y|X=x_{i})$ based on the data we got.
  
  - $y_{i}$ is also normally distributed denoted as $y_{i} \sim N(\beta_{0} + \beta_{1}x_{i}, \sigma^{2})$.

Once data is observed, you can calculate error $e_{i} = y_{i} - \hat{y}_{i}$, where $\hat{y}_{i}$ is our model's prediction, an estimate of the expression above. In our model we assume:
- The expected error $E(e_{i}) = 0$ and $Var(e_{i}) = \sigma^{2}$.
- $e_{i},e_{j}$ are pairwise independent and in turn $y_{i},y_{j}$ are pairwise independent for all $i \neq j$.

Note the difference between the mean function and the regression line:
- **The Mean function:** Talks about some true line constructed using population parameters $\beta_{0}, \beta_{1}$. These parameters are typically unknown and they result in what we view as the expected/actual values for $Y | X=x_{i}$. Usually it's not possible to get the parameters for this line, so we're typically we're trying to estimate. 
- **The regression line:** A line constructed from the data that we collected, which has parameters $\hat{\beta}_{0}, \hat{\beta}_{1}$. Again, the idea of having a sample to estimate population parameters. Same as always.

### Simulation - Constructing Samples 
We assume our linear model is a decent representation of the relationship between Y and X and that there exist parameters $\beta_{0}, \beta_{1}$ and $\sigma$ that best represent the relationship. Though they are unknown to us. Simulate $n=50$ observations for the X values. Let $\beta_{0}=10, \beta_{1}=1.5$ and $\sigma^{2} =100$. Now we'll construct what the response Y values should look like.  

```R
set.seed(1)

beta0 = 10
beta1 = 1.5
sigam2 = 100
n=50
x = sample(50:100, n, replace=TRUE)

# Generate some error terms that we'll draw from a normal distribution
e = rnorm(n, mean=0, sd=sqrt(sigma2)) 
y = beta0+beta1*x+e

# Dataframe contains 50 pairs (x,y)
data.sim = data.frame(x=x, y=y)
```
In most real life situations, those parameters are unknown, so we need to somehow estimate those paramters. In general, your linear regression problems will revolve around calculating $\beta_{0}, \beta_{1}, \sigma^{2}$.

## The Least Squares
The goal of linear regression is to create estimators for those unknown coefficients for the intercept $(\beta_{0})$ and slope ($\beta_{1}$). The idea is to estimate these values based on a criteria. That criterion we'll use is called **Least Squares**. Define a **residual** for the ith observation, $\hat{e}_{i}$, as the vertical distance between the observed response, $y_{i}$, and the y-axis value of the line for $x_{i}$. Denoted as: 

$\hat{e}_{i} = y_{i}-b_{0}-b_{i}x_{i}$. 

The idea is that you get the residuals for all observations, square them (avoids negatives or positive nonsense), and them sum them all up. The result is called the Residual Sum of Squares (RSS) for the line with intercept $b_{0}$ and slope $b_{1}$:

$RSS(b_{0}, b_{1}) = \sum_{i=1}^{n}{(y_{i} - b_{0} -b_{1}x_{i})^{2}}$

The idea is that the smaller the RSS values for the line you have, the more accurate it is. In the end, the idea is to find the best line, the line that minimizes the residual sum of squares. The parameters/solution that results in this line are called the **Ordinary Least Squares (OLS)**, where you have $\hat{\beta}_{0}, \hat{\beta}_{1}$ which minimize the RSS. The line that uses the OLS line is called the least squares regression line:
  - $\hat{\beta}_{0} = \bar{y} -\hat{\beta}_{1}\bar{x}$
    - $\hat{\beta}_{0} \sim N(\beta_{0}, \sigma^{2} \cdot \frac{\sum{x_{j}^{2}}}{n \cdot \sum{(x_{i} - \bar{x})^{2}}})$

  - $\hat{\beta}_{1} = \frac{\sum_{i=1}^{n}{(x_{i}-\bar{x}) (y_{i}-\bar{y})}}{\sum_{i=1}^{n}{(x_{i}-\bar{x})^{2}}}$
    - $\hat{\beta}_{1} \sim N(\beta_{1}, \sigma^{2} \cdot \frac{1}{\sum{(x_{i} - \bar{x})^{2}}})$

  - $\bar{x} = \frac{1}{n}\sum_{i=1}^{n}{x_{i}}$
  
  - $\bar{y} = \frac{1}{n}\sum_{i=1}^{n}{y_{i}}$

  - $\hat{\sigma}^{2} = \frac{RSS}{n-2}$: The variance estimator, also known as the residual mean square. Here $RSS=RSS(\hat{\beta}_{0}, \hat{\beta}_{1})$ is the RSS obtained by using the OLS coefficient estimators. It follows a distribution of a chi-squared distribution with $n-2$ degrees of freedom. Also denoted as: $\hat{\sigma}^{2} \sim [\frac{\sigma^{2}}{n-2}]X^{2}_{n-2}$


### Example: OLS Coefficients with our Heights
```R

# 1. Calculate OLS coefficients
x = Heights$mheight
y = Heights$dheight
xbar = mean(x)
ybar = mean(y)
betahat1 = sum((x - xbar) * (y-ybar)) / sum((x-xbar)^2)
betahat0 = ybar - betahat1 * xbar

# 2. use lm(y ~ x) to get OLS estimates and related info:
# Remember dheight is the response whilst mheight is the predictor
# Note: The mod1 object is very useful and contains a lot of statistics 
# parameters and statistics that we'll use later. 
mod1 = lm(dheight ~ mheight, data = heights)
coef(mod1) # outputs OLS coefficients 

# (Intercept)     mheight 
#    29.91744     0.54175 

# Basically coef but gives some more data as well. This will be very useful.
summary(mod1)

# Plot the scatterplot with plot(y ~ x, dataset) and the line
plot(dheight ~ mheight, Heights)
abline(mod1, col="blue")

# 1. Extracting sigmahat (standard error of the regression) from mod1
# 2. Calculating sigmahat using an equation
sigma(mod1) # 2.26

# Calculate RSS
# 1. Rearranging equation to calculate ehat
ehat = y - (betahat0 + betahat1*x) 
RSS = sum(ehat^2) # 7052

# Then we use what we found from RSS to calculate sigmahat
n <- length(x)
sigmahat_sq <- RSS/(n - 2)
sigmahat <- sqrt(sigmahat_sq)
```
For different samples, you're going to get different values for $\hat{\beta}_{0}, \hat{\beta}_{1}$. You're going to get different OLS estimates. One line will best fit one sample whilst another line will best fit another sample. 

## OLS Coefficients
Let's talk about **interpretations of OLS coefficients:**
- **Intercept:** Intercept of the regression line, $\hat{\beta}_{0}$ is the estimated value of the mean response when the predictor is 0. This interpretation only makes sense if the range of values for the predictor includes 0, else there's no meaningful interpretation because it's not possible to get that y-intercept as an output. Below since the mother's height doesn't include 0, the intercept has no meaningful interpretation.
```R
summary(Heights$mheight)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
   55.4    60.8    62.4    62.5    63.9    70.8 
```
- **Slope:** The slope $V$ is the expected/average rate of change of the response when the predictor increases by one unit. For the heights example, $\hat{\beta}_{1}=0.54$. We can describe it in two ways:
  - If the mother is one inch taller than some given height, we expect her daughter to be 0.54 inches taller.
  - On average, the daughters of mothers who are one inch taller than some given height would be $0.54$ inches taller.

Let's talk about **properties of OLS coefficients:** 
- You can show that $\hat{\beta}_{0},\hat{\beta}_{1}$ can be expressed as linear combinations of $y_{1},y_{2},...,y_{n}$.
  - Because $y_{i}=(Y|X=x_{i})$, they are linear combinations of random variables.
- OLS estimators are unbiased meaning $E(\hat{\beta}_{0} | X=x) = \beta_{0}, E(\hat{\beta}_{1} | X=x) = \beta_{1}$. Also $E(\hat{\sigma}^{2} | X=x) = \sigma^{2}$.