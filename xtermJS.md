

### What is XtermJS?
Xterm.js is a library that lets you insert a fully functional terminal in your frontend applications. You can choose to have stuff like a bash terminal, vim, tmux, etc. The react-xtermjs is just a library that lets you use the Xterm.js in a react application.

```Tsx
import { FitAddon } from '@xterm/addon-fit'
import { useEffect } from 'react'
import { useXTerm } from 'react-xtermjs'

const TerminalComponent = () => {
  const { instance, ref } = useXTerm()
  const fitAddon = new FitAddon()

  useEffect(() => {
    // Load the fit addon
    instance?.loadAddon(fitAddon)

    const handleResize = () => fitAddon.fit()

    // Write custom message on your terminal
    instance?.writeln('Welcome react-xtermjs!')
    instance?.writeln('This is a simple example using an addon.')

    // Handle resize event
    window.addEventListener('resize', handleResize)
    return () => {
      window.removeEventListener('resize', handleResize)
    }
  }, [ref, instance])
  return <div ref={ref} style={{ height: '100%', width: '100%' }} />
}
```
`instance` may not be defined when `useEffect` runs because `useXTerm` is probably asynchronous so the terminal `instance` might not be immediately available. This is why they use optional chaining to safely call methods on `instance` only if it's defined. Breaking down some key points:

- **useXTerm:** This hook creates an instance of the terminal and provides a ref to attach the terminal to a DOM element.
- **FitAddon:** This is an add-on that allows the terminal to resize to fit its container. To add an add-on to the terminal, you need to call `instance?.loadAddon(someAddon)`, which is what we did with fitAddon.
- **useEffect:**  Runs after component mount but it's here to load the fitAddon onto the terminal and set up a resize event listener to resize the terminal when the window is resized.
