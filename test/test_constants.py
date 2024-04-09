import platform
import pytest
from pybind11_examples import CONST, test


def test_constants() -> None:
    e = 2.71828
    CONST.add(pi=3.14159, e=e)

    # cant redefine, even if value is the same
    with pytest.raises(ValueError):
        CONST.add(pi=3.14159)

    # add a constant
    CONST.sqrt2 = 2**0.5
    CONST.hostname = platform.node()
    # c["epsilon"] = 2 ** -52  # need to implement __getitem__/__setitem__
    setattr(CONST, "epsilon", 2**-52)

    # check cant modify
    with pytest.raises(ValueError):
        CONST.pi = 4

    with pytest.raises(ValueError):
        CONST.e = -1

    # check constant not referencing a mutable object
    e = 3
    assert CONST.e == 2.71828

    # check cant delete
    with pytest.raises(AttributeError):
        delattr(CONST, "e")

    # check can iterate
    for key in CONST:
        assert key in CONST

    for key, value in CONST.items():
        assert key in CONST
        assert getattr(CONST, key) == value

    # check singleton
    assert id(CONST.pi) == id(CONST.pi)

    # check can re-construct with new constants
    CONST.add(isquared=-1)

    # check can't re-construct with existing constants
    with pytest.raises(ValueError):
        CONST.add(isquared=-1)

    assert list(CONST) == list(dict(CONST.items()).keys())

    # check can add new constant in C++...
    assert "cpp_version" not in CONST
    test.add_constant("cpp_version", 20)
    assert CONST.cpp_version == 20
    # ...and not modify it
    with pytest.raises(ValueError):
        test.add_constant("cpp_version", 11)


if __name__ == "__main__":
    test_constants()
    print(CONST)
    # note that CONST *can* be reassigned
    CONST = 0
    assert CONST == 0
