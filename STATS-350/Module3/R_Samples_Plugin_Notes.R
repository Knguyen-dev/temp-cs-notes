#' ---
#' title: Data in R
#' subtitle: Samples and pseudo-random samples
#' author: Arturo Valdivia
#' date: ""
#' output:
#'   html_document:
#'     toc: true
#'     toc_depth: 3
#' ---


#' ## 1. Samples
#' to clear global environment use this code:
# This basically resets your workspace
rm(list = ls())


#' 
#' ### 1.1. Samples using scan()

#' Store the data in the same folder as your R file

# Reads numeric values from a file and stores them in a vector 
# Assumes the heights.txt file contains numbers separated by spaces or line breaks
vec <- scan('heights.txt')

# Same as the previous example, but it reads from a URL instead of a local file. 
x <- scan("https://mtrosset.pages.iu.edu/StatInfeR/Data/pulses.dat")


#' ### 1.2. Pseudo-random samples from known distributions

# + Random sample from a normal distribution
# Purpose: It generates 30 pseudo-random numbers from the standard normal distribution (mean=0, sd=1)
rnorm(30)

# So now we're generating 30 random numbers from a normal distribution with mean=10 and sd=2
rnorm(n = 30, mean = 10, sd = 2)

# Generates 20 random numbers form the uniform distribution between 0 and 1.
# Note: Again uniform meaning that all values in the range [0,1] are equally likely to appear in the sample.
runif(20)

# Runs a binomial experiment 25 times:
# - Each experiment consists of 10 independent trials, each with a 30% of success.
# - For each experiment we'll return the number of successes.
# Due to the expected value of a binomial distribution, we think most of our values will be around 3, meaning 3 successes per trial on average.
rbinom(25, size = 10, prob = 0.3)

#' ### 1.3. A pseudo-random sample from a vector

# Use the function 'sample()' to generatea pseudo-random sample from an existing vector.
# Purpose: Takes a sample of size 10 with replacement from the vector vec. "With replacement" just means the
# same value can be selected multiple times.
# Note: This is equivalent to sample(vec, 10, T)
vec
sample(x = vec, size = 10, replace = TRUE)
#' or simply
sample(vec, 10, T)

# Setting the "random seed" is just a common way in programming to make sure that random number
# algorithm is alwyas producing the same random numbers. This ensures your output is reproducible during testing.
set.seed(10) 
sample(vec, 10, T)


# **** Starting notes on the Plugin Principle in R ****
# Let X be the random variable representing the pulse rates for Peruvian indians. We can say the probability
# that a randomly selected person has a pulse less than or equal to 72 is F(72) = P(X<=72). However we don't know the 
# distribution of X, the actual population distribution. But we can use our sample to estimate this distribution based 
# on the plugin principle. x is the vector of our data here.

# Step 1: Since we're dealing with a discrete variable, construct a pmf called fx. 
# Using the plugin principle, assign the same probability (1/n) to each value/outcome in vector x.
# To do this, we'll use the rep() function that replicates the same number n times. We'll replicate the fraction (1/n).
n = length(x)
fx = rep(1/n, n)

# optional: We just wanna see a frequency table for x
# we see that there are 6 people with a heart rate of 60 beats per minute.
# 52 56 60 64 68 72 74 76 80 84 88 92 
# 1  1  6  7  4  8  1  4  2  1  3  1 
table(x)

# Step 2: Construct an empirical CDF. To estimate the probability of the pulse rate 
# being less than or equal to 72, we'll use the corresponding proportion in our sample.
# Just get the number of outcomes where X <= 72 (39) and divide it by the number of observations in the sample.
mean(x <= 72) # output: 0.6923

# Equivalently, R has a predefined function that allows you to calculate the empirical CDF. 
ecdf(x)(72)

# Step 3: Plotting the empirical CDF 
plot(ecdf(x))

# Step 4: Let's start estimating the population attributes
# 1. Population mean, which can be calculated using the summation or the mean() function.
muhat = sum(x * fx) 

# 2. Population variance, or mean((x-mean(x))^2)
# Note: You can get the population standard deviation by doing the sqrt function
# ATTENTION: Please don't use var(x), it doesn't work.
sigma2hat = sum((x-muhat)^2*fx) 

# 3. Population Median (2nd quartile), specific quantiles, and summary statistics
# Note 1: You can also get 5-number summary min, q1, median, q3, and max using summary()
# Note 2: You can also use quantile(x, probs=0.3) for the 0.3 quantile, hint this is how you can calculate specific quantiles.
# Note 3: IQR(x) gets the IQR=q3-q1
median(x)

# ***** Plug-in Principle Quick Check *****

# Q1: Use the plugin principle to calculate the population mean
# 1. Calculate the empirical pmf (1/n)
# 2. Do the summation calculation to get the estimation of the population mean
x = c(2, 6, 8, 4, 10, 6, 4, 6, 3, 4, 2, 3, 5, 5, 8, 3, 6, 5, 1, 6)
n = length(x)
fx = rep(1/n, n)
mean(x)
muhat = sum(x * fx) # 70.30 # 4.85
sigma2hat = sum((x-muhat)^2*fx) # 4.82
median = median(x) # 5
IQR = IQR(x) # 3
IQR
