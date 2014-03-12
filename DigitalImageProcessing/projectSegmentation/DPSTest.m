I = im2single(imread('baboon.bmp'));
O = [ 
    MeanShiftDPSFilterCPP(I,4,0.2,500,1e-8), MeanShiftDPSFilterCPP(I,4,0.4,500,1e-8);
    MeanShiftDPSFilterCPP(I,8,0.2,500,1e-8), MeanShiftDPSFilterCPP(I,8,0.4,500,1e-8);
    MeanShiftDPSFilterCPP(I,16,0.2,500,1e-8), MeanShiftDPSFilterCPP(I,16,0.4,500,1e-8)
    ];
imwrite(O,'DPSTest.png');
imtool(I);
imtool(O);
