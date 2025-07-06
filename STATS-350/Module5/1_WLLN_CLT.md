# Weak Law of Large Numbers (WLLN) and Central Limit Theorem (CLT)

## The Sum and Average of a Random Sample
Let $X_{1}, X_{2}, ..., X_{n}$ be I.I.D model $\mathbb{P}$ be a random sample with $E(X_{i}) = \mu$ and $Var(X_{i}) = \sigma^{2}$ for all $i$. Define some variables:
- Sum of the random sample: $Y = \sum_{i=1}^{n}{X_{i}}$ 
- Sample Mean: $\bar{X}_{n} = \frac{1}{n}\sum_{i=1}^{n}{X_{i}}$

We perform $n$ experiments, 10 different dice rolls. Then $Y$ is the sum of the dice rolls from our 10 experiments. Then $\bar{X}$ is sums everything up and divides it (arithmetic mean), so it finds the average dice roll value that we calculated from the sample. Note that $\bar{X}_{n}$ and $\bar{x}$ are both called the sample mean. With the former being a random variable, and the latter being an observed value.

Using the properties of expected value and variance, we can calculate these properties:
- $E(Y) = E(\sum_{i=1}^{n}{X_{i}}) = \sum_{i=1}^{n}{E(X_{i})} = \sum_{i=1}^{n}{\mu} = n\mu$

- $Var(Y) = Var(\sum_{i=1}^{n}{X_{i}}) = \sum_{i=1}^{n}{Var(X_{i})} = n\sigma^{2}$

- $E(\bar{X}_{n}) = E(\sum_{i=1}^{n}\frac{X_{i}}{n}) = \frac{1}{n}\sum_{i=1}^{n}{E(X_{i})} = \mu$

- $Var(\bar{X}_{n}) = Var(\sum_{i=1}^{n}{\frac{X_{i}}{n}}) = \frac{1}{n^{2}}\sum_{i=1}^{n}{Var(X_{i})} = \frac{1}{n^{2}}n\sigma^{2}= \frac{\sigma^{2}}{n}$

```R
# *** Sum and average of random sample ***

# 1. Create vector that represents population data
N <- 10^4
set.seed(101)
vec <- c(rnorm(N, 10, 3), rnorm(N, 2, 1))
mu <- mean(vec) # So this is our population data

# I'm assuming this is from a variance formula?
sigma.sq <- mean(vec^2) - mean(vec)^2
c(mu, sigma.sq)
plot(denstiy(vec))

# 2. Estimate expected value and variance of our vector

n <- 30
set.seed(2)
Y <- replicate(10^5, sum(sample(vec, 30, replace=TRUE)))
mean(Y)

mu * 30 # should be somewhat close to the mean(Y)

mean(Y^2) - mean(Y)^2
sigma.sq*30 # Should be somewhat close to Var(Y)

# Estimate the expected value and variacne of xbar. 
# So we have a vector of sample means. Then we're going to 
# to find the expected value of that vector.
Xbar <- replicate(10^5, mean(sample(vec, 30, replace=T)))

mean(Xbar) # Mean of your sample means
mean(Xbar^2) - mean(Xbar)^2 # Variance of your sample means
```

## Law of Large Numbers (Refined)

### Intuitive and Informal
Let's say we have a random variable $X$. If we take a sample of $n$ observations of $X$, and we calculate the mean $\bar{X}_{n}$. It tells us our sample mean $\bar{X}_{n} \to E(X) = \mu$ as $n \to \infty$. If you take a large enough sample, you're going to end up getting or at least getting close to the population mean.

Let X = # of heads after 100 tosses of a fair coin (1 RV). We know the expected value $E(X) = 100(0.5) = 50$. So you do may trials $\bar{X}_{n} = \frac{55+65+...+n}{n}$. But the idea is that $\bar{X}_{n}$ as $n \to \infty$. If you increase that sample size, the samplem ean will approach that theoretical population mean.

The idea is that as you do more observations, the average of all of your observations (sample mean) is going to converge to the expected value (theoretical, population mean) of the random variable.

### Weak Law of Large Numbers Technical
Let $X_{1}, X_{2}, ..., X_{n} \sim \mathbb{P} $ (I.I.D) with $E(X_{i}) = \mu$ and $Var(X_{i}) = \sigma^{2}$ for all $i$. Now we can define:

- $\bar{X}_{n} \rightarrow \mu$, meaning $\bar{X}_{n}$ converges in probability to $\mu$ as $n \rightarrow \infty$.
- This means that there's an $\epsilon$ such that $P(\bar{X}_{n} \in (\mu-\epsilon, \mu+\epsilon)) \rightarrow 1$ as $n\rightarrow \infty$.

In english, as your sample size grows larger, the chances of the sample mean and mean of a given variable having a big difference approaches zero.

It's this idea of "convergence on probability". It's saying that the probability/chances of the sample mean $\bar{X}_{n}$ being close to the population mean $\mu$ will increase as sample size $n \to \infty$.
As we average more and more $X_{i}$, the average values  that we observe tends to be distributed closer and closer to the theoretical average of $X_{i}$

