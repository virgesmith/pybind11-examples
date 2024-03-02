#!/usr/bin/env python3

import os
import glob
from setuptools import setup
import numpy
from pybind11.setup_helpers import Pybind11Extension, ParallelCompile

# see https://github.com/pybind/python_example


def readme():
  with open('README.md') as f:
    return f.read()


def list_files(dirs, exts):
  files = []
  for directory in dirs:
    for ext in exts:
      files.extend(glob.glob(os.path.join(directory, "*." + ext)))
  return files


ext_modules = [
  Pybind11Extension(
    '_pybind11_examples',
    sources=list_files(['src'], ["cpp"]),
    include_dirs=[numpy.get_include()],
    depends=["setup.py", "pybind11_examples/__init__.py"] + list_files(["src"], ["h"]),
    cxx_std=20,
  ),
]

ParallelCompile().install()

setup(
  name='pybind11-examples',
  packages=["pybind11_examples"],
  package_data={"pybind11_examples": ["py.typed", "*.pyi"]},
  ext_modules=ext_modules,
  zip_safe=False,
)
