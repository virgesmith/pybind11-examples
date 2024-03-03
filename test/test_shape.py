import pytest
from pybind11_examples import Shape, Circle, call_shape


def test_abstract_base() -> None:
    # ABC *can* be instantiated... (python subclasses need to call __init__)
    a = Shape()
    # ... but calling pure virtual functions will throw
    with pytest.raises(RuntimeError):
        a.area()
    # ...though others won't
    a.colour() # virtual
    a.dim() # nonvirtual


def test_cpp_subclass() -> None:
    # Circle only implements the pure virtual methods
    c = Circle(1.0)
    assert c.area() == 3.14159
    assert c.perimeter() == 6.28318
    assert c.colour() == "Black"
    assert c.dim() == 2

    s = call_shape(c)
    assert "Circle" in s
    assert "2d" in s
    assert "A=3.14159" in s
    assert "L=6.28318" in s
    assert "Black" in s


def test_py_subclass() -> None:
    class Square(Shape):
        def __init__(self, length: float) -> None:
            Shape.__init__(self)
            self.length = length

        # implement the pure virtual method
        def area(self) -> float:
            return self.length ** 2

        # override the (virtual) default implementation
        def perimeter(self) -> float:
            return self.length * 4
        
        def colour(self) -> str:
            return "Green"

        # override the nonvirtual method (this will only affect direct calls from python)
        def dim(self) -> str:
            raise TypeError("nonvirtual impl not visible via Shape ref")

    # direct access
    s = Square(0.5)
    assert s.area() == 0.25
    assert s.perimeter() == 2.0
    with pytest.raises(TypeError):
        s.dim() # override is called

    # access via Base ref
    s = call_shape(s)
    assert "Square" in s
    assert "2d" in s # override not called
    assert "A=0.25" in s
    assert "L=2.0" in s
    assert "Green" in s

if __name__ == "__main__":
    test_abstract_base()
    test_cpp_subclass()
    test_py_subclass()