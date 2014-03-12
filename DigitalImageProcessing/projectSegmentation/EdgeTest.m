names = { 'butterfly.jpg'; 'cougar.jpg'; 'chair.jpg'; 'helicopter.jpg'; 'baboon.bmp' };
hr = [0.15 0.15 0.15 0.15 0.25];
for i = 1:5 
    I = im2single(imread(names{i}));
    [A,B] = MeanShiftEdgeCPP(I,8,hr(i),500,1e-8);
    R = [A;im2single(B)];
    imwrite(R,[names{i} 'EdgeTest.png']);
    imtool(R);
end
