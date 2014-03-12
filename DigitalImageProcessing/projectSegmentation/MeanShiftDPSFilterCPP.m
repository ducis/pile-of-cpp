function [ o ] = MeanShiftDPSFilterCPP( img, spatialBandwidth, colorBandwidth, maxIteration, convergenceDistSqr )
    lp = libpointer('singlePtr',zeros([size(img,3),size(img,1)*size(img,2)],'single'));
    if(3 == size(img,3))
        calllib('FastDLL','MeanShiftDPSrgbf',lp,size(img,1),size(img,2),permute(img,[3 1 2]),spatialBandwidth,colorBandwidth,maxIteration,convergenceDistSqr);
    end
    o = permute(reshape(lp.Value,[size(img,3),size(img,1),size(img,2)]),[2,3,1]);
end

