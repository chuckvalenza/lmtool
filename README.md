##lmtool: The Log-polar Mapping Tool

This project hopes to achieve a log-polar mapping of an input tiff image to an
output tiff image. This is an example of my log-polar transform (lpt) library
which performs the log-polar transform on an input pixel grid.

####Usage:

```bash
  $ bin/lmtool <input file>.tif <output file>.tif <outfile width> <outfile height>
```

example:

```bash
  $ bin/lmtool in.tif out.tif 100 100
```

####Building:
To build this software, you must be in the root directory on a linux
machine. To confirm this, run the following command in the terminal

```bash
  $ ls
```

which should have this output:

```bash
  bin  build  makefile  README.md  src
```

Next, you must ensure that the dependent libraries for the software are
installed:

```bash
  $ sudo apt-get install git libtiff5-dev automake
```

This program has several options for make. By simply using the "make"
command, you execute the "make all" command. Now that the project
dependencies are installed, you can compile with the following:

```bash
  $ make
```

If you would like to recompile the program, run the following:

```bash
  $ make clean
  $ make
```
