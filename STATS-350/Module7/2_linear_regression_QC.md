# Quick Check 2: Linear Regression

## Question 1
- X: Predictor
- Y: Response
- $\beta_{1}$: The slope
- $\beta_{0}$: The intercept
- $\hat{\beta}_{1}$: OLS coefficient for slope
- $\hat{\beta}_{0}$: OLS coefficient for intercept
- $\beta_{0}+\beta_{1}x$: Mean function
- $\hat{\beta}_{0}+\hat{\beta}_{1}$: Regression line

## Question 2 
Select all statements that are correct:

- $\hat{\beta}_{1}$ is an unbiased estimator of $\beta_{1}$. This is true, especially when errors have zero mean with constant variance.
- $\hat{\beta}_{1}$ is an unbiased estimator of $\beta_{1}$

But this was wrong meaning we missed something

Here are the false statements:
- "Linear reg is the same as the mean function". Not true, the mean function is the line that has the true parameters. The linear regression is estimates the mean function.
- "$\beta_{1}$ is a linear comb. of $y_{1}, y_{2}, ..., y_{n}$". No, $\beta_{1}$ is a fixed parameter, but unknown. $\hat{\beta}_{1}$ is a linear combination of $y_{i}$'s when $x_{i}$'s are fixed.

Unfortunately this answer was incorrect.

## Question 3
[,1]	eruptions	numeric	Eruption time in mins
[,2]	waiting	numeric	Waiting time to next eruption (in mins)

waiting is the response $Y$

## Question 4 and 5
Obtain a regression line for the faithful dataset, what is the OLS coefficient for the intercept $\hat{\beta}_{0}$?

- You know that eruption is X and waiting is Y. You can actually plot a really easily get the calculations in R. Let's do this manually first, and then we'll do the more concise R function.
  - $\hat{\beta}_{1} = \frac{\sum{(x_{i}-\bar{x})-(y_{i}-\bar{y})}}{\sum{ (x-\bar{x})^{2}}}$
  - $\hat{\beta}_{0} = \bar{y}-\hat{\beta}_{1} * \bar{x}$
```R

# Calculate numbers needed for the OLS coefficients 
# Then calculate hte coefficients themselves
x = faithful$eruptions
y = faithful$waiting
xbar = mean(x)
ybar = mean(y)
betahat1 = sum((x-xbar) * (y-ybar)) / sum((x-xbar)^2) # 10.729
betahat0 = ybar - betahat1 * xbar # 33.474

# You could also do
mod1 = lm(waiting ~ eruptions, data=faithful)
```
- $\hat{\beta}_{1}= 10.729$
- $\hat{\beta}_{0}= 33.474$

## Question 6
Calculate the RSS using the OLS coefficients:

1. Remember that $RSS(\hat{\beta}_{0}, \hat{\beta}_{1}) = \sum_{i=1}^{n}{(y_{i} - \hat{y}_{i})^{2}} = \sum_{i=1}^{n}{(y_{i} - \hat{\beta}_{0} - \hat{\beta}_{1}x_{i})^{2}}$
2. You're going to be doing this calculation with the help of R:
```R
# Basically rearranging the equation to solve for the error term.
ehat = y - (betahat0+betahat1*x) # vector of observed error terms
RSS = sum(ehat^2) # 9443.38

# n value for next problem
length(x) # n = 272
```

## Question 7
Obtain the variance for this regression line?

1. $\hat{\sigma}^{2} = \frac{RSS}{n-2} = \frac{9443.38}{272 - 2} = 34.97$

## Question 8
Using $\hat{\beta}_{0}, \hat{\beta}_{1}$ determine the residual for the third observation.

1. I mean the straight forward way of doing this is to calculate the vector containing all of the error terms. Then indexing the third observation. We've already done this:
```R
ehat[3] # 4.763
```