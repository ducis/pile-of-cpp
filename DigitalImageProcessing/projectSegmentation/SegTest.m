names = { 'butterfly.jpg'; 'cougar.jpg'; 'chair.jpg'; 'helicopter.jpg' };
for i = 1:4 
    I = im2single(imread(names{i}));
    [A,B] = MeanShiftSegmentationCPP(I,8,0.15,20,500,1e-8);
    O = VisualizeSegmentation([A,B]);
    P = RegionBoundaryOverlay([A,B],[I,I],[1 0 0]);
    R = [I,I;im2single(O);P];
    imwrite(R,[names{i} 'SegTest.png']);
    imtool(R);
end
