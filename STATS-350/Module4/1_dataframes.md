# Dataframes

## Section 1: Dataframes already in R
```R
# See all dataframes that we can choose
data()

# Viewing a data frame
View(chickwts)

# Existing data frames often have a description. Use help() or ?
?chickwts

# Get like the first few rows
head(chickwts)

# Get the dimensions of the dataframe
dim(chickwts)

# Get summary statistics; or str(chickwts)
summary(chickwts) 

# Selecting a series/column from the dataframe using the dolalr sign $.
# So here we're targeting the "weight" column in the chickwts dataframe.
wt <- chickwts$weights
```

## Section 2: Importing Data Frames
There are two main methods to import dataframes. The first one is less programmatic, as your using RStudio:
1. Download data file on disk
2. In RStudio, go to file and click "import dataset". Go from there.

The other way is more programmatic:
```
data1 <- read.table("/path/to/datafile.txt")
head(data1)
```

## Section 3: Installing New Packages in R
Go to tools and install packages. Here you can see what things you have installed and whatnot. Or programmatically you can put: 
```R
# Install library "ggplot2"; this is a one time thing
install.packages("ggplot2")

# Load the library into your environment; done every time.
library(ggplot2)
```


## Quick Check