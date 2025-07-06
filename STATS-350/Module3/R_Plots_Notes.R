# Requirement: ggplot2 since some of the examples later use it
install.packages("ggplot2")

# ***Let's review some ways we can plot our random variable****
pulses <- scan("https://mtrosset.pages.iu.edu/StatInfeR/Data/pulses.dat")

# Example 1: Creating a histogram to plot the frequency of pulses across some bins; Also Boxplot to show quantiles
# 1. Default histogram
# 2. No frequency, just shows the probability density that the observations within a bin contribute to.
# Note: In a boxplot, it'll usually show outliers as separate dots outside of the box and whiskers
hist(pulses)
hist(pulses, freq=False)
boxplot(pulses)

# Example 2: Normal Probability Plots (QQ-Plots). These are also known as quantile-to-quantile plots, 
# and they help us determine whether a sample comes from a normal distribution population. Mathematically, the data 
# is plotted in such a way that the points should form an approximately straight line if the data they come from is normal.
# Deviations from a straight line will indicate that the distribution isn't normal. Any points that are at extreme points 
# above or below the line, or just distant from the line are suspected outliers.
qqnorm(pulses)
qqline(pulses)

# Example 3: Kernel Density Plot creates a graph that approximates the population PDF.
plot(density(pulses))


# Example 4: ggplot histogram; we had to create a histogram
# Note: You only need to download the library once, but you still have to load it in your scripts everytime.
# Also ggplot only works with dataframes.
library(ggplot2)
data1 = data.frame(pulses=pulses)

# this creates the canvas with the values of the heartbeat pulses. It doesn't create the graph yet, but 
# it sets up the scaling 
g1 = ggplot(data=data1, mapping = aes(x = pulses)) 

g1 + geom_histogram() # Create and plot histogram of data
g1+geom_boxplot() # Create and plot boxplot of data

# You need to create a different canvas for qq-plots though.
g2 = ggplot(data=data1, mapping=aes(sample=pulses)) 
g2+geom_qq()+geom_qq_line()


# *** Using R to review how to use data frames ****

# Two variables weight (continuous) and feed (categorical data e.g. casin, horsebean etc).
# We can't really find mean, stdev for categorical data, but we can put them into bins to see their frequencies.
summary(chickwts)
view(chickwts)
wt <- chickwts$weight
wt
hist(wt)

# This creates a scatter plot between the waiting and eruptions vectors
summary(faithful)
plot(waiting ~ eruptions, faithful)