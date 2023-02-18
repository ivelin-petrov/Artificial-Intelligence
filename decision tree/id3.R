# setwd("E:/FMI/Êóðñ 4, Ñåìåñòúð 7/Èçêóñòâåí èíòåëåêò/Programs")

# https://www.r-bloggers.com/2015/04/id3-classification-using-data-tree/

# https://www.geeksforgeeks.org/decision-tree-introduction-example/

library(magrittr)
library(dplyr)
library(data.tree)

#install.packages("rpart")
library(rpart)

#install.packages("rattle")
library(rattle)
#install.packages("rpart.plot")
library(rpart.plot)

# functions

is_pure <- function(data) {
  length(unique(data[,ncol(data)])) == 1
}

entropy = function(target) {
  freq = table(target)/length(target)
  
  vec = as.data.frame(freq)[,2]
  # remove 0 values
  vec = vec[vec > 0]
  
  -sum(vec * log2(vec))
}

ig_cat = function(data, feature, target){
  # remove NA values
  data = data[!is.na(data[,feature]),] 
  
  #using dplyr to compute e and p for each value of the feature 
  dd_data <- data %>% group_by_at(feature) %>% summarise(e=entropy(get(target)), 
                                                         n=length(get(target))
  )
  #entropy for the parent
  e0 = entropy(data[,target])
  
  #calculate p for each value of feature
  dd_data$p = dd_data$n/nrow(data)
  
  ig = e0 - sum(dd_data$p*dd_data$e)
  
  return(ig)
}

check_ig = function(data){
  col_name = character()
  ig = numeric()
  features = names(data)
  
  for (i in 1:ncol(data)){
    col_name[i] = names(data)[i]
    ig[i] = ig_cat(data, names(data)[i], "Class")
  }
  
  ig_df = cbind(col_name, round(ig,2))
  ig_df
}

train_id3 = function(node, data, ordered_df, index){
  node$obsCount = nrow(data)

  if(is_pure(data)){
    child <- node$AddChild(unique(data[,ncol(data)]))
    node$feature <- tail(names(data), 1)
    child$obsCount <- nrow(data)
    child$feature <- ''
  }else{
    feature = ordered_df[index,1]
    node$feature <- feature
    
    childObs <- split(data[,!(names(data) %in% feature)], data[,feature], drop = TRUE)
    
    for(i in 1:length(childObs)) {
      child <- node$AddChild(names(childObs)[i])
      
      train_id3(child, childObs[[i]], ordered_df, index + 1)
    }
  }
}

predict = function(tree, features) {
  if (tree$children[[1]]$isLeaf) return (tree$children[[1]]$name)
  
  child <- tree$children[[features[[tree$feature]]]]
  
  return (predict(child, features))
}

# testing functions

f1 = function(){
  library(mlbench)
  data("BreastCancer")
  
  #ig_cat(BreastCancer, "Cell.size", "Class")
  
  my_df = na.omit(BreastCancer)
  
  # randomize
  my_df = my_df[sample(nrow(my_df)),]
  
  df = check_ig(my_df)
  o_df = df[order(df[,2],decreasing = TRUE),]
  
  # drop ID and Class
  ordered_df = o_df[-(1:2),]
  
  # nrow(my_df) = 683
  
  extra = my_df[(681:683),]
  my_df = my_df[-(681:683),]
  
  accuracy = 0
  
  for(x in 0:9){
    training = my_df[-((68*x+1):(68*x+68)),]
    if(x != 9){
      training = rbind(training, extra)
    }
    testing = my_df[((68*x+1):(68*x+68)),]
    if(x == 9){
      testing = rbind(testing, extra)
    }
    
    # building tree
    tree = Node$new("Class")
    train_id3(tree, training, ordered_df, 1)
    #print(tree, "feature", "obsCount")
    
    #predict(tree, c("Cell.size" = 2, "Cell.shape" = 1, "Epith.c.size" = 1, "Mitoses" = 1))
    
    count = 0
    size = nrow(testing)
    
    for(y in 1:size){
      skip = FALSE
      
      tryCatch(
        if(predict(tree, testing[y,-1]) == my_df[68*x+y,]$Class){
          count = count + 1        
        },
        error = function(e) {skip = TRUE}
      )
      
      if(skip){
        next
      }
    }
    
    m = count/size
    print(m)
    
    accuracy = accuracy + m
  }
    
  print("arithmetic mean")
  return (accuracy / 10)
}

