lmtool
    Author: Chuck Valenza

Abstract:

The human visual system (HVS) utilizes cone photoreceptors for photopic (high  
light level) vision.  It is well known that the distribution of cones in the  
HVS is not uniform, but rather non-uniform which has often been modeled as a  
conformal logarithmic mapping. When the distribution of cone photoreceptors is  
mapped to a logarithmic polar (log-polar) coordinate space, it is noted that  
the density of cone photoreceptors follows the axis rho (the axis moving  
outwards from the center). This concentration of cone photoreceptors allows a  
high resolution of pertinent data at the foveal point while still maintaining  
a wide periphery of under sampled data.  By applying the same techniques that  
the human visual system utilizes to interpret real-time data, we hope to  
achieve an efficient image processing algorithm which can be converted into  
an easily parsed format for machine interpretation. This project is a  
component of a larger research effort by Professor Richard A. Messner to  
study the processing channels that occur in a afferent portion of the HVS.  


Usage:

    bin/lmtool <input file>.tif <output file>.tif <outfile width> <outfile height>

    example:
        bin/lmtool sample_images/camera_man.tif sample_images/camera_man_out.tif 100 100

Building:
    To build this software, you must be in the root directory on a linux
    machine. To confirm this, run the following command in the terminal

       $ ls

    which should have this output:

       bin  build  makefile  README.md  src

    Next, you must ensure that the dependent libraries for the software are
    installed:

        $ sudo apt-get install git libtiff5-dev automake

    This program has several options for make. By simply using the "make"
    command, you execute the "make all" command. Now that the project
    dependencies are installed, you can compile with the following:

        $ make

    If you would like to recompile the program, run the following:

        $ make clean
        $ make

