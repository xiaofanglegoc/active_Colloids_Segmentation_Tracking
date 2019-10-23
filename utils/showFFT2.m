function []=showFFT2(ws,siz)

epsilon=0.0001;
isize=128;

 im1=zeros(isize);
      im1(round(isize/2),round(isize/2))=1;
        
        imTotal=epsilon*im1;
        
        for i=1:size(ws,2)
            K=reshape(ws(:,i),siz);
            imNew=conv2(im1,K,'same');
            imNew=conv2(imNew,flipud(fliplr(K)),'same');
            imTotal=imTotal+imNew;
            figure(1);
            show(K);
            figure(2);
            show(fftshift(abs(fft2(imNew))));
            figure(3);
            show(fftshift(abs(fft2(imTotal))));
            pause;
        end
   