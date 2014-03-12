function [ denoised ] = NonLocalDenoiseFullCPP( withNoise, searchWindowRadius, similarityWindowRadius, h )
    s(1:2) = similarityWindowRadius;
    denoised = NonLocalDenoiseCPP(padarray(withNoise,s,'replicate'),s+1,s+size(withNoise),searchWindowRadius, similarityWindowRadius, h );
end

