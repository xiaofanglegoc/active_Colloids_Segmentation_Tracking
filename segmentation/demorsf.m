function u=demorsf(Img)
c0 = 2;
Img = double(Img(:,:,1));
        iterNum =200;
        lambda1 = 1.0;
        lambda2 = 2.0;
        nu = 0.00001*255*255;% coefficient of the length term
        initialLSF = ones(size(Img(:,:,1))).*c0;
        initialLSF(1:end-1,1:end-1) = -c0;

u = initialLSF;
% figure;imagesc(Img, [0, 255]);colormap(gray);hold on;axis off,axis equal
% title('Initial contour');
% [c,h] = contour(u,[0 0],'r');
% pause(0.1);

timestep = .1;% time step
mu = 1;% coefficient of the level set (distance) regularization term P(\phi)

epsilon = 1.0;% the papramater in the definition of smoothed Dirac function
sigma=2.0;    % scale parameter in Gaussian kernel
              % Note: A larger scale parameter sigma, such as sigma=10, would make the LBF algorithm more robust 
              %       to initialization, but the segmentation result may not be as accurate as using
              %       a small sigma when there is severe intensity inhomogeneity in the image. If the intensity
              %       inhomogeneity is not severe, a relatively larger sigma can be used to increase the robustness of the LBF
              %       algorithm.
K=fspecial('gaussian',round(2*sigma)*2+1,sigma);     % the Gaussian kernel
I = Img;
KI=conv2(Img,K,'same');     % compute the convolution of the image with the Gaussian kernel outside the iteration
                            % See Section IV-A in the above IEEE TIP paper for implementation.
                                                 
KONE=conv2(ones(size(Img)),K,'same');  % compute the convolution of Gassian kernel and constant 1 outside the iteration
                                       % See Section IV-A in the above IEEE TIP paper for implementation.

% start level set evolution
for n=1:iterNum
    u=RSF(u,I,K,KI,KONE, nu,timestep,mu,lambda1,lambda2,epsilon,1);
    if mod(n,20)==0
%         pause(0.1);
%         imagesc(Img, [0, 255]);colormap(gray);hold on;axis off,axis equal
%         [c,h] = contour(u,[0 0],'r');
        iterNum=[num2str(n), ' iterations'];
%         title(iterNum);
%         hold off;
    end
end
% imagesc(Img, [0, 255]);colormap(gray);hold on;axis off,axis equal
% [c,h] = contour(u,[0 0],'r');
% totalIterNum=[num2str(n), ' iterations'];
% title(['Final contour, ', totalIterNum]);

% figure;
% mesh(u);
% title('Final level set function');
