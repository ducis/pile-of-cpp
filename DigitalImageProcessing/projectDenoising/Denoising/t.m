[withNoise,denoised] = test1(imread('airplane.bmp'),[0.1;0.2;0.3],[0.1 0.1;0.3 0.3],[3,10],[3 3],[0.2,2; 0.3,2; 0.4,2; 0.5,2; 0.6,2]);
imwrite(cell2mat([withNoise,denoised]),'t.png');