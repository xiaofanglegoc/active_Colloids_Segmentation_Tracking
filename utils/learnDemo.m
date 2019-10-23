%load Data/berkeleyXs;
% for demo only, make a small training set using patches from a single
% image.  
im1=imread('Data/einstein.pgm');
xs=im2col(im1,[15 15]);
xs=double(xs)/255;
xs=xs';
'calculating whitening filter'
Kwhiten=whiteningFilter(xs,[15 15]);
basis=makeBasis(Kwhiten,2);
load Data/derivPisSigs; % one set of GSM potentials
%load Data/TdistPisSigs; % another set of GSM potentials 
% 
xs=xs(1:10:end,:); % for fast training
[ws,orthVect]=basisRotation(xs,pis,sigs,basis);
showFilts(ws,[15 15]);
