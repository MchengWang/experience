#include<iostream>
using namespace std;
int main()
{ 
 int m,n,i,j,time=1;
 while(cin>>m>>n)
 { if(m>n)
   {int temp;
    temp=m;
    m=n;
    n=temp;
    }
   j=0;
   for(i=m;i<=n;i++)
   { 
    int a,b,c,d;
    a=i/100;
    b=i%100;
    c=b/10;
    d=b%10;
    if(i==a*a*a+c*c*c+d*d*d)
      {
       if(time==1)   
       {cout<<i;
       time++;
        j++;}       
       else
       {cout<<" "<<i;
       j++;}
      }
    if(i==n&&j>0)  
    {cout<<endl;}
   }
  if(j==0)
  {cout<<"no"<<endl;}
  time=1;
 }
return 0;
}