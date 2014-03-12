function [ h, f1, f2 ] = IdealLowPass( sz,cutoff )
    [f1,f2] = freqspace(sz,'meshgrid');
    Hd = zeros(sz,sz); d = sqrt(f1.^2 + f2.^2) < cutoff;
    Hd(d) = 1;
    h = fsamp2(Hd);
end

