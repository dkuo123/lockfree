#include <stdio.h>
int main(){
    unsigned long int a[5];
    int i;
    scanf("%lld",&a[0]);
    unsigned long int  max=a[0],min=a[0],sum=a[0];
    for(i=1;i<5;i++){
        scanf("%lld",&a[i]);
        if(max<a[i])
            max=a[i];
        if(min>a[i])
            min=a[i];
        sum=sum+a[i];
    }
    printf("%lld %lld\n",sum-max,sum-min);
    return 0;
}
