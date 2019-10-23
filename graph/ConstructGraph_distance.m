function [edges_i,edges_j,value]=ConstructGraph_distance(position,base_dir,graph_dir,SIZE)
edges_i=[];
edges_j=[];
value=[];
d_th=20;% max displacement
% d_th1=20;% max displacement
% load(fullfile('./small_size_images1/data',['position','.mat']));

all_num=[1:size(position,1)];
j=1;

while j<=(position(end,end-1)-1)
    % while j<=99
    %
    % find the current and the next frames' all points
    ind_c=find(position(:,end-1)==j);
    xc_r=position(ind_c,1);
    xc_c=position(ind_c,2);
    xc_I=position(ind_c,end);
    ind_next=find(position(:,end-1)==j+1);
    xn_r=position(ind_next,1);
    xn_c=position(ind_next,2);
    xn_I=position(ind_next,end);

    addc_count=numel(find(position(:,end-1)<=j-1));
    addn_count=numel(find(position(:,end-1)<=j));
    
    %  subtract the adjacent frames to find the edges and weights for graph
    n_c=length(ind_c);
    n_next=length(ind_next);
    d_r=repmat(xc_r,1,n_next)-repmat(xn_r,1,n_c)';
    d_c=repmat(xc_c,1,n_next)-repmat(xn_c,1,n_c)';
    d_I=repmat(xc_I,1,n_next)-repmat(xn_I,1,n_c)';
    dist=sqrt(d_r.*d_r+d_c.*d_c+100*d_I);
    [r c]=find(dist<d_th);
    ind_value=find(dist<d_th);
    %% detecting jumping frame
    
    if numel(unique(r))./n_c<0.8
        fprintf('processing %d the frame, large threshold \n',j)
        [r c]=find(dist<2.*d_th);
        ind_value=find(dist<2.*d_th);
    else
        [r c]=find(dist<d_th);
    end
    % this is the edge weight
    if length(r)==length(ind_value) & length(c)==length(ind_value)
        temp_value=dist(ind_value);
%         value=[value;dist(ind_value)];
    else
        j
        error('value is not with the same length of index')
    end
    
      %%  prun the graph
      d_con=20;

for it=1:7
    [r c temp_value]=graph_prun(r,c,temp_value,xc_r,xc_c,xn_r,xn_c,d_con,SIZE);
end
%%

    r=r+addc_count;
    c=c+addn_count;
    edges_i=[edges_i; r];
    edges_j=[edges_j;c];
    value=[value;temp_value];
    j=j+1;
end

% dlmwrite(fullfile(graph_dir,['graph_detecting_frames.txt']),[edges_i edges_j value])
% SparseGraph=sparse(edges_i,edges_j,value,all_num(end),all_num(end));
end

