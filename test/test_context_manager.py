import pytest
from pybind11_examples import ManagedThing


def test_context_manager():
    with ManagedThing(6, 7) as resource:
        # this returns the result of an operation on the resource, not the resource itself
        x = resource()
        assert isinstance(x, int)
        assert x == 42
    # not accessible outside cm after with block
    with pytest.raises(RuntimeError):
        resource()

    # not accessible outside cm outside with block
    resource = ManagedThing(0, 1)
    with pytest.raises(RuntimeError):
        resource()


if __name__ == "__main__":
    test_context_manager()
