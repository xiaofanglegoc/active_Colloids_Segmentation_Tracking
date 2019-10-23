clear all
clc
close all
%% parameter setup
addpath 'ssp_maxflow'
addpath 'segmentation'
addpath 'eval'
addpath 'utils'
d_neigb=20;%25
d_con=50;%20
cur=pwd
data_dir=fullfile(cur,'/data');
%% Segmentation and detecion colloids 
img_dir=fullfile(data_dir,'images/');
gt_dir=fullfile(data_dir,'GT/');
position=particleDetection1(img_dir,seg_dir);
%% only with distance
%  [edges_i,edges_j,value]=ConstructGraph_distance(position,base_dir,graph_dir,SIZE);

%% only with direction
%[edges_i,edges_j,value]=ConstructGraphAngle(position,base_dir,graph_dir,SIZE,d_con,d_neigb);
%% with direction and distance
% [edges_i,edges_j,value]=ConstructGraphDistanceAngle(position,base_dir,graph_dir,SIZE,d_con,d_neigb);
[edges_i,edges_j,value]=ConstructGraphDistanceAngle_conflict_constraint(position,base_dir,graph_dir,SIZE,d_con,d_neigb);
SparseGraph=sparse(edges_i,edges_j,value);
ex_track=mexBallTrackingMinCostFlowAdjListHeapMultiPass(SparseGraph);
result=RerangeDataStructure(position,ex_track,img_dir,result_dir,0);
evaluateMyMOT;
%save('-v7',fullfile(result_dir,['evaluate_ours.mat']),'ClearMOT')
%save('-v7',fullfile(result_dir,['trackout_ours.mat']),'ex_track')
%save('-v7',fullfile(result_dir,['result_ous.mat']),'result')
