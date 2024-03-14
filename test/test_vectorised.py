from time import time
import numpy as np
import pytest
from pybind11_examples import daxpy


def test_vectorised() -> None:
    a = 2.0 
    x = [float(i) for i in range(10)]
    y = [1.0 / float(i + 1) for i in range(10)]

    assert daxpy(1.0, 2.0, 3.0) == 5.0

    # can operate on lists but returns np array
    assert isinstance(daxpy(a, x, y), np.ndarray)

    x = np.array(x)
    y = np.array(y)    

    y.resize((10,2))
    with pytest.raises(RuntimeError):
        _ = daxpy(a, x, y)
    # numpy also fails
    with pytest.raises(ValueError):
        _ = a * x + y

def compare_vectorised_performance() -> None:
    SIZE = 1_000_000
    rng = np.random.default_rng(19937)
    a = 2.0
    x = rng.uniform(size=SIZE)
    y = rng.uniform(size=SIZE)
    z = np.empty(SIZE)

    # unvectorised python
    start = time()
    for i in range(SIZE):
        z[i] = a * x[i] + y[i]
    elapsed_ms = (time() - start) * 1000
    print(f"unvectorised python: {elapsed_ms:.2f}ms")

    # unvectorised pybind11
    start = time()
    for i in range(SIZE):
        z[i] = daxpy(a, x[i], y[i])
    elapsed_ms = (time() - start) * 1000
    print(f"unvectorised pybind11: {elapsed_ms:.2f}ms")

    # vectorised python (numpy)
    start = time()
    z = a * x + y
    elapsed_ms = (time() - start) * 1000
    print(f"vectorised numpy: {elapsed_ms:.2f}s")

    # vectorised pybind11
    start = time()
    z = daxpy(a, x, y)
    elapsed_ms = (time() - start) * 1000
    print(f"vectorised pybind11: {elapsed_ms:.2f}s")



if __name__ == "__main__":
    compare_vectorised_performance()
