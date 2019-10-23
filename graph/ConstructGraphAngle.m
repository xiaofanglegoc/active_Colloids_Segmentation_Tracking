function [edges_i,edges_j,value]=ConstructGraphAngle(position,base_dir,graph_dir,SIZE,d_con,d_neigb)

edges_i=[];
edges_j=[];
value=[];
all_num=[1:size(position,1)];
j=1;

image_bord_r=max(position(:,1));
image_bord_c=max(position(:,2));
while j<=499
%     j
    % find the current and the next frames' all points
    ind_c=find(position(:,3)==j);
    n_c=length(ind_c);
    xc_I=position(ind_c,end);meanI=mean(xc_I);
    xc_r=position(ind_c,1);
    xc_c=position(ind_c,2);
    ind_next=find(position(:,3)==j+1);
    xn_r=position(ind_next,1);
    xn_c=position(ind_next,2);
    nc_neig=[];
    for temp=1:length(ind_c)
        dc_r=repmat(xc_r(temp),1,n_c)-xc_r';
        dc_c=repmat(xc_c(temp),1,n_c)-xc_c';
        dcist=sqrt(dc_r.*dc_r+dc_c.*dc_c);
        nc_neig(temp)=numel(find(dcist<=d_neigb))-1;
    end
    
    % record the index in all points
    addc_count=numel(find(position(:,3)<=j-1));
    addn_count=numel(find(position(:,3)<=j));
    
    %  subtract the adjacent frames to find the edges and weights for graph
    n_next=length(ind_next);
    d_r=repmat(xc_r,1,n_next)-repmat(xn_r,1,n_c)';
    d_c=repmat(xc_c,1,n_next)-repmat(xn_c,1,n_c)';
    %% computer the angle distance
%     d_con=0.1;
     theta_c=atan(xc_c./xc_r);
    theta_n=atan(xn_c./xn_r);
    d_theta=repmat(theta_c,1,n_next)-repmat(theta_n,1,n_c)';
     norm_theta=abs(d_theta)./(3.14);
%      [r c]=find((norm_theta)<=0.1);
%      temp_value=norm_theta(find((norm_theta)<=0.2));
%      a_matrix=repmat((exp(-(nc_neig)/3)').*d_con,1,n_next);

%     [r c]=find((norm_theta-d_con)<=0);
    [temp_value c]=min(norm_theta,[],2);
    r=find(temp_value<0.3);
    c(find(temp_value>=0.3))=[];
      temp_value(find(temp_value>=0.3))=[];
    %% find connection
    
%     dist=sqrt(d_r.*d_r+d_c.*d_c);
%     d_matrix=repmat((exp(-(nc_neig)/3)').*d_con,1,n_next);
%     
%     [r c]=find((dist-d_matrix)<=0);
%     norm_dist=dist./d_con;
    cur_index=[1:1:n_c];
    
    %% compute correspoinding each particles' weight
%     
%     weights=[];
%     for temp=1:length(r)
%         neigbor_number=nc_neig(r(temp));
%         weights(temp)=exp(-(neigbor_number-0.3)/3);
%     end
%     
    
    %% detecting jumping frame
    
%     if numel(unique(r))./n_c<0.8
%         
%         fprintf('processing %d the frame, large threshold \n',j)
%          [r c]=find((norm_theta-2.*d_con)<=0);
% %         [r c]=find((dist-2*d_matrix)<=0);
%          
%         ind_value=find((norm_theta-2*d_con)<=0);
%          temp_value=(norm_theta(ind_value));
% %         temp_value=0.8.*norm_dist(ind_value)+0.2.*(norm_theta(ind_value));
%         temp_r=unique(r);
%         remove_id=[];
%         for k=1:length(temp_r)
%             temp_ind=find(r==temp_r(k));
%             if numel(temp_ind)>=2
%                 remove_id= [remove_id;temp_ind(find(temp_value(temp_ind)~=min(temp_value(temp_ind))))];
%             end
%         end
%         r([remove_id])=[];
%         c([remove_id])=[];
%         temp_value([remove_id])=[];
%     else
        
        ind_value=r+(c-1)*size(norm_theta,1);
         temp_value=norm_theta(ind_value);
%         temp_value=weights'.*norm_dist(ind_value)+(1-weights)'.*(norm_theta(ind_value));
        %         value=[value;temp_value];
%     end
    %%  prun the graph
% for it=1:4
%     [r c temp_value]=graph_prun(r,c,temp_value,xc_r,xc_c,xn_r,xn_c,d_con,SIZE);
% end
   
    %%
    r=r+addc_count;
    c=c+addn_count;
    edges_i=[edges_i; r];
    edges_j=[edges_j;c];
    value=[value;temp_value];
    j=j+1;
end
end

