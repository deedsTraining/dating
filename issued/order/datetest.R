datetestfun<-function(){
 count<-1
 aaawmaster<-read.table("aaawmaster")
 datetest<-numeric(964)
 for(i in 1:9911){
     if( (i%%9==0) && (i%%8!=0)){
          if(aaawmaster$V3[i]!=0){
                datetest[count]<-aaawmaster$V3[i]
			#print(aaawmaster$V3[i])
          }
	  else{
               datetest[count]<-aaawmaster$V4[i]
          }
	  #print(datetest[count])
          count<-count+1
    }
 }
return(datetest)
}