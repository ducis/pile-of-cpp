function [ o ] = VisualizeSegmentation( ulimg )
    o = zeros([size(ulimg),3],'uint8');
    o(:,:,1) = mod(ulimg ,2^8);
    o(:,:,2) = mod(ulimg./(2^8),2^8);
    o(:,:,3) = mod(ulimg./(2^16),2^8);
end

