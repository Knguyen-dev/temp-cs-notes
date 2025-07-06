# Inference For Linear Regression

## Infererence For Regression

### Hypothesis Tests
We'll test for claims about each OLS coefficient:
$H_{0}: \beta_{1} = k$
$H_{0}: \beta_{1} \neq k$

The claim is whether it's equal to some number $k$ or not. Assuming $H_{0}$ is true, the test statistic is given by:

$t = \frac{\hat{\beta}_{1} - k}{se(\hat{\beta}_{1} | X)} \sim T_{n-2}$

The test statistic follows a T-distribution with $n-2$ degrees of freedom. Finding the p-value here is similar to how we did it with T and Z tests. Here's how you can do it in R:
```R

mod1 = lm(dheight ~ mheight, data=Heights)
summary(mod1) 

# Output below: 
Residuals:
 Min     1Q Median     3Q    Max 
-7.397 -1.529  0.036  1.492  9.053 

Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)   29.917      1.623    18.4   <2e-16
mheight        0.542      0.026    20.9   <2e-16

Residual standard error: 2.27 on 1373 degrees of freedom
Multiple R-squared:  0.241,    Adjusted R-squared:  0.24 
F-statistic:  435 on 1 and 1373 DF,  p-value: <2e-16
```
The **Coefficients** section gives us:
- $\hat{\beta}_{1} = 0.542$
- $se(\hat{\beta}_{1} | mheight) = 0.026$
- Under the null hypothesis, the test statistic $t=20.9$
- The p-value for a two-tailed test $p < 2 \cdot 10^{-16}$. This means that the p-value is nearly zero, so likely for any significance level ,we'll reject $H_{0}$, and accept $H_{1}$. If that's the case, we'll have to "manually" do things
```R
# 1. Get the mod object

mod1 = lm(dheight ~ mheight, data=Heights)

# 2. Extract numbers for calculating the t-statistic for our OLS coef
# Note: coef is a matrix of coefficients hsown earlier

k = 0                                   # From assuming H0
beta1hat = summary(mod1)$coef[2,1]
se.beta1hat = summary(mod1)$coef[2,2]   # Standard error
t.stat = (beta1hat-k) / se.beta1hat     # All values extracted, calculate t = 20.868

# 3. For a two sided problem, the p value is just the 2 * P(T >= |t|).
# Note the t distribution with n-2 degrees of freedom.
n = dim(Heights[1]) # number of rows in the dataset
2*(1 - pt(abs(t.stat), n-2))


# 4. Let's say the hypotheses were:
# - $H_{0}: \beta_{1} \leq 0.5$
# - $H_{1}: \beta_{1} > 0.5$
# Now let's calculate a t-statistic for a t-distribution of n-2 degrees of freedom 
# that will say something about betahat1
k = 0.5
t.stat = (beta1hat - k) / se.beta1hat # 1.6081
p = 1 - pt(t.stat, n-2)                   # 0.054

# Based on a significance level of alpha = 0.01, we fail to reject H0
```

---
### Confidence Intervals
A confidence interval is within:

$\hat{\beta}_{j} \pm q \times se(\hat{\beta}_{j} | X)$

Again $q$ is the $(1 - \frac{\alpha}{2})$ quantile of a T-distribution with $(n-2)$ degrees of freedom. Let's get a 98% confidence interval on it in R:
```R
confint(mod1, level=0.98)

# Output below:
                1 %     99 %
(Intercept) 26.13860 33.69628
mheight      0.48128  0.60221
```
We are 98% confident that $\beta_{1}$, for all mothers who are one inch taller than a given height, the average height of their daughters will also be higher by about 0.48-0.6 inches. You could also do:

```R
beta1hat = summary(mod1)$coef[2,1]
se.betahat = summary(mod1)$coef[2,2]

# 1. Calculate the quantiles for T-Distribution with n-2 degrees of freedom
# 2. Calculate lower and upper bounds of the confidence interval. 
lb = beta1hat - qt(1 - (0.02/2), n-2) * se.beta1hat # 0.481
ub = beta1hat + qt(1 - (0.02/2), n-2) * se.beta1hat # 0.602
```

