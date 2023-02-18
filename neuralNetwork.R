# setwd("E:/FMI/Курс 4, Семестър 7/Изкуствен интелект/Programs")

# install.packages("neuralnet")
library(neuralnet)

and = c(rep(0,3), 1)
or = c(0, rep(1,3))
not = c(1,0,1,0)
xor = c(0,1,1,0)

df =  data.frame(expand.grid(c(0,1), c(0,1)), and, or, not, xor)

# inputs           -> {{0,0}, {1,0}, {0,1}, {1,1}}
# expected outputs -> {{0}, {0}, {0}, {1}}

and_nn = neuralnet(and ~ Var1+Var2, data = df, hidden=0, rep=10, algorithm = "backprop", 
                   learningrate = 10, err.fct="sse", linear.output=FALSE)
#print(and_nn)
plot(and_nn, rep="best")

# inputs           -> {{0,0}, {1,0}, {0,1}, {1,1}}
# expected outputs -> {{0}, {1}, {1}, {1}}

or_nn = neuralnet(or ~ Var1+Var2, data = df, hidden=0, rep=10, algorithm = "backprop", 
                  learningrate = 20, err.fct="sse", linear.output=FALSE)
#print(or_nn)
plot(or_nn, rep="best")

# inputs           -> {{0}, {1}, {0}, {1}}
# expected outputs -> {{1}, {0}, {1}, {0}}

not_nn = neuralnet(not ~ Var1, data = df, hidden=0, rep=10, err.fct="sse")
#print(not_nn)
plot(not_nn, rep="best")

# inputs           -> {{0,0}, {1,0}, {0,1}, {1,1}}
# expected outputs -> {{0}, {1}, {1}, {0}}

xor_nn = neuralnet(xor ~ Var1+Var2, data = df, hidden=4, rep=5, algorithm = "backprop", 
                   learningrate = 0.1, err.fct="sse")
#print(xor_nn)
plot(xor_nn, rep="best", col.hidden = 'darkgreen', col.hidden.synapse = 'darkgreen', fill = 'lightblue')

# err.fct -> "sse" - sum of squared errors
#         -> "ce"  - cross-entropy
#
# linear.output -> TRUE or FALSE - if act.fct (function used for result smoothing) should not be applied
#                                  to the output neurons -> TRUE, otherwise -> FALSE
# algorithm used -> backpropagation


