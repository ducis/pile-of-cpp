function [ o,pruned ] = MeanShiftSegmentationCPP( img, spatialBandwidth, colorBandwidth, minRegionArea, maxIteration, convergenceDistSqr )
    lp = libpointer('uint32Ptr',zeros([size(img,1),size(img,2)],'uint32'));
    if(3 == size(img,3))
        calllib('FastDLL','MeanShiftSegmentationrgbf',lp,size(img,1),size(img,2),permute(img,[3 1 2]),spatialBandwidth,colorBandwidth,minRegionArea,maxIteration,convergenceDistSqr);
    end
    o = lp.Value;
    calllib('FastDLL','PruneSmallRegionsul',lp,size(img,1),size(img,2),o,minRegionArea);
    pruned = lp.Value;
end

