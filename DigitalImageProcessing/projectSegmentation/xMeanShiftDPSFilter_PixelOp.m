function [ PS_out ] = MeanShiftDPSFilter_PixelOp( img,row,column,spatialBandwidth, oneOverSpatialBandwidthSquared, oneOverColorBandwidthSquared, maxIteration, convergenceDistSqr, kernel )
    from = [ row; column; squeeze(img(row,column,:)) ];
    for i=1:maxIteration
        to = JointDomainMean(img,from,spatialBandwidth,oneOverSpatialBandwidthSquared,oneOverColorBandwidthSquared,kernel);
        if(InProximity(from,to,convergenceDistSqr))
            PS_out = to(3:end);
            return;
        else
            from = to;
        end
    end
    PS_out = from(3:end);
end

function mean = JointDomainMean( img, jointDomainPoint, spatialBandwidth, oneOverSpatialBandwidthSquared, oneOverColorBandwidthSquared, kernel )
    spatial = jointDomainPoint(1:2);
    color = jointDomainPoint(3:end);
    iPos = round(spatial);
    from = iPos-spatialBandwidth;
    to = iPos+spatialBandwidth;
    %[rs,cs] = ndgrid(from(1):to(1),from(2):to(2));
    %jointDomainNeighbors = cat(3,rs,cs,img(from(1):to(1),from(2):to(2)));
    sumWeights = zeros(1,class(jointDomainPoint));
    sumWeighted = zeros(size(jointDomainPoint),class(jointDomainPoint));
    sz = [size(img,3),1];
    for r = from(1):to(1)
        for c = from(2):to(2)
            deltaSpatial = [r;c]-spatial;
            deltaColor = reshape(img(r,c,:),sz)-color;
            %w = normpdf(sum(deltaSpatial.^2)*oneOverSpatialBandwidthSquared)*normpdf(sum(deltaColor.^2)*oneOverColorBandwidthSquared);
            ds = sum(deltaSpatial.^2)*oneOverSpatialBandwidthSquared;
            a = FastNormPDF(ds,kernel);
            dc = sum(deltaColor.^2)*oneOverColorBandwidthSquared;
            b = FastNormPDF(dc,kernel);
            w = a*b;
            %w = FastNormPDF(sum(deltaSpatial.^2)*oneOverSpatialBandwidthSquared,kernel)*FastNormPDF(sum(deltaColor.^2)*oneOverColorBandwidthSquared,kernel);
            %w = kernel(min(round(abs(sum(deltaSpatial.^2)*oneOverSpatialBandwidthSquared)*1000),2000)+1)*kernel(min(round(abs(sum(deltaColor.^2)*oneOverColorBandwidthSquared)*1000),2000)+1);
            new = [r;c;reshape(img(r,c,:),sz)];
            sumWeighted = sumWeighted+w*new;
            sumWeights = sumWeights+w;
        end
    end
    mean = sumWeighted/sumWeights;
end

function inProx = InProximity( lhs, rhs, convergenceDistSqr)
    inProx = sum((lhs-rhs).^2)<convergenceDistSqr;
end

function d = FastNormPDF(x,kernel)
    %if(abs(x)>1)
    %    d = 0;
    %else
        d = kernel(min(round(abs(x)*1000),2000)+1);
    %end;
end
