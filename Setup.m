addpath 'boyangCode'
base_dir='.\small_size_images1\';
img_dir=fullfile(base_dir,'image\');
gt_dir=fullfile(base_dir,'gt\');

seg_dir=fullfile(base_dir,'Seg\');
if ~exist(seg_dir)
    mkdir(seg_dir)
end
graph_dir=fullfile(base_dir,'Graph\');
if ~exist(graph_dir)
    mkdir(graph_dir)
end
result_dir=fullfile(base_dir,'Result_newGraphRefinement\');
if ~exist(result_dir)
    mkdir(result_dir)
end
SIZE=size(imread(fullfile(img_dir,'Frame1.jpg'))); 
if ~exist(fullfile(seg_dir,['\','position','.mat']))
    position=particleDetection1(img_dir,seg_dir);
else
    load(fullfile(seg_dir,['\','position','.mat']))
end
