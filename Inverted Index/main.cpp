//
//  main.cpp
//  Inverted Index
//
//  Created by Ethan on 10/26/18.
//  Copyright © 2018 Ethan. All rights reserved.
//

#include <iostream>
#include <fstream>
//#include <iomanip>
//#include <string>
//#include <cstdlib>
#include <vector>
#include <array>
#include <tuple>
#include <algorithm>
#include <cctype>
#include <map>

using namespace std;

int main(int argc, const char * argv[]) {
    
    //test file list
    array<string, 2> fileList = {"test.txt", "test2.txt"};
    
    //create a test file
    ofstream testOut; //create out file object
    testOut.open("test.txt"); //open the file
    testOut<< "This is a test...\n"; // add dummy data to the file
    testOut.close();
    
    // temp variables for pulling tokens
    string x;
    vector<string> tokens;
    
    // variables for each word in a file
    //string term;
    //size_t docID, wordPos;
    
    // data structure for postings list
    tuple<string, size_t, size_t> posting;
    vector<tuple<string, size_t, size_t>> tokensList;
    
    // data structure for dictionary
    tuple <string, size_t, const vector<tuple<string, size_t, size_t> > > dictionaryTerm; //term, freq, ref to postings list
    vector<tuple <string, size_t, const vector<tuple<string, size_t, size_t> > > > dictionary;
    
    //data structure v2
    
    // term - doc# - position
    map< string, vector< map<size_t, size_t>>> dict2;
    
    
    //open test files for input
    for ( size_t i{0}; i< fileList.size(); i++) {
        // open ith file in fileList.
        ifstream inFile{fileList[i], ios::in};
        //test if file was opened
        if (!inFile) {
            cerr << "File could not be opened" << endl;
            exit(EXIT_FAILURE);
        }
        
        cout<< "Reading from "<<fileList[i]<< " into the tokens list." << endl;
        size_t wordCt{0}; //counter for word position in each text file
        while(inFile >> x) {
            //normalize terms
            
            //tolower
            transform(x.begin(), x.end(), x.begin(), ::tolower); // lower case; this is a pain in the ass
            
            // iterate through characters to check alphanumeric, delete non-alphanum
            for ( size_t i{0}; i< x.size(); i++) { //check chars in string
                if( ! isalpha(x[i]) ) { //if not alphanumeric, then erase the char
                    x.erase(i);
                }
            }
            
            tokensList.push_back(make_tuple(x, i, wordCt++));
            // push post (tuple<word, doc#, position>) into tokensList
        }
    }
    
    //sort the postings List by term (first element of the tuple)
    sort( tokensList.begin(), tokensList.end());
    
    //test our postings list by printing them out
    cout<< "Printing from vector of strings"<<endl;
    cout<< "Term Doc# Position"<< endl;
    for ( size_t i{0}; i<tokensList.size(); i++) {
        cout << get<0>(tokensList[i]) << " " << get<1>(tokensList[i]) << " " <<get<2>(tokensList[i]) << endl;
    }
    cout << "Postings count: " << tokensList.size()<< endl;
    cout<<endl; //end test
    
    // make the dictionary -> turn tokens list into a dictionary by grouping terms
    //dictionary < term, doc freq>
    vector<tuple<string, size_t>> dict;
    
    for ( size_t i{0}; i< tokensList.size(); i++) {
        //check ith element in the tokenslist vs the last element in the dictionary
        //if the word exists, increase the counter
        if ( dict.size() > 0 && get<0>(tokensList[i]) == get<0>(dict.back()) ) { //dict.size() > 0 is necessary to prevent bad memory access
            get<1>(dict.back())++;
        }
        else { //otherwise add it to the dictionary
            dict.push_back(make_tuple( get<0>(tokensList[i]), 1));
        }
    }
    
    //test the dictionary
    for ( size_t i{0}; i < dict.size(); i++) {
        cout<< get<0>(dict[i]) << " " << get<1>(dict[i]) << endl;
    }
    
    
    // make postings lists
    //vector o
    
    
    
    return 0;
}

