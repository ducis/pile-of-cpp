function [withNoise,denoised] = test1(original,snpParams,gaussianParams,ssParams,medParams,fdParams)
    withNoise = [
        SnPTest(original,snpParams);
        GaussianTest(original,gaussianParams)
        ];
%     denoised = cell(size(withNoise,1),size(ssParams,1)+size(medParams,1)+size(fdParams,1));
%     for i=1:size(withNoise)
%        denoised(i,:) = DenoiseTest(original,withNoise{i},ssParams,medParams,fdParams);
%     end;
    %denoised = cellfun( @(wn)DenoiseTest(original,wn,ssParams,medParams,fdParams), withNoise);
    denoised = cellfun( @(wn)DenoiseTest(original,wn,ssParams,medParams,fdParams), withNoise, 'UniformOutput', false );
    denoised = vertcat(denoised{:});
    %r = {withNoise denoised};
end

function [withNoise] = SnPTest(original,params)
%     withNoise = cell(size(params,1),1);
%     for i=1:size(params)
%         withNoise{i} = imnoise(original,'salt & pepper',params(i));
%     end;
    withNoise = arrayfun( @(p)imnoise(original,'salt & pepper',p),params, 'UniformOutput', false);
end

function withNoise = GaussianTest(original,params)
%     withNoise = cell(size(params,1),1);
%     for i=1:size(params)
%         withNoise{i} = imnoise(original,'gaussian',params(i,1),params(i,2));
%     end;
    withNoise = arrayfun( @(m,v)imnoise(original,'gaussian',m,v),params(:,1),params(:,2), 'UniformOutput', false);
end

function [denoised,PSNR] = DenoiseTest(original,withNoise,ssParams,medParams,fdParams)
    denoised = [
        SpatialSmoothTest(withNoise,ssParams)  MedianFilterTest(withNoise,medParams)  FrequencyDomainFilterTest(withNoise,fdParams)
        ];
    %PSNR = ComputePSNR(original,denoised);
end

function filtered = MedianFilterTest(withNoise,params)
%    filtered = cell(1,size(params,1));
%    for i=1:size(params)
%        filtered{i} = medfilt2(withNoise,params(i,1:2));
%    end;
    filtered = transpose(arrayfun( @(m,n)medfilt2(withNoise,[m n]), params(:,1), params(:,2), 'UniformOutput', false ));
end

function filtered = SpatialSmoothTest(withNoise,params)
    %filtered = transpose(arrayfun( @(m,n)medfilt2(withNoise,[m n]),
    %params(:,1), params(:,2), 'UniformOutput', false ));
    filtered = transpose(arrayfun( @(h,s)imfilter(withNoise,fspecial('gaussian',h,s),'replicate'), params(:,1), params(:,2), 'UniformOutput', false ));
end

function filtered = FrequencyDomainFilterTest(withNoise, params)
    filtered = transpose(arrayfun( @(lo,hi)imfilter(withNoise,TestNotchFilter(lo,hi),'replicate'), params(:,1), params(:,2), 'UniformOutput', false ));
end

