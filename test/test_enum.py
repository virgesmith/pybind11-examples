from enum import Enum

import pybind11_examples as outerscope
from pybind11_examples import CEnum, CppEnum


class PyEnum(Enum):
    SEVEN = 7
    EIGHT = 8


def test_enum():
    # enums dont implicitly equate to integers
    assert CEnum.ONE != 1
    assert CEnum.TWO.name == "TWO"
    assert CEnum.TWO.value == 2

    assert CppEnum.THREE != 3
    assert CppEnum.FOUR != 4
    assert PyEnum.SEVEN != 7
    assert PyEnum.EIGHT != 7

    assert CppEnum.THREE.value == 3
    assert CppEnum.FOUR.value == 4

    # class X(int): pass
    # assert issubclass(X, int)

    Pybind11Enum = [CppEnum, CEnum]

    # name comparison (note 3.12 syntax)
    # also pybind11 enums don't subclass enum and
    # pybind11_builtins.pybind11_object is seemingly inaccessible
    def n[T: Enum | Pybind11Enum](e: T, name: str) -> bool:
        return e.name == name

    assert n(CEnum.ONE, "ONE")
    assert n(CppEnum.THREE, "THREE")
    assert n(PyEnum.SEVEN, "SEVEN")


def test_enum_scope() -> None:
    # export_values() pollutes outer scope
    assert hasattr(outerscope, "ONE")
    assert hasattr(outerscope, "TWO")
    assert not hasattr(outerscope, "THREE")
    assert not hasattr(outerscope, "FIVE")
