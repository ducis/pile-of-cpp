function [ mse ] = MeanSquareError( lhs,rhs )
    mse = sum(sum((lhs-rhs).^2))/numel(lhs);
end

