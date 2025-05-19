# Git Fetch vs Git Pull

### What's the difference?
- `git fetch origin`: Downloads any new commits, tags, and branches from the remote. It stores it in `.git/refs/remotes/origin/`. Your local branches remain unchanged until you want to merge. You'll probably see the message "Your branch is behind 'origin/main' by 2 commits, etc." which means there are 2 new commits on the remote that your local doesn't have.
- `git merge origin main`: Then you're telling git to merge those changes onto your local main branch. The main branch, or head, should now include the commits from the fetch.
- `git pull`: A command that does git fetch and then git merge.

Let's also look at a visualization:
```
              4 (remote main)
              |
3 (main)      3
|             |
2             2
|             |
1             1
```
So here someone pushed a new commit to the remote and we need to update our local to match the remote. Now we do git fetch:
```
4             4 (remote main)
|             |
3 (main)      3
|             |
2             2
|             |
1             1
```
We've downloaded the commits and changes into our local computer, but we haven't actually incorporated them yet. They're just stored there, ready to be used. Now we do `git pull`
```
4 (main)      4 (remote main)
|             |
3             3
|             |
2             2
|             |
1             1
```

### Why not use git pull all the time?
With the latter we have less control, as sometimes being able to fetch and merge automatically won't work given the setup.
```
A (main)      B (remote main)
|             |
3             3
|             |
2             2
|             |
1             1
```
This represents the situation where you made a local commit, but your teammate pushed a new commit on the remote. They conflict with each other so you can't simple do `git pull` as you'll get merge conflicts. 

### Approach 1: Rebasing 
You could "rebase" using `git pull --rebase`, which will try to overlay your local commits over the remote one.
```
A (main)
|
B
|
3
|
2
|
1
```
This makes it look like you pushed your commit (A) after B, even though they were both originally positioned after commit 3. This is popular because we avoid needing to do a merge commit, a commit often done when merging branches. There's also a chance your changes will conflict and cause **rebase conflicts**. Git will pause the rebase and allow you to resolve the conflicts:
  - Keeping your local changes
  - Deferring to remote changes
  - Or keeping both and then control which sections you're keeping or removing.

After this, you'll stage the changes for the files you resolved, and confirm the rebase with:
```bash
git rebase --continue
```

### Approach: `git pull --ff`
Downloads the new commits and starts a standard merge. Git will have you do a merge commit, which is just when you have to record a commit when we're merging branches. Again, git will help you handle any merge conflicts.

So here is that same situation before:
```
A (local main)    B (remote)
|                 |
2                 2
|                 |
1                 1
```

Then the `git pull --ff` process will result in this:



### Handling "Pulling from main"
You have some staged and unstaged changes and want to pull updates from main.

```bash

# 1. Confirm what changes are staged, unstaged, or untracked 
git status

# 2. Stash local changes
git stash -u -m "WIP: stash before merging master"
# -u: Ensures untracked files are also stashed
# -m: For putting a stash message
# git stash list for seeing the various lists we have.


# 3. Pull latest updates from main 
git fetch origin
git merge origin/main

# 4. Apply stashed changes
# NOTE: git stash apply would also work, but it doesn't remove your stashed changes. Also if you have merge conflicts like 
git stash pop
```

Again maybe after resolving stuff you could have messed up and whatnot:
```bash
git merge --abort
```

You could also make another contingency plan by backing up your current branch and its unstaged and untracked changes.

### Handling backing up a branch
```bash

git add -A
git commit -m "TEMP: backup of WIp with untracked and unstaged changes"
git checkout -b "backup/<branch_name>"

# Go back to your working branch and undo the commit.
git reset HEAD~1
```

