import platform
import pytest
from pybind11_examples import Constants


def test_constants() -> None:

    e = 2.71828
    c = Constants(pi=3.14159, e=e)

    # cant redefine, even if value is the same
    with pytest.raises(ValueError):
        Constants(pi=3.14159)

    # add a constant
    c.sqrt2 = 2**0.5
    Constants().hostname = platform.node()
    # c["epsilon"] = 2 ** -52  # need to implement __getitem__/__setitem__
    setattr(c, "epsilon", 2**-52)

    # check cant modify
    with pytest.raises(ValueError):
        Constants().pi = 4

    with pytest.raises(ValueError):
        Constants().e = -1

    # check constant not referencing a mutable object
    e = 3
    assert c.e == 2.71828

    # check cant delete
    with pytest.raises(AttributeError):
        delattr(c, "e")
    assert c.e == 2.71828

    # check can iterate
    for key in c:
        assert key in Constants()

    for key, value in c.items():
        assert key in Constants()
        print(key, value)
        assert getattr(c, key) == value

    assert id(c.pi) == id(Constants().pi)

    assert list(c) == list(dict(c.items()).keys())


if __name__ == "__main__":
    test_constants()
