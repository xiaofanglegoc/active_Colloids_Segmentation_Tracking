function [im2,logP]=minimizedenoise(im,wws,siz,pis,sigs,sigma,nIter)


im2=im;
lambda=1/(2*sigma^2)
im2vec=im2(:);
imvec=im(:);
imsize=size(im);

[im2,logP]=minimize(imvec,'Fxdenoise',nIter,imsize,wws,siz,pis,sigs,lambda,im2vec);
im2=reshape(im2,imsize);




