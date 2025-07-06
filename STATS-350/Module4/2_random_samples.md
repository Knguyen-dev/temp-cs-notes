# Section 1: Random Samples 

### Pseudo-random vs random samples
In R we generate pseudo-random samples because in reality the random numbers are chosen by an algorithm. However, it still maintains the properties of random samples, so from now on we'll call/treat pseudo random samples as random samples.

In programming, whenever we're dealing with random numbers, we'd like to be able to re-run our code and make sure we get the same answers, for testing purposes. To do this, set add a random seed to our code with something like `set.seed(<some-numbe>)`.

```R
# Generates 10 random numbers from a normal dist. 
rnorm(10)

set.seed(123)

# Will produce the sample 10 numbers
rnorm(10)
```

### Random Samples using the normal distribution
```R
# Generate 10 from a standardized normal dist. N(0,1)
z <- rnorm(30)

# Generate 50 numbers from a normal dist. with mean 10 and std-dev 5
y <- rnorm(n=50, mean=10, sd=5)
```

### Random samples using the uniform distribution
The name for a uniform distribution is `unif`, and so you can imagine its functions are `dunif()`, `punif()`, etc.

```R
# Generate 10 numbers from a uniform dist. with min 0 and max = 1
x <- runif(n=10, min=0, max=1)
```

### Random Samples Using Binomial Distribution
```R
# Generate a random sample of size 30 from a Binomial(20, 0.9)
x <- rbinom(30, 20, 0.9)
```

### Random Sample From Existing Data
Some pre-requisite steps:
- Load ggplot2
- Create a vector containing the values of sleep_total

```R
library(ggplot2)
vec <- msleep$sleep_total

# A random sample of 30 values from the vector, and when we take a value we'll replace it. 
sleep_sample <- sample(x=vec, size=30, replace=TRUE);

# Then you can do math on those
mean(sleep_sample) # output: 12.11
```

# Section 2: Introduction to Simulations
Simulations are computer-based representations of real life experiments, which allow us to study the patterns an experiment produces when we perform it many times (replication). Here's how we setup a simulation in R:
- Define the experiment:
  - Roll a die twice and add the numbers observed.
  - Toss a coin five times and count the number of heads.
  - Take two tickets from an urn and subtract the second from the first.
  - Obtain a random sample form a vector of values and obtain the sample mean.
- Write the R code to repeat the experiment through replication, and store the results of those replications.

Let's consider drawing three tickets, with replacement, from an urn and then calculating the product of those three tickets:

$urn=\{ 3, 5, 5, 6, 6, 6, 6, 7, 7, 7 \}$

Let's see the code for this:
```R

urn <- c(3, 5, 5, 6, 6, 6, 6, 7, 7, 7)

# Get a random sample of 3 numbers from the urn with replacement.
# Then calculate the product. Okay that's one experiment, but 
# now we need to scale this up to many. An example result is 210.
prod(sample(x=urn,size=3,replace=TRUE))

# Use the replicate(n, expr) function where you pass the number of times you want to run the simulation and then a callback function.Then you store the results of each of those simulations in a vector.

# Now the vecot conatins 1000 values (1000 samples), each one being the product of 3 tickets that were randomly chosen from the urn.
simulation_results <- replicate(n=1000, expr=prod(sample(urn, 3, TRUE)))
```
Let's look at one more example that talks about the mean of one sample, and then the mean of all samples:

```R
# my_vec = [1, 2, ..., 20]
my_vec = 1:20
set.seed(123)


# Get a random sample of 30 numbers from the vector and calculate the mean of that sample (sample mean)
mean(sample(x=my_vec, n=30, replace=TRUE))

# Let's repeat this a large number of times, take many samples.
# So now we have a sample of 15 sample means.
sample_means <- replicate(n=15, expr=mean(sample(x=my_vec, n=30, replace=TRUE)))

```

