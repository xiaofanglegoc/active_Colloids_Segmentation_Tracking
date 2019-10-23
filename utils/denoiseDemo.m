im1=imread('Data/einstein.pgm');
    im1=double(im1)/255;
    im1Clean=im1;
    sigma=0.05;
    load Data/derivPisSigs;% the GSM parameters
    load Data/learntFromBerkely.mat;
      siz=[15 15];
% for Roth and Black filters % normalized to have unit norm
%    load Data/rothFilts.mat; 
 %   siz=[5 5];
    
    figure(1);
    showFilts(ws,siz);
   

%     im1=im1+sigma*randn(size(im1));
    
    [im3,logP]=minimizedenoise(im1,ws,siz,pis,sigs,sigma,20);
    % note denoising results can be made better by changing sigma 
    % 
     figure(2);
      show([im1 im3]);
      % calculate PSNR while ignoring edges
      imCleanS=im1Clean(16:end-16,16:end-16);
      im3S=im3(16:end-16,16:end-16);
    psnr = 20 * log10(1 / std(imCleanS(:)-im3S(:)))
    