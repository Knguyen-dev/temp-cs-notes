# Quick Check 3: Inference Linear Regression

## Question 1
Get a regression line for `faithful`. Then test the null hypothesis that the eruption time of the current eruption has no effect on the waiting time of the next eruption. What is the $H_{0}$?

1. Here are the hypotheses
- $H_{0}: \beta_{1} = 0$; This assumes no eruption time. Beta1 is the slope.
- $H_{1}: \beta_{1} \neq 0$


## Question 2

What is the test statistic t?

1. Assume the null hypothesis
2. $t = \frac{\hat{\beta} - k}{se(\hat{\beta}_{1}|X)}$
3. We weren't shown how to calculate standard error, since it's a little complex (when you look it up). So you're supposed to rely on R for now:
```R
# 1. Create predictor, response, and linear model object (mod1)
x = faithful$eruptions
y = faithful$waiting
mod1 = lm(y ~ x, data=faithful)

# 2. Calculate t-statistic
# Assuming H0: There is no effect on eruption and waiting, make k=0
k = 0 
beta1hat = summary(mod1)$coef[2, 1]     # 12.729
se.beta1hat = summary(mod1)$coef[2, 2]  # 0.314
t.stat = (beta1hat - k) / se.beta1hat   # 34.08
```

## Question 3
Test $H_{0}$, the claim that the eruption time has of the current eruption has no effect on the waiting time of the next eruption. 

Let's walk through the R code and procedure:
```R
# 1. Create predictor, response, and mod1 object
x = faithful$eruptions
y = faithful$waiting
mod1 = lm(y ~ x, data=faithful)

# 2. Calculate t-statistic
# Assuming H0: There is no effect on eruption and waiting
k = 0 
beta1hat = summary(mod1)$coef[2, 1]     # 12.729
se.beta1hat = summary(mod1)$coef[2, 2]  # 0.314
t.stat = (beta1hat - k) / se.beta1hat   # 34.08
n = length(x)

# 3. Calculate p-value. It's a two tailed test so the p-value should be 
# P = 2 * P(T >= |34.08|); here it's a t-distribution with n-2 degrees of freedom, so you're summing up two tails.
p_value = 2 * (1-pt(abs(34.08), n-2)) # p = 0
```
- We reject the null hypothesis
- We conclude that the eruption time of the current eruption influences the waiting time to the next eruption.
- the results are statistically significant.


## Question 4, 5, 6
You believe that for every additional minute that eruption lasts, the next eruption increases more than 10 minutes. Do the test and calculate the test statistic.

- Hypotheses:
  - $H_{0}: \beta_{1} \geq 10$. Our default belief
  - $H_{1}: \beta_{1} < 10$

1. Here's the code
```R
x = faithful$eruptions
y = faithful$waiting
mod1 = lm(y ~ x, data=faithful)

# 2. Calculate t-statistic
# Assuming H0: There is 10 minute increase
k = 10 
n = length(faithful$eruptions)
beta1hat = summary(mod1)$coef[2, 1]     
se.beta1hat = summary(mod1)$coef[2, 2]  
t.stat = (beta1hat - k) / se.beta1hat   # 2.318

# 4. Right tailed test
1- pt(q=t.stat, df=n-2) # 0.0105
```

- $t=2.318$
- $p=0.989$
- Since $p > \alpha = 0.01$, we fail to reject the null hypothesis

## Question 7
Get a 99% confidence interval:

```R
# Set up variables
x = faithful$eruptions
y = faithful$waiting
mod1 = lm(y ~ x, data=faithful)
beta1hat = summary(mod1)$coef[2, 1]
se.beta1hat = summary(mod1)$coef[2, 2]

# Calculate the alpha/2 quantile for that t-dist with n-2 degrees 
alpha = 1 = 0.99
n = length(x)
q = qt(1 - (alpha)/2, n-2)

lb = beta1hat - q * se.beta1hat # 9.91
ub = beta1hat + q * se.beta1hat # 11.54
```
- Upper bound is 11.54


## Question 8
Calculate $R^{2}$

1. I mean you could do the calculation by hand, we have it in our notes.
```R
x = faithful$eruptions
y = faithful$waiting
mod1 = lm(y ~ x, data=faithful)

# 0.811, so 81% of the change in waiting time (variation) 
# is explained by the x (eruption time of the current eruption)
r_squared = summary(mod1)$r.sq 
```


## Question 9
Create a residual plot of it:
```R
x = faithful$eruptions
y = faithful$waiting
mod1 = lm(y ~ x, data=faithful)
plot(mod1, which=1)
```
- Two clusters of data.
- Clear patterns.

## Question 10
Let's look at the residual plot
```R
# Create the linear model using the faithful dataset directly
mod1 <- lm(waiting ~ eruptions, data = faithful)

# Create new data frame for prediction (eruption lasting 4.5 minutes)
x_star <- data.frame(eruptions = 4.5)

# Calculate the 92% prediction interval
prediction <- predict(mod1, newdata = x_star, interval = "prediction", level = 0.92)

# Display the full prediction result
print("Full prediction result:")
print(prediction)

# Extract the lower bound
lower_bound <- prediction[1, "lwr"]
print(paste("Lower bound:", round(lower_bound, 3)))
```

- Lower bound = $71.331$