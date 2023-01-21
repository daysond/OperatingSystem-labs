//
//  lab1.cpp
//
//  Created by Yyuan  Dong on 2023-01-20.
//  Student ID: 065-182-131

#include <iostream>
#include "pidUtil.h"
#include <vector>

using namespace std;

int main(int argc, const char *argv[])
{

    vector<int> pids;
//    ErrStatus status = GetAllPids(pids);
//    if (status == Err_OK){
//        for(auto pid: pids)
//            cout << pid << endl;
//    } else {
//        cout << status << endl;
//    }
//      string name = "hi";  
    string name = GetErrorMsg(Err_NoPid);
  

    ErrStatus s = GetNameByPid(1, name);
//
    cout<< "name: " << name << endl;
    
    return 0;
}
