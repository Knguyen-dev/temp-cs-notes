x = c(88, 76, 84, 64, 60, 64, 60, 64, 68, 74,
      68, 68, 72, 76, 72, 52, 72, 64, 60, 56,
      72, 88, 80, 76, 64, 72, 60, 76, 88, 72, 
      64, 60, 60, 72, 92, 80, 72, 64, 68)

# a. Step 1: Graph empirical CDF 
plot(ecdf(x))

# b. Create the empirical pmf 
n = length(x)
fx = rep(1/n, n)
muhat = sum(x*fx)
sigma2hat = sum((x-muhat)^2*fx)
muhat
sigma2hat


# c.
medianhat = median(x)
IQR = IQR(x)

medianhat
IQR


# d. Ratio between IQR / sigma
sigmahat = sqrt(sigma2hat)
ratio = IQR / sigmahat
sigmahat
ratio

# e. Boxplot 
boxplot(x)

# f. Normal Probability Plot, 
qqnorm(x)
qqline(x)

# g. Kernel Density Plot
plot(density(x))

# h. 
