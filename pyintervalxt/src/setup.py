import os
from distutils.core import setup
from setuptools.command.egg_info import egg_info

SRC_PATH = os.path.dirname(__file__)

class vpath_egg_info(egg_info):
    r"""
    A VPATH aware egg_info for VPATH builds with Automake, see
    https://blog.kevin-brown.com/programming/2014/09/24/combining-autotools-and-setuptools.html
    """

    def run(self):
        r"""
        Run egg_info.run() but build the .egg-info in the --build-base path
        that was given to "build".
        """
        if "build" in self.distribution.command_obj:
            build_command = self.distribution.command_obj["build"]
            self.egg_base = build_command.build_base
            self.egg_info = os.path.join(self.egg_base, os.path.basename(self.egg_info))
        egg_info.run(self)


setup(
    name = 'pyintervalxt',
    version = '3.3.4',
    packages = ['pyintervalxt',],
    license = 'GPL 3.0+',
    install_requires=[
        'cppyy',
        'cppyythonizations',
    ],
    long_description=open(os.path.join(os.path.dirname(__file__), '../../README.md')).read(),
    include_package_data=True,
    cmdclass={'egg_info': vpath_egg_info},
    package_dir={"": os.path.relpath(os.path.dirname(__file__))},
    package_data={"": ["../../COPYING"]},
)
