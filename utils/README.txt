This directory contains Matlab code implementing the algorithms described in
" What makes a good model of natural images ?"
    Yair Weiss and William T. Freeman
    CVPR 2007 
The paper can be downloaded at:
http://www.cs.huji.ac.il/~yweiss/foe-final.pdf

The code is released for academic research only under the Lesser
General Public License.  This package is a free software as described
in the LGPL license.

The code includes minimize.m, a conjugate gradient optimizer
written by Carl Edward Rasmussen. We downloaded it from:
http://www.kyb.tuebingen.mpg.de/bs/people/carl/code/minimize/

The denoising code is based on modifying the code written
by Stephan Roth. 

There are two basic demos: learnDemo.m and denoiseDemo.m which
should be self explanatory. The learning demo runs on a small
training set. The training set we actually used can be downloaded
at:
http://www.cs.huji.ac.il/~yweiss/berkeleyXs.mat


