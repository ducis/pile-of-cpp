I = im2single(imread('lena.bmp'));
%nIs = {imnoise(I,'salt & pepper') imnoise(I,'gaussian')};
vs = 0.004:0.003:0.016;

nIs = arrayfun(@(v)imnoise(I,'gaussian',0,v),vs, 'UniformOutput', false);
hs = transpose(10:15);
dnIs = arrayfun( @(h)cellfun( @(nI,v)NonLocalDenoiseFullCPP(nI,10,3,h*v),nIs,num2cell(vs), 'UniformOutput', false ), hs,'UniformOutput', false );
dnIs = vertcat(dnIs{:});
%dnIs = cellfun( @(nI,v)NonLocalDenoiseFullCPP(nI,10,3,12*v),nIs,num2cell(vs), 'UniformOutput', false );
show = vertcat(cell2mat([nIs;dnIs]));
mse = cellfun( @(dnI)MeanSquareError(dnI,I),dnIs )*(255^2);
psnr = cellfun( @(dnI)PSNR(dnI,I,1),dnIs );
