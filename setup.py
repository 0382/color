from distutils.core import setup
from distutils.extension import Extension
import platform


if platform.system() == "Linux":
    include_path = "/usr/include/python%s" % platform.python_version()[:3]
    include_dirs = [include_path]
    define_macros = [('linux', None)]
elif platform.system() == "Windows":
    file_name = platform.__file__.split('\\')
    include_path = ""
    for word in file_name:
        if word.lower() == 'lib': break
        include_path += word + "\\"
    include_path += "include"
    include_dirs = [include_path]
    define_macros = [('_WIN32', None)]

ext_modules = [
    Extension(
        name = 'color._color',
        sources = ['color/_color/color.cpp'],
        language = "c++",
        include_dirs = include_dirs,
        extra_compile_args = ['-std=c++17'],
        define_macros = define_macros
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