datetestvalfun<-function(){
 count<-1
 aaawmaster<-read.table("aaawmaster")
 datetestval<-numeric(2202)
 for(i in 1:9911){
     if( (i%%9==0) || (i%%8==0)){
          if(aaawmaster$V3[i]!=0){
                datetestval[count]<-aaawmaster$V3[i]
			#print(aaawmaster$V3[i])
          }
	  else{
               datetestval[count]<-aaawmaster$V4[i]
          }
	  #print(datetestval[count])
          count<-count+1
    }
 }
return(datetestval)
}