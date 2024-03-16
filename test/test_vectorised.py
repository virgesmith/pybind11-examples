import numpy as np
import pytest
from pybind11_examples import daxpy, average_exectime


def test_vectorised() -> None:
    a = 2.0
    x = [float(i) for i in range(10)]
    y = [1.0 / float(i + 1) for i in range(10)]

    assert daxpy(1.0, 2.0, 3.0) == 5.0

    # can operate on lists but returns np array
    assert isinstance(daxpy(a, x, y), np.ndarray)

    x = np.array(x)
    y = np.array(y)

    y.resize((10, 2))
    with pytest.raises(RuntimeError):
        _ = daxpy(a, x, y)
    # numpy also fails
    with pytest.raises(ValueError):
        _ = a * x + y


def compare_vectorised_performance() -> None:
    SIZE = 10_000_000
    rng = np.random.default_rng(19937)
    a = 2.0
    x = rng.uniform(size=SIZE)
    y = rng.uniform(size=SIZE)
    z = np.empty(SIZE)

    # unvectorised python loop
    @average_exectime(n=3)
    def u_py_loop() -> None:
        for i in range(SIZE):
            z[i] = a * x[i] + y[i]

    elapsed_ms, _ = u_py_loop()
    print(f"unvectorised python loop: {elapsed_ms:.2f}ms")

    # unvectorised python listcomp
    @average_exectime(n=3)
    def u_py_lcomp() -> None:
        _ = [a * x[i] + y[i] for i in range(SIZE)]

    elapsed_ms, _ = u_py_lcomp()
    print(f"unvectorised python listcomp: {elapsed_ms:.2f}ms")

    # unvectorised pybind11
    @average_exectime(n=3)
    def u_pybind() -> None:
        for i in range(SIZE):
            z[i] = daxpy(a, x[i], y[i])

    elapsed_ms, _ = u_pybind()
    print(f"unvectorised pybind11: {elapsed_ms:.2f}ms")

    # vectorised python (numpy)
    @average_exectime(n=3)
    def v_numpy() -> None:
        z = a * x + y

    elapsed_ms, _ = v_numpy()
    print(f"vectorised numpy: {elapsed_ms:.2f}ms")

    # vectorised pybind11
    @average_exectime(n=3)
    def v_pybind() -> None:
        z = daxpy(a, x, y)

    elapsed_ms, _ = v_pybind()
    print(f"vectorised pybind11: {elapsed_ms:.2f}ms")


if __name__ == "__main__":
    compare_vectorised_performance()
