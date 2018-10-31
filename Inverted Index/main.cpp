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

using namespace std;

int main(int argc, const char * argv[]) {
    
    //test file list
    array<string, 2> fileList = {"test.txt", "test2.txt"};
    
    //create test file
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
    vector<tuple<string, size_t, size_t>> postingsList;
    
    // data structure for dictionary
    tuple <string, size_t, const vector<tuple<string, size_t, size_t> > > dictionaryTerm; //term, freq, ref to postings list
    vector<tuple <string, size_t, const vector<tuple<string, size_t, size_t> > > > dictionary;
    
    
    //open test files for input
    for ( size_t i{0}; i< fileList.size(); i++) {
        // open ith file in fileList.
        ifstream inFile{fileList[i], ios::in};
        //test if file was opened
        if (!inFile) {
            cerr << "File could not be opened" << endl;
            exit(EXIT_FAILURE);
        }
        
        cout<< "Reading from "<<fileList[i]<< " into the postings list." << endl;
        size_t wordCt{0}; //counter for word position in each text file
        while(inFile >> x) {
            //normalize term
            transform(x.begin(), x.end(), x.begin(), ::tolower); //this is a pain in the ass
            
            postingsList.push_back(make_tuple(x, i, wordCt++));
            // push post (tuple<word, doc#, position>) into postingsList
        }
    }
    
    //sort the postings List by term (first element of the tuple)
    sort( postingsList.begin(), postingsList.end());
    
    //test our postings list by printing them out
    cout<< "Printing from vector of strings"<<endl;
    cout<< "Term Doc# Position"<< endl;
    for ( size_t i{0}; i<postingsList.size(); i++) {
        cout << get<0>(postingsList[i]) << " " << get<1>(postingsList[i]) << " " <<get<2>(postingsList[i]) << endl;
    }
    cout << "Postings count: " << postingsList.size()<< endl;
    cout<<endl; //end test
    
    // normalize / clean words - I think this is a "nice to have" because it wasn't explicitly stated in the HW
    
    //sort the postings List by term (first element of the tuple)
    sort( postingsList.begin(), postingsList.end());
    
    return 0;
}

