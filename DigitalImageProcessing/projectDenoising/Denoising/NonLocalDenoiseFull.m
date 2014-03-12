function [ denoised ] = NonLocalDenoiseFull( withNoise, searchWindowRadius, similarityWindowRadius, h )
    s(1:2) = similarityWindowRadius;
    denoised = NonLocalDenoise(padarray(withNoise,s,'replicate'),s+1,s+size(withNoise),searchWindowRadius, similarityWindowRadius, h );
end

