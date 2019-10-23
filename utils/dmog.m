function [yy,logprob]=dmog(xx,pis,sigs)
% calculate derivative of log (mog (xx))

  
% return derivative of  mixture with priors pis, means mus, and variances
% sigs
  

  
  siz=size(xx);
  xx=xx(:);
  pis=pis(:);
  sigs=sigs(:);
  
 ss=1./sigs.^2;
 
 err=exp(-0.5*xx.^2*(ss)');
 err=err*diag(pis./sigs);
  prob=sum(err,2);
  
  
  weight=(err*ss)./prob;
  yy=-weight.*xx;
  yy=reshape(yy,siz);
  logprob=sum(log(prob));

 