f2 = function(){
  # my_data size = 286
  my_data = read.csv2("breast-cancer.txt", header = FALSE, sep = ",", dec = ".")
  colnames(my_data) = c("Class",
                       "age","menopause","tumor-size",
                       "inv-nodes","node-caps","deg-malig",
                       "breast","breast-quad","irradiat")
  
  # set class column to be last
  my_data = my_data[c("age","menopause","tumor-size",
                    "inv-nodes","node-caps","deg-malig",
                    "breast","breast-quad","irradiat",
                    "Class")]
  
  indices = c()
  
  for(i in 1:nrow(my_data)){
    for(j in 1:ncol(my_data)){
      if(my_data[i,j] == "?"){
        indices = append(indices, i)
      }
    }
  }
  
  my_data = my_data[-indices,]
  
  df = check_ig(my_data)
  o_df = df[order(df[,2],decreasing = TRUE),]
  ordered_df = o_df[-1,]

  tree = Node$new("Class")
  tryCatch(
    train_id3(tree, my_data, ordered_df, 1),
    error = function(e) {}
  )
  #print(tree, "feature", "obsCount")
  
  count = 0
  size = nrow(my_data)
  
  for(x in 1:size){
    skip = FALSE
    
    tryCatch(
      if(predict(tree, my_data[x,]) == my_data[x,]$Class){
        count = count + 1        
      },
      error = function(e) {skip = TRUE}
    )
    
    if(skip){
      next
    }
  }
  
  return (count/size)
}

f3 = function(){
  library(mlbench)
  data("BreastCancer")
  
  my_df = na.omit(BreastCancer[,-1])
  
  # randomize
  my_df = my_df[sample(nrow(my_df)),]
  
  # nrow(my_df) = 683
  
  extra = my_df[(681:683),]
  my_df = my_df[-(681:683),]
  
  accuracy = 0
  
  for(x in 0:9){
    training = my_df[-((68*x+1):(68*x+68)),]
    if(x != 9){
      training = rbind(training, extra)
    }
    testing = my_df[((68*x+1):(68*x+68)),]
    if(x == 9){
      testing = rbind(testing, extra)
    }
  
    decision_tree_model = rpart(Class ~ ., data = training, method = "class", control = rpart.control(cp = 0, maxdepth = 8, minsplit = 100))
    
    #prepruning
    #decision_tree_model = rpart(Class ~ ., data = training, method = "class", control = rpart.control(cp = 0, maxdepth = 8))
    
    #fancyRpartPlot(decision_tree_model, caption = NULL)
    
    #plotcp(decision_tree_model)
    #printcp(decision_tree_model)
    
    fancyRpartPlot(decision_tree_model, caption = NULL)
    
    #postpruning
    #dtm_postPruned = prune(decision_tree_model, cp = 0.0084)
    
    #plotcp(dtm_postPruned)
    #printcp(dtm_postPruned)
    
    testing$pred = predict(decision_tree_model, testing)
    #testing$pred = predict(dtm_postPruned, testing)
    
    
    m = mean(testing$pred == testing$Class)
    print(m)
    
    accuracy = accuracy + m
  }
  
  print("arithmetic mean")
  return (accuracy / 10)
}

f4 = function(){
  my_data = read.csv2("breast-cancer.txt", header = FALSE, sep = ",", dec = ".")
  colnames(my_data) = c("Class",
                       "age","menopause","tumor-size",
                       "inv-nodes","node-caps","deg-malig",
                       "breast","breast-quad","irradiat")
  
  indices = c()
  
  for(i in 1:nrow(my_data)){
    for(j in 1:ncol(my_data)){
      if(my_data[i,j] == "?"){
        indices = append(indices, i)
      }
    }
  }
  
  my_data = my_data[-indices,]
  
  #randomize
  my_data = my_data[sample(nrow(my_data)),]
  
  # nrow(my_data) = 277
  extra = my_data[(271:277),]
  my_data = my_data[-(271:277),]
  
  accuracy = 0
  
  for(x in 0:9){
    training = my_data[-((27*x+1):(27*x+27)),]
    if(x != 9){
      training = rbind(training, extra)
    }
    testing = my_data[((27*x+1):(27*x+27)),]
    if(x == 9){
      testing = rbind(testing, extra)
    }
    
    decision_tree_model = rpart(Class ~ ., data = training, method = "class")
    
    #printcp(decision_tree_model)
    #plotcp(decision_tree_model)
    
    fancyRpartPlot(decision_tree_model, caption = NULL)
    
    #prepruning
    #decision_tree_model = rpart(Class ~ ., data = training, method = "class", control = rpart.control(cp = 0, maxdepth = 8, minsplit = 100))
    
    #postpruning
    #dtm_postPruned = prune(decision_tree_model, cp = 0.0084)
    
    testing$pred = predict(decision_tree_model, testing)
    
    m = mean(testing$pred == testing$Class)
    print(m)
    
    accuracy = accuracy + m
  }
  
  print("arithmetic mean")
  return (accuracy / 10)  
}

# cp -> complexity parameter in R to control the tree growth
# node splitting
# maxdepth 

# data(BreastCancer)
f1()

# data from breast-cancer.txt 
f2()

# data(BreastCancer)
f3()

# data from breast-cancer.txt
f4()


