# setwd("E:/FMI/Êóðñ 4, Ñåìåñòúð 7/Èçêóñòâåí èíòåëåêò/Programs")

library(class)

knn_classifier = function(setK){
  df = iris
  new_df = df[sample(nrow(df)),]
  
  accuracy = 0
  
  for(x in 0:9){
    training = new_df[-((15*x+1):(15*x+15)), 1:4]
    testing = new_df[((15*x+1):(15*x+15)), 1:4]
    
    training_species = new_df[-((15*x+1):(15*x+15)),5]
    testing_species = new_df[((15*x+1):(15*x+15)),5]
    
    classifier = knn(train = training, test = testing, cl = training_species, k = setK)
    
    #table(testing_species, classifier)
    
    m = mean(classifier == testing_species)
    print(m)
    
    accuracy = accuracy + m
  }
  
  print("arithmetic mean")
  return (accuracy / 10)
}

knn_classifier(1)

knn_classifier(3)

knn_classifier(9)




