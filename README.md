# BrainFuck JIT Compiler Interpreter

## Optimizations
### Run Length Encoding

_Example:_
```BrainFuck
+++++>+ = {4+} > +
``` 
### Peephole optimization(WIP)
_Example:_
```BrainFuck
+- = No change
>< = Do nothing
```

For traditional interpreter, use ```main``` branch.
For JIT, use ```JIT```. Feel free to contribute!!