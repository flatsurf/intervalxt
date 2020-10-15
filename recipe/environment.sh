#!/bin/bash

# ##################################################################
# Source this file to setup additional environment variables for the
# conda-build environment.
# ##################################################################

export SNIPPETS_DIR=`realpath "${SNIPPETS_DIR:-$SRC_DIR/recipe/snippets}"`
export GITHUB_ORGANIZATION=flatsurf
export GITHUB_REPOSITORY=intervalxt

source $SNIPPETS_DIR/cxx/environment.sh
source $SNIPPETS_DIR/make/environment.sh

export SUBDIRS=libintervalxt:pyintervalxt
