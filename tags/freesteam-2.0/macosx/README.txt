This folder contains a Mac OS X PackageMaker document, allowing freesteam to be packaged
for use on Mac OS X. This package has not been extensively tested across different OS X
versions.

For this document to work, you need to

cd ~/freesteam
scons INSTALL_PREFIX=/usr/local
scons INSTALL_ROOT=~/freesteam/temp_install install

Then open freesteam.pmdoc using PackageMaker, and build and run.

The package currently contains the Python language bindings but not the ASCEND bindings.

