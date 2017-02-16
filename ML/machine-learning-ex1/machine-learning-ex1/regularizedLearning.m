% to avoid overfitting on high order of polynomal function 

% for linear regression
function theta = regularizedNormalEqn(X, y, lambda)
  [m, n] = size(X);
  L = eye(n);
  L(1,1) = 0;
  theta = pinv(X'*X + lambda*L) * X' * y;
end

function cost = regularizedCost(theta, X, y, lambda)
[m,n] = size(X);
cost = 0;
h = X*theta - y;
theta(1,1) = 0;
cost = h' * h + lambda*theta'*theta;
cost = cost / (2*m);
endfunction


function grad = regularizedGradient(theta, X, y, lambda)
  [m,n] = size(X);
  h = X * theta ; % linear cost
  h = X' * (h-y);  
  theta(1,1) = 0;
  grad = (h + lambda*theta)/m;
  % grad is same dimension as theta, 0...n
endfunction


function [theta, J_history] = gradientDescent(X, y, theta, lambda, alpha, num_iters)
%GRADIENTDESCENT Performs gradient descent to learn theta
%   theta = GRADIENTDESCENT(X, y, theta, alpha, num_iters) updates theta by 
%   taking num_iters gradient steps with learning rate alpha

  % Initialize some useful values
  m = length(y); % number of training examples
  J_history = zeros(num_iters, 1);
  
  for iter = 1:num_iters
  
      % ====================== YOUR CODE HERE ======================
      % Instructions: Perform a single gradient step on the parameter vector
      %               theta. 
      %
      % Hint: While debugging, it can be useful to print out the values
      %       of the cost function (computeCost) and gradient here.
      %
  
      #disp(theta)
  
      % ============================================================
  
      % Save the cost J in every iteration    
      J_history(iter) = regularizedCost(theta, X, y, lambda);
      
      theta = theta - alpha*regularizedGradient(theta, X, y, lambda);
  
  end

endfunction

% it would be interesting to see if the above gradientDescent() generates different theta from regularizedNormalEqn()
%  =================================

% for logistic regression
% use fminunc, it needs costFunction as parameter

function cost = logisticRegularizedCost(theta, X, y, lambda)
  [m,n] = size(X);
  cost = 0;
  h = X*theta;
  h = y'*log(h) + (1-y')*log(1-h);  % logistic cost
  % h now is non-regularized logistic cost  
  % the following added regularized cost except on theta zero ( theta(1,1))
  % it means to explicitly exclude the bias term of theta zero
  theta(1,1) = 0;
  cost = -h + lambda*theta'*theta/2;
  cost = cost / m;
endfunction

function grad = logisticRegularizedGradient(theta, X, y, lambda)
  [m,n] = size(X);

  h = 1 ./ (1+ e .^ -(X * theta)); % logistic cost
  h = X' * (h - y);  
  theta(1,1) = 0;
  grad = (h + lambda*theta)/m;
  % grad is same dimension as theta, 0...n
endfunction

function [theta, J_history] = logisticRegularizedGradientDescent(X, y, theta, lambda, alpha, num_iters)
%GRADIENTDESCENT Performs gradient descent to learn theta
%   theta = GRADIENTDESCENT(X, y, theta, alpha, num_iters) updates theta by 
%   taking num_iters gradient steps with learning rate alpha

  % Initialize some useful values
  m = length(y); % number of training examples
  J_history = zeros(num_iters, 1);
  
  for iter = 1:num_iters
  
      % ====================== YOUR CODE HERE ======================
      % Instructions: Perform a single gradient step on the parameter vector
      %               theta. 
      %
      % Hint: While debugging, it can be useful to print out the values
      %       of the cost function (computeCost) and gradient here.
      %
  
      #disp(theta)
  
      % ============================================================
  
      % Save the cost J in every iteration    
      J_history(iter) = logisticCost(theta, X, y, lambda);
      
      theta = theta - alpha*logisticRegularizedGradient(theta, X, y, lambda);
  
  end

endfunction

% its costFunction signature is
function [jVal, gradient] = costFunction(theta,X, y, lambda)
  jVal = logisticCost(theta, X, y, lambda);
  gradient = logisticRegularizedGradient(theta,X, y, lambda);
endfunction

  