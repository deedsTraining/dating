orderseq<-function(){
sequences<-as.matrix(read.table("sequences"))
a<-seq(1,7,1)
for( i in 1:100){
	b<-sequences[i,]-a
	j<-1
	count<-0
	while(j<=7){
		if(b[j]==0){
			count=count+1
		}
		j<-j+1
	}
	print(c(i, count))
}
    
}