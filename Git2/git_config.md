

## Step 1: Go to the specific repo
```bash

# Run this in root github repo
# --list: shows all configs settings
# --show-origin: shows where each config value is coming from, allowing us to know whether it's a global or local config.
# 


git config user.name "Kevin Nguyen"
git config user.email "kevin12.TRN@infosys.com"
git config commit.gpgsign false
```


```bash

git checkout main-clean

# git rebase -i <commit-id before your target>
git rebase -i 3b0e8ac07a3272dbef981d50edaec81d56a009c3^

# A terminal will open up and just let your target commit be edit instead of pick
# save and close editor

# Amend that target commit, then in the editor remove the text you want
# Then save and close the editpr
git commit --amend

# Now finish the rebase:
git rebase --continue 

# switch back to main branch
git switch main

# Makes local exactly identical to main-clean
git reset --hard main-clean

git push origin main --force-with-lease

# Then remove the co-authored message
# continue rebase. then force push the main branch
```