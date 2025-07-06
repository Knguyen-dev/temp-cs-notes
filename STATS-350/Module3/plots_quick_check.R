# PSet: R Plots 



# *** Questions 1-3 ****
# Shape; 
# 1. Symmetric: median is in the center of the box, whiskers are equal length
# 2. Right skew: Median is closer to Q1
# 3. Left skew: Median is closer to Q3
# Credits: https://medium.com/geekculture/what-are-right-skewed-and-left-skewed-distributions-a29b3def7598

# Center: The median, which is indicated by the medain line in the box.
# Spread (Variability: IQR measures the middle 50% of the data, which we can reason geometrically. It 
# just shows the range of middle 50%. Anyways the larger hte IQR the more spread out that middle 50% is. The smaller,
# the more tightly clustered it is. In our case, it means the middle 50% of data values are spread out over a range of 0.9 units.
# which is a tight fit.

# *** Analyzing our BoxPlot ***
# 1. Shape: Slightly left skew since median is closer to Q3.
# 2. Likely not a normal dist. due to skewness and outliers.
# 3. Maximum value and third quartile are very close to each other.

# Note: Is it normal? well symmetry may suggest normality but our boxplot is kind of skewed 
# and it has outliers on the left side. So it's probalby not normal.

max = 6
min= 3.9 # there are numbers lower but they're outliers in the boxplot 

q3 = 5.9
median = 5.6
q1 = 5
iqr = 0.9
q3+1.5*iqr # 7.25
q1-1.5*iqr # 3.65


# *** Question 4 ***
# 1. Scan data into vector
# 2. Do a QQ-Plot and a QQ-Line plot too
# Determine the amount of outliers
x <- scan("https://mtrosset.pages.iu.edu/StatInfeR/Data/sample774.dat")
library(ggplot2)
data4 = data.frame(x=x)
g4 = ggplot(data=data1, mapping=aes(sample=x)) 
g2+geom_qq()+geom_qq_line()

# *** Question 5 ****
plot(density(x)) # 3 peaks

# *** Question 6 ***
plot(density(x, bw=10)) # looks like a normal distribution, but what's bandwidth, the horizontal scale?

# *** Question 7 ***
# 1. Create Variable
# 2. Create Data frame (ggplot likes dataframes)
# 3. Build QQ plot using ggplot 2
# Note: I mean yeah it seems like a straight line; just some outliers here and there, but 
# other than that everything seems alright.
y <- log(x)
data7 = data.frame(y=y)
g7 = ggplot(data=data7, mapping=aes(sample=y))+geom_qq()+geom_qq_line()

