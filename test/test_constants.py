import platform
import pytest
from pybind11_examples import Constants, test


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
        assert getattr(c, key) == value

    # check singleton
    assert id(c.pi) == id(Constants().pi)

    # check can re-construct with new constants
    c = Constants(isquared=-1)

    # check can't re-construct with existing constants
    with pytest.raises(ValueError):
        c = Constants(isquared=-1)

    assert list(c) == list(dict(c.items()).keys())

    # check can add new constant in C++...
    assert "cpp_version" not in c
    test.add_constant("cpp_version", 20)
    assert c.cpp_version == 20
    # ...and not modify it 
    with pytest.raises(ValueError):
        test.add_constant("cpp_version", 11)

    # note that c *can* be reassigned
    c = 0
    assert c == 0

if __name__ == "__main__":
    test_constants()
    print(Constants())
