function [g,logprob] = evaluate_mog_log_grad(wws, siz, pis,sigs, x)
% based on code by Stefan Roth
% this actually returns negative log prob so the lower the better

  imsize=size(x);
  nfilters = size(wws, 2);

  pad_lo = ceil(0.5 * (siz - 1));
  pad_hi = floor(0.5 * (siz - 1));
  g      = zeros(size(x));  
  logprob=0;
  
  for j = 1:nfilters
    % Filter mask and mirrored filter mask
    f  = reshape(wws(end:-1:1, j), siz);
    fm = reshape(wws(:, j), siz);
    
    % Convolve and pad image appropriately
    tmp = zeros(size(x));
    tmp(1+pad_lo(1):end-pad_hi(1), 1+pad_lo(2):end-pad_hi(2)) = ...
        conv2(x, f, 'valid');
    
    [dtmp,logp]=dmog(tmp(:),pis,sigs);
    dtmp=reshape(dtmp,imsize);
    % Compute expert gradient
    tmp2 = conv2(dtmp, fm, 'same');
    g = g - tmp2;
    logprob=logprob-logp;
  end
  