## Prediction
If a new observation, $x*$ is observed, the new response is given by:
- $y* = E(Y|X = x*) + e^{*} = \beta_{0}+\beta_{1}x* + e*$
- $\hat{y}^{*} = \hat{\beta}_{0} + \hat{\beta}_{1}x^{*}$

Since we almost never know the population parameters, you'd use regression version. 

This has two sources of variation: First the error term $e^{*}$ and the OLS estimators since they are going to be different for each different sample drawn. Both variations are captured by the standard error of $\hat{y}^{*}$, which is denoted as $se_{y}(\hat{y}^{*} | X=x^{*})$. **We can construct a confidence interval for our prediction itself (Prediction Interval):**

- $\hat{y}^{*} \pm q \times se_{y}(\hat{y}^{*}|X=x^{*})$

Again q is the quantile from a t-distribution with n-2 degrees of freedom. Here's how we can do this in R:
```R
# Create a 97% prediction interval for the daughter's height (Y)
# given that the mother's height (X) is 63

# 1. Store predictor value in dataframe
x_star = data.frame(mheight=63)

# 2. Use predict function
predict(mod1, newdata = x_star, interval="prediction", level=0.97)

# Output for predict(). Given that the mother is 64 inches tall, I predict 
# the daughter to be around 59.122 to 68.973 inches tall.
   fit    lwr    upr
1 64.047 59.122 68.973
```


## Residuals 
- Fitted Value: $\hat{y}_{i} = \hat{\beta}_{0} + \hat{\beta}_{1}x_{i}$. The estimated response, fitted value for the ith observation. 
- Error term: $\hat{e_{i}} = y_{i} - \hat{y}_{i}$. It's the difference betwee estimated and observed. Different from $e_{i} = y_{i} - E(Y|X = x_{i})$

Say we do `plot(mod1, which=1)`. We're creating a residual plot, residuals (Y) vs fitted values (X). This plot is used to check if the model is a good for the data. Can we see patterns in our residual plot?

The plot will also show a red line, called a **lowess curve**. A non-paramettric curve that finds averages in local points in the function. Not that important.

A **nullplot** is a scatterplot in which the only pattern is a horizontal line (slope equa lto 0) and no curvature on that line is present. The red line is the loess smoother, which is a helpful reference for curvature. Not the end all be all, but a good reference. The vertical dispersion should be constant for any level of fitted values, and this is called the assumption of homoskedasticity (constancy of variance).
  - When evaluating homoskedasticity, consider the number of poitns for any interval. It's expected to have more variation if you have more points.
  - If variance is constant, fitted values are about symmetric around the mean and predictor values can be random, and the residual plot (and its contour) may be ellipsed shape.



## Coefficient of Determination
$\sum_{i=1}^{n}{(y_{i}-\bar{y})^{2}} = \sum_{i=1}^{n}{(y_{i}-\hat{y}_{i})^{2}} - \sum_{i=1}^{n}{(\hat{y}_{i}-\bar{y})^{2}}$

This means the total sum of squares can be decomposed to the sums of the regression and the residual sum of squares. Then:

$R^{2} = \frac{\sum_{i=1}^{n}{(\hat{y}_{i}-\bar{y})^{2}}}{\sum_{i=1}^{n}{(y_{i} - \bar{y})^{2}}} = 1-\frac{\sum_{i=1}^{n}{(y_{i}-\hat{y}_{i})^{2}}}{\sum_{i=1}^{n}{(y_{i} - \bar{y})^{2}}}$

Let's look at an example where we calculate this and actually interpret it:
```R
summary(mod1)

Call:
lm(formula = dheight ~ mheight, data = Heights)

Residuals:
   Min     1Q Median     3Q    Max 
-7.397 -1.529  0.036  1.492  9.053

Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)   29.917      1.623    18.4   <2e-16
mheight        0.542      0.026    20.9   <2e-16

Residual standard error: 2.27 on 1373 degrees of freedom
Multiple R-squared:  0.241,    Adjusted R-squared:  0.24 
F-statistic:  435 on 1 and 1373 DF,  p-value: <2e-16
```
Here R-squared is 0.24, which means about 24% of the variation of the daughter's height is explained by the mother's height. Essentially, if this is close to 1, the regression line explains a lot about the variation of the response in relation to the predictor. Else little can be explained.