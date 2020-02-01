#!/bin/bash
set -ex

source $RECIPE_DIR/environment.sh

cd libintervalxt
$SNIPPETS_DIR/make/install.sh
