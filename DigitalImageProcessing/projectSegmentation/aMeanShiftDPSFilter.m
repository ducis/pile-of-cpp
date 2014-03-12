function [ filtered ] = MeanShiftDPSFilter( img, rectMin, rectMax, spatialBandwidth, colorBandwidth, maxIteration, convergenceDistSqr )
    [rows,columns] = ndgrid(rectMin(1):rectMax(1),rectMin(2):rectMax(2));
    kernel = single(normpdf(0:0.001:2,0,1));
    kernel(1000:end) = 0;
    o = arrayfun(@(r,c)MeanShiftDPSFilter_PixelOp(img,r,c,spatialBandwidth,1/spatialBandwidth^2,1/colorBandwidth^2, maxIteration, convergenceDistSqr,kernel),rows,columns,'UniformOutput',false);
    %filtered = o;
    %filtered = reshape(cell2mat(o),[rectMax-rectMin+1,size(img,3)]);
    filtered = zeros([rectMax-rectMin+1,size(img,3)],class(img));
    for r = 1:size(filtered,1)
        for c = 1:size(filtered,2)
            filtered(r,c,:) = o{r,c}; 
        end; 
    end;
end

