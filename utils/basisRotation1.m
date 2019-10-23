function [w,u]=basisRotation1(xs,pis,sigs,orthVect,basis,initW)
% find best projection using iterated PCA
% here we get as input a basis matrix, and learn eigenvectors w=basis*u
% where u is an orthogonal matrix. This makes sure that log Z(w) = log Z(basis)


[npoints,nfeatures]=size(xs);
nfeatures=size(basis,2);
if ~exist('initW')
    u=randn(nfeatures,1);
    u=u/norm(u);
    w=basis*u;
end

if ~exist('orthVect')
    orthVect=0;
end


[uu,ss,vv]=svd(orthVect*orthVect');
I=find(diag(ss)<0.0000000001);
L=uu(:,I);

%


err=zeros(npoints,length(sigs));
nIter=50;

wOld=0*w;
for i=1:nIter
    % E step
    f=xs*w;
    for k=1:length(sigs)
        err(:,k)=(pis(k)/sigs(k))*exp(-f.^2/(2*sigs(k).^2));
    end
    prob=sum(err,2);
    for k=1:length(sigs)
         err(:,k)=err(:,k)./prob;
    end

    if (max(abs(w-wOld))<0.000001)
      'converged'
      break;
    end
    weight=sum((err*diag(1./sigs.^2)),2);
    
    W=sparse(1:length(f),1:length(f),weight);
    % and now M step
    cov=xs'*W*xs;
    
   
   cov2=basis'*cov*basis;
   
    [uu,ss,vv]=svd(L'*cov2*L);
    u=L*uu(:,end);
    wOld=w;
    w=basis*u;
   
end

