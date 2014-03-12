function [ result ] = RegionBoundaryOverlay( regionIDMap, drawOn, boundaryVal )
    delta = [-1 0 1];
    result = drawOn;
    for r = 1:size(regionIDMap,1)
        for c = 1:size(regionIDMap,2)
            for dr = delta
                for dc = delta
                    rr = r+dr;
                    cc = c+dc;
                    if( rr>=1 && rr<=size(regionIDMap,1) && cc>=1 && cc<=size(regionIDMap,2) && regionIDMap(r,c)~=regionIDMap(rr,cc) )
                        result(r,c,:) = boundaryVal;
                        %result(r,c) = boundaryVal;
                    end
                end
            end
        end
    end
end

