function [ denoised ] = NonLocalDenoiseCPP( withNoise, rectMin, rectMax, searchWindowRadius, similarityWindowRadius, h )
    szGa = 2*similarityWindowRadius+1;
    gaussianWeights = fspecial('gaussian',szGa, 0.2*szGa);
    oneOverHSquared = 1/(h.^2);
    lp = libpointer('singlePtr',zeros(rectMax-rectMin+1));
    calllib('CPP','NonLocalDenoisef',lp,size(withNoise,1),size(withNoise,2),withNoise,size(gaussianWeights,1),size(gaussianWeights,2),gaussianWeights,rectMin(1),rectMax(1),rectMin(2),rectMax(2),searchWindowRadius,similarityWindowRadius,oneOverHSquared);
    denoised = lp.Value;
end
