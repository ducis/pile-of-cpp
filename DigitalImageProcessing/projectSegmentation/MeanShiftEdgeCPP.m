function [ o,b ] = MeanShiftEdgeCPP( img, spatialBandwidth, colorBandwidth, maxIteration, convergenceDistSqr )
    lp = libpointer('singlePtr',zeros([size(img,1),size(img,2)],'single'));
    if(3 == size(img,3))
        calllib('FastDLL','MeanShiftEdgenessMaprgbf',lp,size(img,1),size(img,2),permute(img,[3 1 2]),spatialBandwidth,colorBandwidth,maxIteration,convergenceDistSqr);
    end
    o = lp.Value;
    o = o/max(max(o));
    b = im2bw(o,0.45);
end

