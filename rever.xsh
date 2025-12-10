######################################################################
#  This file is part of intervalxt.
#
#        Copyright (C) 2020-2025 Julian Rüth
#
#  intervalxt is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  intervalxt is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
#####################################################################

import sys

from rever.activity import activity
from rever.activities.command import command

try:
  input("Are you sure you are on the master branch which is identical to origin/master and the only pending changes are a version bump in the configure.ac of the library? [ENTER]")
except KeyboardInterrupt:
  sys.exit(1)

command('pixi', 'pixi lock')

@activity
def dist():
    r"""
    Run make dist and collect the resulting tarball.
    """
    from tempfile import TemporaryDirectory
    from xonsh.dirstack import DIRSTACK
    with TemporaryDirectory() as tmp:
        ./bootstrap
        pushd @(tmp)
        @(DIRSTACK[-1])/configure
        make dist
        mv *.tar.gz @(DIRSTACK[-1])
        popd
    return True

$PROJECT = 'intervalxt'

$ACTIVITIES = [
    'version_bump',
    'pixi',
    'changelog',
    'dist',
    'tag',
    'push_tag',
    'ghrelease',
]

$VERSION_BUMP_PATTERNS = [
    ('configure.ac', r'AC_INIT', r'AC_INIT([intervalxt], [$VERSION], [julian.rueth@fsfe.org])'),
    ('libintervalxt/configure.ac', r'AC_INIT', r'AC_INIT([libintervalxt], [$VERSION], [julian.rueth@fsfe.org])'),
    ('pyintervalxt/configure.ac', r'AC_INIT', r'AC_INIT([pyintervalxt], [$VERSION], [julian.rueth@fsfe.org])'),
    ('pyintervalxt/src/setup.py', r'version = ', r"version = '$VERSION',"),
]

$CHANGELOG_FILENAME = 'ChangeLog'
$CHANGELOG_TEMPLATE = 'TEMPLATE.rst'
$CHANGELOG_NEWS = 'doc/news'
$CHANGELOG_CATEGORIES = ('Added', 'Changed', 'Deprecated', 'Removed', 'Fixed', 'Performance')
$PUSH_TAG_REMOTE = 'git@github.com:flatsurf/intervalxt.git'

$GITHUB_ORG = 'flatsurf'
$GITHUB_REPO = 'intervalxt'

$GHRELEASE_ASSETS = ['intervalxt-' + $VERSION + '.tar.gz']
