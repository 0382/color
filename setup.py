from distutils.core import setup
from distutils.extension import Extension

ext_modules = [
    Extension(
        name = '_color',
        sources = ['color/_color/color.cpp'],
        language = "c++",
        include_dirs = ['C:/MyProgram/Python3/include']
    )
]

setup(
    name = 'color',
    version = '0.0.1',
    description='set console color',
    author = '0.382',
    packages = ['color'],
    ext_modules = ext_modules
)