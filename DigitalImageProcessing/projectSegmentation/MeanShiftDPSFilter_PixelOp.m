function [ PS_out ] = MeanShiftDPSFilter_PixelOp( img,row,column,spatialBandwidth, oneOverSpatialBandwidthSquared, oneOverColorBandwidthSquared, maxIteration, convergenceDistSqr, kernel )
    from = img(row,column,:);
    for i=1:maxIteration
        [rr,cc,to] = JointDomainMean(img,row,column,from,spatialBandwidth,oneOverSpatialBandwidthSquared,oneOverColorBandwidthSquared,kernel);
        %if(InProximity(from,to,convergenceDistSqr))
        %    PS_out = to;
        %    return;
        %else
            from = to;
            row = rr;
            column = cc;
        %end
    end
    PS_out = squeeze(from);
end

function [meanRow,meanColumn,meanColor] = JointDomainMean( img, row, column, color, spatialBandwidth, oneOverSpatialBandwidthSquared, oneOverColorBandwidthSquared, kernel )
    iPos = round([row,column]);
    from = max(iPos-spatialBandwidth,[1 1]);
    to = min(iPos+spatialBandwidth,[size(img,1) size(img,2)]);
    %[rs,cs] = ndgrid(from(1):to(1),from(2):to(2));
    %jointDomainNeighbors = cat(3,rs,cs,img(from(1):to(1),from(2):to(2)));
    tp = class(img);
    sumWeights = zeros(1,tp);
    sumWeightedRowCoords = zeros(1,tp);
    sumWeightedColCoords = zeros(1,tp);
    sumWeightedColors = zeros(size(color),tp);
    sz = numel(color);
    %cache = reshape(img(from(1):to(1),from(2):to(2),:),(to-from+1).*[sz,1]);
    for r = from(1):to(1)
        for c = from(2):to(2)
            %w = normpdf(sum(deltaSpatial.^2)*oneOverSpatialBandwidthSquared)*normpdf(sum(deltaColor.^2)*oneOverColorBandwidthSquared);
            ds = ((r-row)^2+(c-column)^2)*oneOverSpatialBandwidthSquared;
            %a = FastNormPDF(ds,kernel);
            a = 0.75*(1-ds^2);%Epanechnikov
            %newColor = color;
            %for i=1:sz
            %    newColor(i) = img(r,c,i);
            %end
            newColor = img(r,c,:);
            vColor = newColor-color;
            %dc = sum(vColor.^2)*oneOverColorBandwidthSquared;
            dc = vColor(1);
            for i=2:sz 
                dc = dc+vColor(i);
            end
            dc = dc*oneOverColorBandwidthSquared;
            %b = FastNormPDF(dc,kernel);
            b = 0.75*(1-dc^2);%Epanechnikov            
            w = a*b;
            %w = FastNormPDF(sum(deltaSpatial.^2)*oneOverSpatialBandwidthSquared,kernel)*FastNormPDF(sum(deltaColor.^2)*oneOverColorBandwidthSquared,kernel);
            %w =
            %kernel(min(round(abs(sum(deltaSpatial.^2)*oneOverSpatialBandwidthSquared)*1000),2000)+1)*kernel(min(round(abs(sum(deltaColor.^2)*oneOverColorBandwidthSquared)*1000),2000)+1);
            sumWeightedRowCoords = sumWeightedRowCoords + w*r;
            sumWeightedColCoords = sumWeightedColCoords + w*c;
            sumWeightedColors = sumWeightedColors + w*newColor;
            sumWeights = sumWeights+w;
        end
    end
    meanRow = sumWeightedRowCoords/sumWeights;
    meanColumn = sumWeightedColCoords/sumWeights;
    meanColor = sumWeightedColors/sumWeights;
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
