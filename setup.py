import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

version = '1.0'

with open("README.md", "r") as fh:
    long_description = fh.read()

class ObjetBuild(build_ext):
    def run(self, ):
        subprocess.check_call(['make', 'python'])
        build_ext.run(self)

setup(
    name='pyobjet',
    version=version,
    author='Mahan Fathi',
    author_email='mahan0fathi@gmail.com',
    description="OBJET: A Computer Vision Graphical Sandbox",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/MahanFathi/OBJET",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux",
    ],
    install_requires=['numpy'],
    packages=['pyobjet'],
    package_data={'': ["_OBJET.so"]},
    cmdclass=dict(build_ext=ObjetBuild),
    zip_safe=False,
)
