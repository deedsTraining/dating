#computes the number of matches, i.e similarity (1-hemming distance), between
#the sequence 1,2,3,4,5,6,7 and the top documents in "docprobfile" file that 
#matched the document sequences 1,2,3,4,5,6,7. 

mathcingnum<-function(){
	sequences<-as.matrix(read.table("sequences"))
	docprobfile<-scan("docprobfile")
	permutationoutpt<-as.matrix(read.table("permutationoutpt"))
	o<-order(-docprobfile)
	sequence<-c(1,2,3,4,5,6,7)
	matchingseq<-numeric(5040)
	
	for(i in 1:100){
		match<-0
		for(j in 1:7){
			if(sequences[o[i],j]==sequence[j]){
				match<-match+1	
			}
		}
		matchingseq[i]<-match  
		print(matchingseq[i])
	}
	
	#return(o[seq(1,10,1)])
	#return(permutationoutpt[o[seq(1,10,1)],])
	#return(sum(matchingseq[seq(1,232,1)]==1))
	#plot(seq(1,232,1), matchingseq[seq(1,232,1)],pch="*")
	#plot(seq(1,232,1), docprobfile[o[seq(1,232,1)]],pch="*" )
}

mathcingnum2<-function(){
	#out of the sample of 100 documents the top 3 that got selected
	# are the seuences (1,2,5,7,3,6,4), (1,4,3,2,7,6,5), (1,2,6,7,5,3,4)

	sequenceselect<-c(1,2,6,7,5,3,4)
	arrayseq<-matrix(0,92,7)
	#408 is the number j you find below
	#The first of the eight column indicates the numberline 
	#of the file "permutationoutpt" where one finds the matching sequence
	#from columns 2 to 8 of matrix arrayseq
       

        sequences<-as.matrix(read.table("sequences"))
        o<-order(-docprobfile)
	permutationoutpt<-as.matrix(read.table("permutationoutpt"))
	j<-0 
	k<-45

	for(i in 1:5040){
		if(sum(sequences[k,]==permutationoutpt[i,])>=4){
			j<-j+1
			arrayseq[j,]<-(permutationoutpt[i,])
		}
	}
	#return(j)
	return(arrayseq)
}