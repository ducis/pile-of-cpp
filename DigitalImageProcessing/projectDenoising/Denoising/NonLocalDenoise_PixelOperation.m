
function [ PS_out ] = NonLocalDenoise_PixelOperation( withNoise, gaussianWeights, iRow, iColumn, rFind, rSim, oneOverHSquared, rectMin, rectMax )
    startRow = max(iRow-rFind, rectMin(1));
    endRow = min(iRow+rFind, rectMax(1));
    startColumn = max(iColumn-rFind,rectMin(2));
    endColumn = min(iColumn+rFind, rectMax(2));
    
    %[rows,columns] =  ndgrid(startRow:endRow,startColumn:endColumn);
    %similarities = arrayfun(@(r,c)NonLocalDenoise_EvaluateSimilarity(withNoise,gaussianWeights,iRow,iColumn,r,c,rSim),rows,columns);
    
    
    rows = startRow:endRow;
    columns = startColumn:endColumn;
    dist = single(zeros([endRow-startRow+1,endColumn-startColumn+1]));
    sz = [endRow-startRow+1,endColumn-startColumn+1];
    for r = 1:sz(1)
        for c = 1:sz(2)
            %similarities(r,c) = NonLocalDenoise_EvaluateDist(withNoise,gaussianWeights,iRow,iColumn,rows(r),columns(c),rSim);
                     
            neighborhoodFirst = withNoise(iRow-rSim:iRow+rSim,iColumn-rSim:iColumn+rSim); %GetNeighborhood( iRowFirst, iColumnFirst, rSim, withNoise );
            neighborhoodSecond = withNoise(rows(r)-rSim:rows(r)+rSim, columns(c)-rSim:columns(c)+rSim); %GetNeighborhood( iRowSecond, iColumnSecond, rSim, withNoise );
            wd2 = (neighborhoodFirst-neighborhoodSecond).^2.*gaussianWeights;
            
            a = sum(wd2(:));
            
            dist(r,c) = a;  %sum(wd2(:));
        end
    end
    
    unw = exp(-dist*oneOverHSquared);
    weights = unw./sum(unw(:));
    weightedColor = weights.*withNoise(startRow:endRow,startColumn:endColumn);
    PS_out = sum(weightedColor(:));
end
