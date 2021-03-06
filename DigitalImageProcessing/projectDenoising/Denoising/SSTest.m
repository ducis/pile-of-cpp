I = im2single(imread('lena.bmp'));
%nIs = {imnoise(I,'salt & pepper') imnoise(I,'gaussian')};
ds = 0.01:0.02:0.09;
vs = 0.004:0.003:0.016;
nIs = [ arrayfun(@(d)imnoise(I,'salt & pepper',d),ds, 'UniformOutput', false) arrayfun(@(v)imnoise(I,'gaussian',0,v),vs, 'UniformOutput', false) ];
hs = transpose(3:2:31);
dnIs = arrayfun( @(h)cellfun( @(nI)imfilter(nI,fspecial('gaussian',h,0.2*h),'replicate'),nIs, 'UniformOutput', false ), hs,'UniformOutput', false );
dnIs = vertcat(dnIs{:});
show = vertcat(cell2mat([nIs;dnIs]));
mse = cellfun( @(dnI)MeanSquareError(dnI,I),dnIs )*(255^2);
psnr = cellfun( @(dnI)PSNR(dnI,I,1),dnIs );
