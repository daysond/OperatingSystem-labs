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
    ErrStatus err = GetAllPids(pids);
    string result = "";

    cout << "1. Getting All PIDs and names" << endl;
    if (err == Err_OK)
    {
        for (auto pid : pids)
        {
            err = GetNameByPid(pid, result);
            cout << ((err == Err_OK) ? pid + ": " + result : GetErrorMsg(err)) << endl;
        }
    }
    else
    {
        cout << err << endl;
    }

    cout << "\n2. Getting name of PID 1" << endl;
    err = GetNameByPid(1, result);
    cout << ((err == Err_OK) ? "PID 1: " + result : GetErrorMsg(err)) << endl;

    cout << "\n3. Getting PID of Lab1" << endl;
    int pid;
    err = GetPidByName("Lab1", pid);
    cout << ((err == Err_OK) ? "Lab 1 PID: " + pid : GetErrorMsg(err)) << endl;

    cout << "\n4. Getting PID of Lab11" << endl;
    err = GetPidByName("Lab11", pid);
    cout << ((err == Err_OK) ? "Lab 11 PID: " + pid : GetErrorMsg(err)) << endl;

    return 0;
}
