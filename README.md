
# boost.exploratory

Explore incremental additions and replacements to selected boost libraries.
NOTE, this is **NOT** currently a part of boost!

## boost.additions

This subdirectory includes possible additions to selected libraries.

## boost.replacements

This subdirectory includes possible replacements (and hopefully improvements) to selected libraries.

## Make method

The top level directory contains *.imk files and a single perl script,
dirup_dir_file.pl.  The *.imk files are used in the subdirectory
Makefile's by using the [gmake
include](https://www.gnu.org/software/make/manual/html_node/Include.html)
directive where the target of the include directive is found via the
perl script.

