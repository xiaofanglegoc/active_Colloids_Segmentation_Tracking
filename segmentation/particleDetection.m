function position=particleDetection(img_dir,seg_dir)
detection_dir=fullfile(seg_dir,'detection');mkdir(detection_dir)
addpath(genpath('utils'))
m=17;
num_frame=500;
imgname=['Frame',int2str(1),'.jpg'];
imgpath=fullfile(img_dir,imgname);
rimg=imread(imgpath);
temp=zeros(size(rimg));
for j=1:num_frame
    imgname=['Frame',int2str(j),'.jpg'];
    imgpath=fullfile(img_dir,imgname);
    rimg=imread(imgpath);
    temp=temp+im2double(rimg);
end

background=temp./num_frame;
clear temp rimg
position=[];
for i=1:num_frame
    
    imgname=['Frame',int2str(i),'.jpg'];
    imgpath=fullfile(img_dir,imgname);
    img=imread(imgpath);
    deimg=denoiseBRFOE(img,m);
    sub_img=abs(im2double(deimg)-background);%% Detect cycles with imfindcircle
    h2=adapthisteq(sub_img);
    filter_img=filter2(fspecial('disk',3),h2);
    [centers,radii]= imfindcircles( filter_img,[5 6],'ObjectPolarity','bright',...
        'Sensitivity',0.93,'Method','twostage');
    %     clear deimg h2 filter_img ;
    %% Segmentation with RSF and combine with find circle
    seg_img=demorsf(sub_img.*255);
    [L N]=bwlabel(im2bw(seg_img,0),8);
    c1=regionprops(L,'Centroid');
    c1=struct2cell(c1);
    c1=cell2mat(c1);
    centers1=[c1(1:2:end-1)' c1(2:2:end)'];
    radii1=regionprops(L,'MinorAxisLength');
    radii1=struct2cell(radii1);
    radii1=cell2mat(radii1)';
    radii2=regionprops(L,'MajorAxisLength');
    radii2=struct2cell(radii2);
    radii2=cell2mat(radii2)';
    ind=unique([find(radii1<=2);find((radii2-radii1)>=5)]);
    centers1(ind,:)=[];
    radii1(ind)=[];
    clear ind
  addcenter=[];
  addr=[];
%     if length(radii)<length(radii1)
        for k=1:length(radii1)
            ind=find(sqrt(sum((repmat(centers1(k,:),length(radii),1)-centers).^2,2))<mean(radii));
            b= round(centers1(k,:));
            n=L(b(2),b(1));
%             clear b n;
            if length( ind)<1
                addcenter=[addcenter; centers1(k,:)];
                addr=[addr; 0.7.*radii1(k)];
                centers=[centers; centers1(k,:)];
                radii=[radii; 0.7.*radii1(k)];
            end
        end
%     else
        
%         for k=1:length(radii1)
% %             if abs(radii1(k)-mean(radii1))<4
%                 ind=find(sqrt(sum((repmat(centers1(k,:),length(radii),1)-centers).^2,2))<mean(radii));
%                 b= round(centers1(k,:));
%                 n=L(b(2),b(1));
%                 clear b n;
%                 if length( ind)<1
%                     centers=[centers; centers1(k,:)];
%                     radii=[radii; 0.7.*radii1(k)];
%                 end
% %             end
%         end
%     end
%     
%    
%     mean_Ins=[];frame_ind=[];
%     for k=1:length(radii)
%         bw_boundary=create_shifted_circle_c(p,q,radii(k),centers(k,2),centers(k,1));
%         detected_region=imfill(bw_boundary,'holes');
%         if numel(find(detected_region)==1)>0.8*3.14*mean(radii).^2
%             
%             x=detected_region.*im2double(img);%,'MeanIntensity');
%             %         figure(k)
%             %         imshow(x)
%             ind=find(x>0);
%             num_ind=numel(ind);
%             mean_Ins(k)=sum(x(ind))./num_ind;
%         else
%             mean_Ins(k)=NaN;
%         end
%         frame_ind(k)=i;
%     end
     [p,q]=size(img);
    Size=[p,q];
    fig = figure(1);
    imshow(img);hold on ;
    viscircles(centers,radii,'EdgeColor','b','LineWidth',2)
    if ~isempty(addcenter)
        viscircles(addcenter,addr,'EdgeColor','r','LineWidth',2)
    end
    pos = get(fig, 'Position');
    set(fig, 'Units', 'pixels', 'Position', ...
        [pos(1), pos(2)+pos(4)-Size(1), ...
        Size(2), Size(1)]);
    set(gca,'Position', [0 0 1 1], 'Visible', 'off');
    %         pause(0.1)
    
    filename3=fullfile(detection_dir,['Frame',int2str(i),'.jpg']);
    saveas(gcf,filename3)
    hold off
    close all
    
    position=[position ;[centers repmat(i, length(radii),1)]];
    clear rimg p q seg_img L
end
% save('-v7',fullfile(seg_dir,['position','.mat']),'position')
end
