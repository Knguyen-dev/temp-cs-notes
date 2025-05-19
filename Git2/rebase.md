



### Git Rebase Explained
- Motivation: Take commits from a branch and put it on master

I mean for smaller or medium sized projects this probably won't even matter. However, there are multiple ways to deal with or get a a more linear version history:
1. You can squash your merge. As a result all of your commits in your feature branch appear as one commit. 
2. Do a rebase
3. Or you could do both.

#### Reviewing a regular merge
```
M
| \
|  E
A  |
|  D
B /
|
C
```
Here we have a branch with two new commits D and E, whilst our main branch is still on A. Our merge will put those commits onto the main branch and create a branch commit, handling any errors. However some people may not like this model because if you have multiple branches and merges, it may get really hard to parse.

#### Idea of a rebase
```
   E
A  |
|  D
B /
|
C
```
Instead, a rebase is going to duplicate the commits on the branch and then copy them on top of the current master. You can think of it like we just made two new commits to main, and those commits came from the branch. So it's re-writing history. Here's how that works visualized:
```
E
|
D
|
A
|
B
|
C
```
Then the benefits of this approach is a nice, clean graph. It'd make things really easy to follow. However there are drawbacks. Rebasing doesn't play really well with open source projects and pull requests. Can be dangerous if you're on a shared branch, as you may re-write the commits of other developers. However there are ways to mitigate this.

It's called rebasing because when you created the branch, you diverged from the main branch at some commit in the past, which can be viewed as the base of your branch. However when you're doing "rebasing" you're copying those new branch commits onto the latest commit on main, treating the main branch was the base for your new commits. You're changing bases, "rebasing". With this, your feature branch would have all the changes from main, and then the vice versa when you "merge".

This type of merge that keeps your history in a linear fashion is called fast-forwarding. Other pit-falls:
1. Don't rebase if you've already pushed your feature branch. This is because when you push, those commits have your original hashes. However when you rebase, those branch commits get new commit hashes. Git won't be able to differentiate between the two of them, and you may lose work if you're re-writing the history.

  - **NOTE:** If you aren't sure whether your feature branch has been already pushed, just do 

2. Merge conflicts (rebase conflicts): When anchoring your commits onto the latest version of the main branch, you'll probably run into rebase conflicts, which are a little more complicated. Rebase will apply your branch commits to the main one by one, making you resolve them one by one. And the worse thing is, the way you resolve one merge conflict may cause new ones to pop up in the following commits. As a result, you may have to go through multiple stages of conflict resolution.


#### Demo 1
```bash

# Make sure you're up to date
git pull 

# go on feature branch 
git checkout -b new_feature 

# then you're going to be periodically committing
git commit -m "More commits"

# Then your fellow developers are shipping commits to the remote main. But that's okay, we'll fix that
# when we want to merge. Now let's say you're ready to integrate your changes with the main.

# Make sure your local master is up to date with the remote master
git checkout main
git pull

# Ensure your feature branch doesn't conflict with those changes
git checkout my_feature

# Reanchors your feature branch against the latest changes. As a result, it'd look like the first commit of your feature branch follows the most recent commit of your local master.
# At this point git will let you know whether or not you have any conflicts with your local master
# However at this point, your commits are just anchored, they're prepared, but they haven't been committed or saved onto the master yet
git rebase master

# We go back to master and then this saves your branch's commits onto the local master branch
git checkout master 
git rebase my_feature 

# Then finally you'll sync your local master with the remote master.


```

#### Demo 2
Imagine you're on the "feature/login" branch and you want put your changes onto main using rebasing.Let the feature branch have 3 new commits. 


```bash

# Rebase with main. When we have a conflict, it will pause the rebase, allowing us to manually fix any merge conflicts.
# The conflict resolution for a single commit is the same as merge. Rebase will also tell you how many remaining commits that you'll need to go through in order to be done with the rebase.
git rebase main

# Now you still have 2 more commits that need to be applied when doing the rebase.
# So indicate that you're continuing the rebase. Git will open up Vim, letting you 
# modify the original commit message for the commit you just resolved; If it wasn't already clear,
# Git will automatically apply commits that don't conflicts.
# Once you think you've successfully rebased, you should do git status to check if you've you're done.
git rebase --continue 
```


### Git Interactive Rebase

With `git rebase --interactive` we can: 
- edit previous commits, including their commit messages.
- split a single commit into multiple commits.
- squash multiple commits into one.
- delete and swap the ordering of commits on the history.

#### Example: Editing a previous commit 
The main branch has already been pushed whilst the feature branch hasn't been pushed. Since the latter is true, we can attempt rebasing. Let's say on one of our commits, we left some print statements and commented out code.

```
git rebase --interactive HEAD~4
```
Takes the last four commits and isolates them for rebasing. If you don't want to count out the commits, then just replace HEAD~4 with the commit hash of the ancestor for our branch (the commit we diverged out). Now the rebase starts and you're in vim or something. If it says "pick" we will leave that commit as is.

If you change to "edit", it'll apply changes, and then pause to let you edit the file and remove those debug statements. After you're good with those changes, do git add on the files you've changed, and then `git rebase --continue`.

Since you only made one commit as "edit", all other commits will be applied automatically because git saw you left them as "pick".

#### Splitting previous into multiple commits 
Let's say that we notice that our previous commit had some changes that weren't really related to the main issue. So you want to split those commits to make things more atomic and organized.

```bash

# Isolate the commits for the feature branch
git rebase --interactive HEAD~4

# Remove the last commit in the history, but leave the working change in.
# So this actually removes first commit on the feature branch. 
# However the changes still exist/are staged
git reset HEAD ~1

# We can now create a new commit for it. On success, we're going to be placing this commit in that empty point in history. Then for lal commits that you apply after, they go after one another.
git add my_file 
git commit -m "Add readme"

# Then do another commit
git add my_other_file
git commit -m "Createdd other file"

# At this point you've done what you wnated to do, so let git apply 
# the rest of those rebases and here it'll check if you're right.
```

#### Squashing 
Let's say you want to squash a given commit into the previous commit.

git rebase --interative head~4

Then type squash for the commit you want to squash

#### Deleting 
You literally just remove them from the file


## Credits 
1. [Learn Git Rebase in 6 minutes - The Modern Coder](https://youtu.be/f1wnYdLEpgI?si=LIIynzh3Qg0WxN29)
2. [Git interactive rebase](https://youtu.be/42392W7SgnE?si=PR3NGNvyT-DXf45t)