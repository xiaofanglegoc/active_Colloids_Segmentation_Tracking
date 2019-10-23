function dist = mydistance_centroid(bboxesDetect, target, VOCscore )

% if(strcmp('VOCscore',typeComp))
   
   rA=bboxesDetect(1);
   cA=bboxesDetect(2);
   rT=target(1);
   cT=target(2);
   dist =sqrt((rA-rT).^2+(cA-cT).^2);

 dist=exp(-dist/15);
   if dist<0.01
        dist=0;
   else
       dist=dist;
   end
       

end