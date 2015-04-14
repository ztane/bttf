# -*- coding: utf-8 -*-
try:
    from setuptools import setup, find_packages
except ImportError:
    from ez_setup import use_setuptools
    use_setuptools()
    from setuptools import setup, find_packages

from setuptools import Extension, Feature

import platform
import sys

if sys.version_info >= (3,):
    speedups = Feature(
        "Mandatory C extension module for bttf",
        standard = True,
        ext_modules = [
            Extension('bttf._bttf',
                ['bttf/_bttf.c'],
                depends=['bttf/typehacks.h', 'bttf/bytesformat.h'],
                optional=False),
        ]
    )

extra_kw = dict(features={'speedups': speedups })

setup(
    name='bttf',
    version='0.1',
    description='Brings some of the magic from the future to the past and from the past back to the future.',
    author='Antti Haapala',
    author_email='antti.haapala@anttipatterns.com',
    url='https://github.com/ztane/bttf',
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Framework :: Pyramid",
        "Programming Language :: Python :: Implementation :: CPython",
    ],
    setup_requires=[],
    include_package_data=True,
    packages=find_packages(),
    tests_require=[],
    **extra_kw
)