# Section 3: Functions in R
You can create your own custom functions in R:
```R
# Ex. 1Create the function using function() in R
diff_fn <- function(x,y){x-y}
diff_fn(5,3) # output: 2, you may also use kwargs

set.seed(1) # However, remember we can also get the difference of vectors
a <- rbinom(10, 20, 0.8)
b <- rbinom(10, 20, 0.2)
diff_fn(a,b)

# Ex.2: Creating a function that produces the plug-in estimate of the standard deviation. It accepts a vector.
plugin_sd <- function(x){
  mu_hat = mean(x)
  sigma_sq_hat = mean(x^2) - mean(x)^2
  sigma_hat = sqrt(sigma_sq_hat)
  return(sigma_hat)
}
plugin_sd(c(1,2,4,9)) # Then using it on any vector
```
A replication could involve taking a random sample of 20 tickets with replacement. Then we calculate the plug-in standard deviation of the sample. Then let's do 100 samples:
```R
set.seed(2)
urn <- c(3, 5, 5, 6, 6, 6, 6, 7, 7, 7)
vector_plugin_sd = replicate(100, plugin_sd(sample(urn,20,TRUE)))

# 
head(vector_plugin_sd)

```


# Quick Check:

### Q1
Obtain a random sample of size 50 from a normal distribution with mean 10 and variance 25. What is the 0.7-quantile of your sample. Answer with 3 decimals. 
```R
# 1. Obtains a random sample of size 50 from a norm. dist. with mean 10 and variance 25
# 2. Finding the 0.7-quantile of the sample? Well you could just do 
sample <- rnorm(n=50, mean=10, sd=5)
quantile(sample, probs=0.7)
```

### Q2
Use the random seed 1234 before obtaining your sample. Then, obtain a random sample of 25 observations, with replacement, from a population distributed as a binomial(5,0.8). What is the fifth observation in your sample?
```R
set.seed(1234)
sample <- rbinom(n=25, size=5, prob=0.8)
sample[5]
```

### Q3
```R
set.seed(1)
urn <- rnorm(10, 0, 1)
x <- sample(x = urn, size = 10, replace = FALSE)
y <- sample(x = urn, size = 10, replace = FALSE)
IQR(x) - IQR(y)
IQR(x-y)
```
Explain why `IQR(x-y) != 0`, and why `IQR(x)-IQR(y) == 0`. The vectors do contain the same data, but ordered differently. As a result, we can say their IQR values are equal, the order of the data in teh vector doesn't affect that. However they aren't the same vector. The seed ensures reproducibility, these are two independent samples of each other so they aren't identical. Even if the urns were bigger, they would still have the same values. Remember we aren't replacing, so we're just picking every number in the urn. However if we did use replacement, we would have different results. The outcome with the difference in IQR is just the nature of the seed. If you change the seed, the values in `urn` will change, and then the IQRs of the two samples could be different. So here's the correct statements:
- Vectors x and y are the same vector because we have used a seed.
- Both outcomes would be different than zero if we use `replace=TRUE`, when making the samples.
- If the urn was bigger say 100, the IQR could be different since the vectors could be different. The only reason we were getting the same answers before is because there are 10 options, and regardless both samples choose all 10 options. However if there are 100 options, well we have some flexibility.

### Q4
Create a function called range80 that uses as argument a vector of numbers and calculates the difference of the 0.9-quantile minus the 0.1-quantile of this vector. Apply your function to the following vector:

```R
vec <- c(7.73, 7.17, 18.79, 9.21, 15.37, 10.73, 12.90, 12.42, 13.92,
         5.34, 6.85, 14.05, 9.63, 6.85, 4.75, 10.25, 17.33, 15.50,
         11.18, 9.95, 12.93, 8.79, 14.59, 17.17, 11.23)
range80 <- function(x) {
  a = quantile(x, probs=0.9)
  b = quantile(x, probs=0.1)
  return(a-b)
}
range80(vec) # 9.652
```

### Q5
This question uses the function `range80()` that you created in the previous question. Assume that your population can be described by $X$ and $X∼binomial(20,0.4)$. You are asked to run the following simulation:
- The experiment consists on getting the `range80` from a random sample of 50 observations from this population.
- Generate $10,000$ replications and store them in a vector.
- Obtain and report the mean of your vector of replications. Use two decimals.
```R
# Question 5
# 1. Calculate range80 for one sample of 50 observations
# 2. Create 10,000 replicas of the experiment
samples = replicate(n=10000, expr={
    sample_data = rbinom(n=50, size=20, prob = 0.4)
    return(range80(sample_data))
  }
)
mean(samples)
```