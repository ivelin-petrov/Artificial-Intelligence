# setwd("E:/FMI/Курс 4, Семестър 7/Изкуствен интелект/Programs")

library(e1071)
# library(caTools)
# library(mlbench)
# library(r2r)

# 435 rows
data(HouseVotes84, package = "mlbench")

# mydata = read.csv2("house-votes-84.txt", header = FALSE, sep = ",")
# data.frame(mydata)

nbc_classifier = function(){
  df = HouseVotes84[(1:430),] # [(431:435),] will be added later
  new_df = df[sample(nrow(df)),]
  
  accuracy = 0
  
  for(x in 0:9){
    training = new_df[-((43*x+1):(43*x+43)),]
    if(x != 9){
      training = rbind(training, HouseVotes84[(431:435),])
    }
    testing = new_df[((43*x+1):(43*x+43)),]
    if(x == 9){
      testing = rbind(testing, HouseVotes84[(431:435),])
    }
    
    # naiveBayes -> na.action -> by default does not count the values
    classifier = naiveBayes(Class ~ ., data = training, laplace = 1, log = TRUE, na.action = na.omit)
    predictions = predict(classifier, newdata = testing)
    
    m = mean(predictions == testing$Class)
    print(m)
    
    accuracy = accuracy + m
  }
  
  print("arithmetic mean")
  return (accuracy / 10)
}

nbc_classifier()

# mytable = table(testing$Class, predictions)
# myproptable = prop.table(mytable)
# success = myproptable[1,1] + myproptable[2,2]

# arithmetic mean -> some results: 0.9012685 | 0.9033827 | 0.9057082 | 0.9078224 | 0.9104651

#
# laplace    -> fix problem with zero probabilities
# log = TRUE -> probabilities p are given as log(p).
# na.action  -> na.omit returns the object with incomplete cases removed
#