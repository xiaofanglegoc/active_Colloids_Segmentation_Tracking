function [ws,orthVect]=basisRotation(xs,pis,sigs,basis,nVects)
% find approximate MLE filters by rotating a basis
% xs - the training set of patches
% pis,sigs - the GSM parameters
% basis - the basis to be rotated

if ~exist('nVects','var')
    nVects=size(basis,2);
end

siz=[sqrt(size(basis,1)) sqrt(size(basis,1))];

ws=zeros(size(basis,1),nVects);
orthVect=0*basis(1,:);
orthVect=orthVect';
for i=1:nVects
    disp(sprintf('calculating filter %d',i));
    [w,u]=basisRotation1(xs,pis,sigs,orthVect,basis);
    ws(:,i)=w;
    orthVect=[orthVect u];
end

if (nVects<size(basis,2))
    % complete orthVect to full rotation
    orthVect2=null(orthVect');
    ws2=basis*orthVect2;
    K=equivalentFilter(ws2,siz);
    ws=[ws K(:)];
%    ws=[ws ws2];
    orthVect=[orthVect orthVect2];
end
%keyboard;
