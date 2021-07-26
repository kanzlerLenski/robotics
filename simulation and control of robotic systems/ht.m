% function to compute transformation matrix from one frame to another.
% coordinates.
function [T] = ht(a, alpha, d, q)

% for a better representation, since cos(x) in MATLAB returns a really
% small number instead of zero.
if (q == pi/2) || (q == -pi/2)
    c_q = 0;
else
    c_q = cos(q);
end

if (q == pi) || (q == -pi)
    s_q = 0;
else
    s_q = sin(q);
end

if (alpha == pi/2) || (alpha == -pi/2)
    c_alpha = 0;
else
    c_alpha = cos(alpha);
end

if (alpha == pi) || (alpha == -pi)
    s_alpha = 0;
else
    s_alpha = sin(alpha);
end

% computation of the transformation matrix.
T = [c_q -s_q*c_alpha s_q*s_alpha a*c_q;
    s_q c_q*c_alpha -c_q*s_alpha a*s_q;
    0 s_alpha c_alpha d;
    0 0 0 1];

end

