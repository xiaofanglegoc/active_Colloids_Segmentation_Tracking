function result=RerangeDataStructure(position,track,img_dir,save_dir,display)
num_frame=500;
track_out=[];
for i=1:length(track)
    temp=zeros(1,num_frame);
    temp1=track{i}';
    temp(1:length(temp1))=temp1;
    track_out=[track_out;temp];
end
clear i;
nodes_ind=[];% [index in all, # of tracker, # of frame, centroid]
for num_track=1:size(track_out,1)
    ind_path=track_out(num_track,:);
    ind_path(find(ind_path==0))=[];
    frame=position(ind_path,end-1);
    nodes_ind=[nodes_ind;[ind_path' repmat(num_track,length(ind_path),1) frame position(ind_path,1:2)]];
end
clear num_track
%% display

Ms = [3 5]; %marker sizes
% ccc = rand(1000,3, 'double');
load ccc;
centoids=[];
track_number=[];
C=nodes_ind;
for f=1:num_frame
    
    ind=find(nodes_ind(:,3)==f);
    result{f}=[nodes_ind(ind,end-1:end)  nodes_ind(ind,3)  nodes_ind(ind,2)];
    %% display
    if display==1
        imgname=['Frame',int2str(f),'.jpg'];
        imgpath=fullfile(img_dir,imgname);
        img=imread(imgpath);
        cX=[nodes_ind(ind,2) nodes_ind(ind,end-1)];
        cY=[nodes_ind(ind,2) nodes_ind(ind,end)];
        clf
        imshow(img);
        hold on
        set(gca,'position',[0  0 1 1])
        %       plot(cX(:,end),cY(:,end),'o');
        %         hold on
        tt=unique(C(ind,2));
        for j=1:length(tt)
            
            ind1=find(C(:,2)==tt(j));
            if f>=11
                indd=find(C(ind1,3)< f & C(ind1,3)>=f-10);
            else
                indd=find(C(ind1,3)<=f);
            end
            
            tmX=C(ind1(indd),end-1);
            tmY=C(ind1(indd),end);
            ind_text=find(cX(:,1)==tt(j));
            text_X=cX(ind_text,end);
            text_Y=cY(ind_text,end);
            plot(cX(j,end),cY(j,end),'o','markersize',10,'Color',ccc(tt(j),:),'linewidth',5)
            hold on
            plot([tmX;cX(j,end)],[tmY;cY(j,end)],'-','markersize',8,'Color',ccc(tt(j),:),'linewidth',5)
            hold on
            text(5+cX(j,end),5+cY(j,end),int2str(tt(j)),'Color',ccc(tt(j),:), 'FontSize',12)
            hold on
        end
        
        save_path=[save_dir,'Frame',int2str(f),'.jpg'];
        saveas(gcf,save_path);
    end
end
%save([save_dir,'\','result.mat'],'result')
clear f
end