### Corollary: Law of Averages
So we have a couple of $X_{i} \sim Bernoulli(p)$. Then $\bar{X}_{n}$ is the observed frequency in which A occurs within n trials. Here are some other pieces of info:
  - $\mu = E(X_{i}) = p = P(A)$ is the theoretical probability of A. If you toss a fair coin 10 times, your sample mean could be $\bar{X}_{10} = 0.9$. However if you know that the coin is indeed fair ($p=0.5$), then we would remain confident that 
  - The law states that the expected value of the random variables converges to its theoretical probability $P(A)$ as we do more trials.

### Example: Weak Law of Large Numbers in R
```r
num_samples = 10^4
x2 = c(rnorm(num_samples, 10, 3), rnorm(num_samples, 2, 1)) # population isn't normal
plot(density(x2))
# We can create a function that calculates the proportion of sample means 
# that are close to the population mean. Remember x is our population data. 
#   1. Get many N random samples from the population, each one of size n.
#   2. For each random sample, calculate the mean of that sample, and store it in the vector xbar.vec
#   3. Calculate lower and upper bounds
#   4. Calculation the proportion of samples that are within the interval. Then print it out.
wlln = function(x, n, N=num_samples, epsilon=0.1) {
  xbar.vec = replicate(N, mean(sample(x, n, replace=T)))
  # Calculate the upper and lower bounds
  lb = mean(x) - epsilon
  ub = mean(x) + epsilon
  # How many of those samples are within the interval?
  prob = mean(xbar.vec >= lb & xbar.vec <= ub)
  print(data.frame(n=n, probability=round(prob, 2)))
}
wlln(x=x2, n=10)   # Only 0.05 of the samples have a mean within pop mean
wlln(x=x2, n=10^2) # Only 0.17 of the samples are near the population mean
wlln(x=x2, n=10^3) # Only 0.52, but we're increasing.
wlln(x=x2, n=10^4) # 0.97 of the samples have a mean near the population mean.

# We fixed the number of samples, but we let sample size vary. Notice 
# that has sample size increases, chances of our observations 
# deviating largely away from the population mean is lower. It's almost 
# as if with a larger sample size, things average out!
```


## Central Limit Theorem (CLT)

### Central Limit Theorem Intuition and Informal
With any distribution that has a well-defined mean and standard deviation. Instead of taking single observations of the random variable, we'll take samples and calculate the average. Let's say we're taking values from $X$, which is a discrete and non-normal distribution.

An example is taking samples of size $n=4$:
  - $s_{1} = {1,1, 3 6}$ and $\bar{x}_{1}$
  - $s_{2} = {3,4, 3 2}$ and $\bar{x}_{2}$
  - The idea of the sampling distribution. We keep taking these samples, keep calculating the means, and then the distribution of sample means should eventually become approximately normal. This is the core of the central limit theorem.

Now that was for a sampling distribution with sample size $n=4$. Now if you do it for $n=20$ your standard deviation for your distribution is going to decrease and become more normal. So as $n$ gets larger, the sampling distribution sample mean/sum/etc will become more normal. Though the rule of thumb is usually $n \geq 30$ for you to make this justification.

### Central Limit Theorem: CLT 
WLLN states that the distribution of values in the sample mean converges to the population mean as the sample size increases. Then the CLT answers questions about how quickly it does this, and questions about how the sample mean's distribution changes as it tends towards the population mean.

Let $X_{1}, X_{2}, ..., X_{n} \sim \mathbb{P}$ (I.I.D) with $E(X_{i}) = \mu$ and $Var(X_{i}) = \sigma^{2}$ for all $i$. We can define these:
- $Z_{n} = \frac{\bar{X}_{n} -\mu}{ \sigma / \sqrt{n}}$
- $Z \sim Normal(\mu=0, \sigma^{2}=1)$
- $F_{n}(z) \rightarrow \phi(z)$ as $n \rightarrow \infty$ Here $F_{n}$ is the CDF of $Z_{n}$, and $\phi$ is just the CDF of $Z$. The CLT states that for any real number $z$, the above expression is true.

And the only conditions for this to be true is to have I.I.D observations, and for the size of the sample to be large enough. Typically $n \geq 30$.

### Example with CLT
```r
# use this function to see the distribution of sample means
clt = function(x,n,N=10^4){
  xbar.vec = replicate(N, mean(sample(x,n,replace=T)))
  op = par(mfrow=c(1,3))
  hist(xbar.vec, breaks = 100, 
       xlim = c(min(x), max(x)),
       xlab = paste("Sample mean (Xbar_n)"))
  plot(density(xbar.vec))
  qqnorm(xbar.vec);qqline(xbar.vec)
  par(op)
}
clt(x=x2,n=30)
```

## Credits:
- [Law of Large Numbers - Khan Academy](https://www.youtube.com/watch?v=VpuN8vCQ--M&t=505s)
- [Central Limit Theorem - Khan Academy](https://youtu.be/JNm3M9cqWyc?si=12bH5tMsWL1T78qx)