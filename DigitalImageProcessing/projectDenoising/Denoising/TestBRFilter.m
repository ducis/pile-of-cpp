
function f = TestBRFilter(lo,hi)
    [f1,f2] = freqspace(65,'meshgrid');
    r2 = f1.^2+f2.^2;
    Hd = (r2<(lo^2))|(r2>hi^2);
    f = fsamp2(Hd);
end