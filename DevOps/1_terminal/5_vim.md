# Vim (VI Improved)
Vim is a terminal editor, an improvement of a terminal editor called "VI" which happened a long time ago. At the barebones level, Vim is a very basic text editor. For most people, they'll use plugins, or a more modern variant of vim called "Neovim". The idea behind using vim is that some people feel a lot more comfortable and faster when just using the keyboard rather than using the mouse.

A major thing is "vim bindings" which is just your shortcuts that make things a little faster. On Linux or Mac you may already have Vim installed. Then on Windows, you could use the WSL and then install vim on theee `sudo apt install vim`.


### The Basics
```bash

# Opens a file, if it doesn't already exist, then it's going to create a new file
vim <fileNmae>

# Closes vim. 
:q

# A common issue is that people change the file. Then if you try to Vim will yell at you because you can't just quit a file after you've made changes. As a result, you'll need to tell Vim to disregard the changes you made.
:q!

# The exclamation mark is actually used to execute commands linux commands whilst in vim. So here you're going to list directories in our PWD. The file in vim will tell be opened, and you'll be prompted to continue typing in vim after the command runs.
:!ls

# Vim works with modes. When you enter Vim, you're in **"normal"** mode. Here every key is a command. So you're aren't writing/typing letters to the file yet. Here you'll run commands using colon and then some letters after it:

:q
:ls

# To type letters into the file, press the `i` key. Now the keys you press don't have some special functionality, but rather they are just used to type letters into a file. To go back into normal mode, press `esc`.

# This command writes your changes to the file
:w

# You can write your changes and then quits/closes the file
:wq
```

There are multiple keys that etner the insert mode, but the difference is how they do it:
- using i: Inserts after the cursor
- Using a: Inserts before the cursor
- USing o: Enters insert mode but starts off with a new lines


### Basics of upgrading
- `:set number`: Create line numbers
- Each action can be repeated x number of times. If you combine numbers with keybindings, you can traverse a lot faster.
- Basic movement: k goes up, j goes down, h goes left and l goes right. w skips to the next word forward, whilst b goes one word back.
- `:set relativenumber`: Instead of getting absolute line numbers, you get the line numbers relative to your position. This lets you do keybinding a lot better.




## Credits:
- [Getting Started With Vim - PrimaGen](https://www.youtube.com/watch?v=X6AR2RMB5tE)
- [Vim tutorial - freeCodeCamp](https://www.youtube.com/watch?v=RZ4p-saaQkc)
- [Vim tutorial - Ben Awad](https://www.youtube.com/watch?v=IiwGbcd8S7I)
