#generates the permutation of a, i.e all n! sequences.
fn_perm_list<-
function(n,r, v=1:n){
   if(r==1)
      matrix(v,n,1)
   else if (n==1)
      matrix(v,1,r)
   else {
     X<- NULL
     for(i in 1:n)
        X<- rbind(X,cbind(v[i], fn_perm_list(n-1, r-1, v[-i])))
     X
  }
}
