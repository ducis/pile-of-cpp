function [ filtered ] = MeanShiftDPSFilter( img, rectMin, rectMax, spatialBandwidth, colorBandwidth, maxIteration, convergenceDistSqr )
    [rows,columns] = ndgrid(rectMin(1):rectMax(1),rectMin(2):rectMax(2));
    kernel = cast(normpdf(0:0.001:2,0,1),class(img));
    kernel(1000:end) = 0;
    rectMin = cast(rectMin,class(img));
    rectMax = cast(rectMax,class(img));
    spatialBandwidth = cast(spatialBandwidth,class(img));
    colorBandwidth = cast(colorBandwidth,class(img));
    maxIteration = cast(maxIteration,class(img));
    convergenceDistSqr = cast(convergenceDistSqr,class(img));
    rows = cast(rows,class(img));
    columns = cast(columns,class(img));
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

