function [ denoised ] = NonLocalDenoise( withNoise, rectMin, rectMax, searchWindowRadius, similarityWindowRadius, h )
    %sz = size(withNoise);
    %[rows,columns] = ndgrid(1:sz(1),1:sz(2));
    szGa = 2*similarityWindowRadius+1;
    gaussianWeights = fspecial('gaussian',szGa, 0.2*szGa);
    oneOverHSquared = 1/(h.^2);
    [rows,columns] = ndgrid(rectMin(1):rectMax(1),rectMin(2):rectMax(2));
    denoised = arrayfun(@(r,c)NonLocalDenoise_PixelOperation(withNoise,gaussianWeights,r,c,searchWindowRadius,similarityWindowRadius, oneOverHSquared, rectMin, rectMax),rows,columns);
%    denoised = zeros(rectMax-rectMin+1);
%     for r = rectMin(1):rectMax(1)
%         for c = rectMin(2):rectMax(2)
%             denoised(r,c) = NonLocalDenoise_PixelOperation(withNoise,gaussianWeights,r,c,searchWindowRadius,similarityWindowRadius,oneOverHSquared, rectMin,rectMax);
%         end
%     end
end
