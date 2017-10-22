# Description

This is test with comparison of exact solution with numerical one (should have second order accuracy).

# Exact solution

```
U(x,t) = i * exp (-i * (wt - kx))
```

# Numerical solution

Case of `dt = 0.5 * dx / c`. Comparison of two numerical solutions for `dx = A` and `dx = A / 2`: for latter error should reduce `2^2 = 4` times in comparison with the former.
