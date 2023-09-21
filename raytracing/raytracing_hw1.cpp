#include<iostream>
#include <string>
#include <fstream>
#include <fstream>
#include <sstream>
#include "algebra3.h"

using namespace std;

int eyes[3];
ifstream in;

void ReadData(){


    string s;

    in.open("hw1_input.txt"); 

    if (in.fail()){  
        cout << "Error opening file" ;
        exit(1);
    }
    else{
        //printf("File opened successfully\n");
    }


}

void Circle_raytraing(int E[],int O[],int R[],int S[]){


}




int main(){

    ReadData();


    in.close();
    return 0;
}
