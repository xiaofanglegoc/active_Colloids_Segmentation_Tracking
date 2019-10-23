function [f,df]=Fxdenoise(imvec,imsize,wws,siz,pis,sigs,lambda,im2vec)

[g,logprob]=evaluate_mog_log_grad(wws, siz, pis,sigs, reshape(imvec,imsize));
df=g(:)+2*lambda*(imvec-im2vec);
f=logprob+lambda*sum(imvec-im2vec).^2;
