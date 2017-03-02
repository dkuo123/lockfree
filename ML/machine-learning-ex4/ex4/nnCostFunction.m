function [J grad] = nnCostFunction(nn_params, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, ...
                                   X, y, lambda)
%NNCOSTFUNCTION Implements the neural network cost function for a two layer
%neural network which performs classification
%   [J grad] = NNCOSTFUNCTON(nn_params, hidden_layer_size, num_labels, ...
%   X, y, lambda) computes the cost and gradient of the neural network. The
%   parameters for the neural network are "unrolled" into the vector
%   nn_params and need to be converted back into the weight matrices. 
% 
%   The returned parameter grad should be a "unrolled" vector of the
%   partial derivatives of the neural network.
%

% Reshape nn_params back into the parameters Theta1 and Theta2, the weight matrices
% for our 2 layer neural network
t1_end = hidden_layer_size * (input_layer_size + 1);
Theta1 = reshape(nn_params(1:t1_end), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params((1 + t1_end):end), ...
                 num_labels, (hidden_layer_size + 1));

% Setup some useful variables
m = size(X, 1);
         
% You need to return the following variables correctly 
J = 0;
Theta1_grad = zeros(size(Theta1));  % 25x401
Theta2_grad = zeros(size(Theta2));  % 10 x 26

% ====================== YOUR CODE HERE ======================
% Instructions: You should complete the code by working through the
%               following parts.
%
% Part 1: Feedforward the neural network and return the cost in the
%         variable J. After implementing Part 1, you can verify that your
%         cost function computation is correct by verifying the cost
%         computed in ex4.m
%
A1 = [ones(m,1) X]; % add bias unit
Z2 = A1*Theta1'; % 5000x25
A2 = sigmoid(Z2);
A2 = [ones(m,1) A2]; % 5000x26
Z3 = A2*Theta2';
A3 = sigmoid(Z3);  %5000x10

Y = zeros(m,num_labels); %convert y into Y, 5000x10
for (i =1:m)
  Y(i,y(i)) = 1;
endfor

for (i =1:m)
   tmp_y = Y(i,:);
   J = J + log(A3(i,:))* tmp_y' + log(1-A3(i,:))*(1-tmp_y');
endfor
J = J/(-m);

% Part 2: Implement the backpropagation algorithm to compute the gradients
%         Theta1_grad and Theta2_grad. You should return the partial derivatives of
%         the cost function with respect to Theta1 and Theta2 in Theta1_grad and
%         Theta2_grad, respectively. After implementing Part 2, you can check
%         that your implementation is correct by running checkNNGradients
%
%         Note: The vector y passed into the function is a vector of labels
%               containing values from 1..K. You need to map this vector into a 
%               binary vector of 1's and 0's to be used with the neural network
%               cost function.
%
%         Hint: We recommend implementing backpropagation using a for-loop
%               over the training examples if you are implementing it for the 
%               first time.
%
% I didn't use for loop, it's mind twist, but matrix calc is more efficient
% D here is delta matrix
D3 = A3-Y;  % 5000x10
% hold on, Z2 is 5000x25, but D3*Theta2' is 5000x26
%D2 = (D3*Theta2') .*  sigmoidGradient(Z2); % it's .*, not *, 5000x26

D2 = (D3*Theta2) .* (A2 .* (1-A2));
%D2 = D2(:, 2:end);  % keep row, drop bias column of theta result, 5000x25
% there is no D1
Theta2_grad = D3' * A2;  % 10x26
% assign D2 first column to zero is the same as drop it from accumulation
% D2(:,1) = 0;
T1 = D2' * X;  % 26x401 
Theta1_grad = T1(2:end,:);  % 25x401

Theta2_grad = Theta2_grad/m;
Theta1_grad = Theta1_grad/m;
% Part 3: Implement regularization with the cost function and gradients.
%
%         Hint: You can implement this around the code for
%               backpropagation. That is, you can compute the gradients for
%               the regularization separately and then add them to Theta1_grad
%               and Theta2_grad from Part 2.
%
% regularized cost doesn't include the bias columns of betas, so zero them out
Theta1(:,1) = 0; 
t1 = reshape(Theta1.',1,numel(Theta1));% convert matrix to vector 1xN
% another way is t1 = [Theta1(:)]; which will create matrix of Nx1
Theta2(:,1) = 0;
t2 = reshape(Theta2.',1,numel(Theta2));  
J = J + lambda*(t1*t1' + t2*t2')/(2*m);

% -------------------------------------------------------------

% =========================================================================

% Unroll gradients
grad = [Theta1_grad(:) ; Theta2_grad(:)];


end
