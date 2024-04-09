from pybind11_examples import Registry


def test_registry() -> None:
    class PyBase:
        """Some unrelated base class"""

    class Test0(Registry, PyBase):
        pass

    class Test1(Registry, PyBase, id=5):
        pass

    class Test2(Registry, PyBase, id=8):
        pass

    class Test3(PyBase, Registry, id=13, other="string"):
        pass

    assert Test0 in Registry()
    assert Registry()[Test0] == {}
    assert Registry()[Test1]["id"] == 5
    assert Registry()[Test2] == {"id": 8}
    assert Registry()[Test3] == {"id": 13, "other": "string"}

    # access via singleton instance
    R = Registry()
    assert Test0 in R
    assert R[Test0] == {}
    assert R[Test1]["id"] == 5
    assert R[Test2] == {"id": 8}
    assert R[Test3] == {"id": 13, "other": "string"}

    # keys via __iter__
    keys = list(R)
    assert Test0 in keys
    assert Test1 in keys
    assert Test2 in keys
    assert Test3 in keys

    # dict items
    items = dict(R.items())
    assert items[Test0] == {}
    assert items[Test1]["id"] == 5
    assert items[Test2] == {"id": 8}
    assert items[Test3] == {"id": 13, "other": "string"}


if __name__ == "__main__":
    test_registry()

    print(list(Registry()))
    print(dict(Registry().items()))
