#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<iostream>
#include<errno.h>
#include "Charioctl.h"
#include<sys/ioctl.h>
#define Device "/dev/chardev"
using namespace std;
int file=0;
Letter_Method method;
int device_wirte(){
int length;
int ret;
char *inputdata=new char[1024] ; 
cout<<"Please input the data: "<<endl;
cin.getline(inputdata,1000);
length=strlen(inputdata);
cout<<"Input file content: "<<inputdata<<endl;
cout<<"Input file length: "<<length<<endl;
ret=write(file,inputdata,length);
if(ret==-1)
cout<<"Write the data failed"<<endl;
else
cout<<"Write the data successfully"<<endl;
delete inputdata;
return 0;

}
int device_read(){
int readlength;
int ret;
char *readdata=new char[1024]; 
cout<<"Please input the number of read:"<<endl;
cin>>readlength;
memset(readdata,0,sizeof(readdata));
ret=read(file,readdata,readlength);
  if(ret==-1)
      cout<<"read the data filed"<<endl;
  else
      cout<<"read the data successfully"<<endl;
cout<<"The read file is :"<<readdata<<endl;
delete(readdata);
return 0;
}

int main(){
int value;
Letter_Method method;
if(access(Device,F_OK)==-1)
{
cout<<Device<<" not loaded;"<<endl;
return 0;
}
else
cout<<Device<<" is loaded"<<endl;

cout<<"Please Enter the option:"<<endl;
cout<<"1 to write,"<<endl;
cout<<"2 to UP the letters and  read,"<<endl;
cout<<"3 to Down the letters and read"<<endl;
cout<<"4 to Reserve the letters and  read,"<<endl;
cout<<"Now pleae Enter.."<<endl;
cin>>value;

char name[256];
cin.getline(name,256);

switch(value)
{

case 1:
      cout<<"The write option is selected!"<<endl;
      file=open(Device,O_RDWR); 
      if(file==-1)
      {
       cerr<<"errno: "<<strerror(errno)<<endl;
       cout<<"can't open the driver"<<endl;
       exit(0);
       }
      device_wirte(); 
      close(file); 
      break; 
case 2:
      method=Up;
      cout<<"The up letter and read option is selected!"<<endl;
      file=open(Device,O_RDWR); 
      ioctl(file,IOCTL_SET_MSG,&method);
      device_read(); 
      close(file);
      break;
case 3:
      method=Down;
      cout<<"The down letter and read option is selected!"<<endl;
      file=open(Device,O_RDWR);
      ioctl(file,IOCTL_SET_MSG,&method);
      device_read(); 
      close(file);
      break;
case 4:
      method=Reverse;
      cout<<"The resverse letter and read option is selected!"<<endl;
      file=open(Device,O_RDWR);
      ioctl(file,IOCTL_SET_MSG,&method);
      device_read(); 
      close(file);
      break;
default:
      cout<<"Unkonw the option,please input the valid opton"<<endl;
     break;  
}

return 0;
}
