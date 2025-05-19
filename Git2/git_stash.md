# Git Stash and Git Worktree


### What is `git stash`?
- Takes uncommitted changes (staged or unstaged) and reverts working directory back to match the head commit.
- Stashes are stored in a stack like structure so you can have multiple stashes. 
- So you'd see that there are new commits. You'd stash your current work, get those new commits, and then unstash your work. You could definitely have merge conflicts when unstashing.

#### Example 1:
You're working on a feature branch and you modified some files like `navbar.tsx` or `utils.ts`. Now the code isn't finished, so the conventional way of saving a draft to come back to it later would be using `git stash -m <stash-name>`. Your uncommitted changes are now stored, and your feature branch has been reset to match the head.

You should also be able to move to other feature branches without git yelling at you. As well as this, you should be able to create other feature branches without your changes following you. Lastly, just do `git stash pop` on the branch you stashed your changes at, and it will pop out those changes for you.

- **NOTE:** Only stashes files that are tracked by git.

#### Advanced Applications
You could name the stashes, list your stages, and store specific changes to specific stashes.

### What is `git worktree`?

#### Premise: Review of `git stash`
You're working changes on a feature branch, but your team needs you working on another branch:
1. You stash your uncommitted changes on your feature branch
2. You switch to the other branch they want you to work on. When you're done with this branch, you go back to your original feature branch.
3. You unstash your uncommitted changes you had on this branch and get back to where you left off.

This is a decent workflow, but there's also another convenient way of dealing with this.

#### Introducing Git WorkTree
Normally you can only have one branch checked out, and this is where you're uncommitted changes are and this is where you can work. This is called your **working copy**, it's a branch that still has work that hasn't been committed yet. 

However with git worktree, you can have multiple branches checked out, and they can both have uncommitted changes, and therefore different worktrees. You can also commit and push changes on one branch, and this doesn't affect uncommitted changes in your other worktree tree.

#### Example 1
You're on the `feature/login` branch with two uncommitted changes and want to do a hotfix on the `main` branch. Git won't let you switch to the `main` branch due to uncommitted changes, so let's fix that.

1. `git worktree add../my-project-main-dir main`
  - This creates a new directory (`../my-project-main-dir`) where the `main` branch is checked out.
  - Now you can work on `main` separately without affecting `feature/login`

So this creates a new directory (../my-project-main-dir) with the main branch checked out. Now you can switch back and forth between your feature/login work and your hotfix on main, even opening them in different windows and having them both checked out.



## Credits 
- [Git STASH Tutorial - The Modern Coder](https://www.youtube.com/watch?v=BSLzA8oCT7g)
- [git worktree intro - Philomatics](https://www.youtube.com/watch?v=ntM7utSjeVU)