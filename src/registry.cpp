#include "registry.h"

// needs to be a raw pointer as must not be accessed in any way (i.e. deleted) outside the GIL
py::dict* Registry::registry = new py::dict;
