#pragma once
#include "channels.h"
#include "protocol.h"

#include <iostream>
#include <fstream>

using namespace std;

void bin_search(vector<bool> &sender, vector<bool> &receiver, int begin_sen, int begin_rec, int size,vector<bool> &changed,bool &is_changed); //recursive function which can negate last one

