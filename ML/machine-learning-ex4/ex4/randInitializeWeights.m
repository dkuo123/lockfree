function W = randInitializeWeights(L_in, L_out)
%RANDINITIALIZEWEIGHTS Randomly initialize the weights of a layer with L_in
%incoming connections and L_out outgoing connections
%   W = RANDINITIALIZEWEIGHTS(L_in, L_out) randomly initializes the weights 
%   of a layer with L_in incoming connections and L_out outgoing 
%   connections. 
%
%   Note that W should be set to a matrix of size(L_out, 1 + L_in) as
%   the first column of W handles the "bias" terms
% theta(W)  = matrix of L_out x (1+L_in);

% You need to return the following variables correctly 
W = zeros(L_out, 1 + L_in);

% ====================== YOUR CODE HERE ======================
% Instructions: Initialize W randomly so that we break the symmetry while
%               training the neural network.
%
% Note: The first column of W corresponds to the parameters for the bias unit
%

%randomly select values for (l) uniformly in the range [-epsilon; epsilon].
% Randomly initialize the weights to small values
epsilon = 0.12;
% good suggestion is epsilon = sqrt(6)/sqrt(size(L_in)+size(L_out));
W = rand(L_out, 1 + L_in) * 2 * epsilon - epsilon;

% =========================================================================

end
