function [ imgOut ] = SimilarityMap( imgIn, row, column, rectMin, rectMax, h, rSim )
    szGa = 2*rSim+1;
    gW = fspecial('gaussian',szGa,0.2*szGa);
    oneOverHSquared = 1/(h.^2);
    [rows,columns] = ndgrid(rectMin(1):rectMax(1),rectMin(2):rectMax(2));
    unw = exp(-arrayfun(@(r,c)NonLocalDenoise_EvaluateSimilarity(imgIn,gW,row,column,r,c,rSim),rows,columns)*oneOverHSquared);
    imgOut = unw./sum(sum(unw));
end

