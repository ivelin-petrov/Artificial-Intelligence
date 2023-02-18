# setwd("E:/FMI/Êóðñ 4, Ñåìåñòúð 7/Èçêóñòâåí èíòåëåêò/Programs")

#install.packages("factoextra")
library(factoextra)

f = function(filename, separate, k, ns){
  df = read.csv2(filename, header = FALSE, sep = separate, dec = ".")
  
  # nstart -> random starts 
  km_result = kmeans(df, centers = k, nstart = ns)
  
  # Vector of within-cluster sum of squares, one component per cluster
  #km_result$withinss
  
  # Total within-cluster sum of squares, i.e. sum(withinss)
  #km_result$tot.withinss
  
  # The between-cluster sum of squares
  #km_result$betweenss
  
  # A matrix of cluster centers
  #km_result$centers
  
  #print(km_result$size)
  
  fviz_cluster(km_result, data = df)
}

f("kmeans-normal.txt", "\t", 4, 10)
f("kmeans-unbalance.txt", " ", 8, 10)

unbalance_reduced = function(k, ns){
  df_unbalance = read.csv2("kmeans-unbalance.txt", header = FALSE, sep = " ", dec = ".")
  # clusters -> (first, second, third, other 5) -> (1:2000, 2001:4000, 4001:6000, 6001:6500)
  # first, second, third -> 2000 points each
  # other 5 -> 100 points each
  df_reduced = rbind(df_unbalance[sample(1:2000, 100, replace = FALSE),], 
                     df_unbalance[sample(2001:4000, 100, replace = FALSE),], 
                     df_unbalance[sample(4001:6000, 100, replace = FALSE),], 
                     df_unbalance[(6001:6500),])
  
  km_result = kmeans(df_reduced, centers = k, nstart = ns)
  
  fviz_cluster(km_result, data = df_reduced)
}

unbalance_reduced(8, 10)

df_normal = read.csv2("kmeans-normal.txt", header = FALSE, sep = "\t", dec = ".")
#df_unbalance = read.csv2("kmeans-unbalance.txt", header = FALSE, sep = " ", dec = ".")

# elbow_method
fviz_nbclust(df_normal, kmeans, method = "wss")
#fviz_nbclust(df_unbalance, kmeans, method = "wss") # k=8
fviz_nbclust(df_reduced, kmeans, method = "wss") # k=8


