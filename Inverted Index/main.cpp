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
#include <utility>

using namespace std;

int main(int argc, const char * argv[]) {
    
    //test file list
    array<string, 3> fileList = {"test.txt", "test2.txt", "test3.txt"};
    
    //create a test file
    ofstream testOut; //create out file object
    testOut.open("test.txt"); //open the file
    testOut<< "This is a dog test...\n"; // add dummy data to the file
    testOut.close();
    testOut.open("test2.txt");
    testOut<< "I pity the fool! test\n";
    testOut.close();
    testOut.open("test3.txt");
    testOut<< "The quick brown fox jumped over the lazy brown dog.\n";
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
    
    //    // data structure for dictionary
    //    tuple <string, size_t, const vector<tuple<string, size_t, size_t> > > dictionaryTerm; //term, freq, ref to postings list
    //    vector<tuple <string, size_t, const vector<tuple<string, size_t, size_t> > > > dictionary;
    
    //data structure v2
    
    // term -                doc# - position
    map< string, vector< map<size_t, vector<size_t>>>> dictionary;
    
    
    //open test files for input
    for ( size_t i{0}; i< fileList.size(); i++) {
        // open ith file in fileList.
        ifstream inFile{fileList[i], ios::in};
        //test if file was opened
        if (!inFile) {
            cerr << "File could not be opened" << endl;
            exit(EXIT_FAILURE);
        }
        
        cout<< "Reading from "<<fileList[i]<< " into the index." << endl;
        size_t wordCt{0}; //counter for word position in each text file
        while(inFile >> x) {
            //normalize terms
            
            //tolower
            transform(x.begin(), x.end(), x.begin(), ::tolower); // lower case; this is a pain in the ass
            
            // iterate through characters to check alphanumeric, delete non-alphanum
            for ( size_t j{0}; j< x.size(); j++) { //check chars in string
                if( ! isalpha(x[j]) ) { //if not alphanumeric, then erase the char
                    x.erase(j);
                }
            }
            
            //data structure reference for coding the mess below
            //map< string, vector< map<size_t, vector<size_t>>>> dictionary;
            //map<term, vector<map<doc#, vector<wordCt>>>> dictionary;
            
            //if - word exists in dict
            if( dictionary.count(x) > 0) {
                //if doc# exists in dict, then add the word position into the vector mapped to the doc# key
                // x = word
                // i = doc#
                // j = vector iterator
                
                for ( size_t j{0}; j< dictionary.at(x).size(); j++) { // for a given word, iterate through the first level vector to find doc#
                    if ( dictionary.at(x)[j].count(i) ) {
                        //if doc# is found, add position to the word location vector
                        dictionary.at(x)[j].at(i).push_back(wordCt); //this is so fucked up
                        break;
                    }
                    //if word exists, but doc# doesn't exist in dict, add both the doc# key and word position vector
                    else if ( j == dictionary.at(x).size() ) {
                        vector<size_t> temp{wordCt};
                        map<size_t, vector<size_t>> temp2;
                        temp2.insert( make_pair(i, temp));
                        dictionary.at(x).push_back(temp2);
                    }
                }
                
            }
            
            //check if a key exists in the dictionary
            
            //else - word doesn't exist in dict, build a new word entry to insert into dictionary
            else {
                vector<size_t> temp{wordCt};
                map<size_t, vector<size_t>> temp2;
                temp2[i] = temp;
                vector<map <size_t, vector<size_t>>> temp3 = {temp2};
                dictionary.insert( make_pair( x, temp3));
            }
            // push post (tuple<word, doc#, position>) into tokensList
            wordCt++;
        }
        
        
    }
    
    //test our dictionary by printing them out
    cout<< "Printing from dictionary"<<endl;
    
    //map iterator
    for ( map< string, vector< map<size_t, vector<size_t>>>>::iterator it = dictionary.begin(); it != dictionary.end(); it++ ) { //iterate through first map
        cout<< it->first << " ";
        //iterate through first vector
        for ( size_t j{0}; j< it->second.size(); j++) {
            for ( map<size_t, vector<size_t>>::iterator it2 = it->second[j].begin(); it2 != it->second[j].end(); it2++){ //iterate through second map
                cout<< it2->first << " "; //print doc #
                
                for ( size_t k{0}; k< it2->second.size(); k++) {
                    cout << it2->second[k] << " ";
                }
                cout << endl;
                
            }
        }
        
        cout << endl;
    }
    
    
    //    cout<< "Term Doc# Position"<< endl;
    //    for ( size_t i{0}; i<tokensList.size(); i++) {
    //        cout << get<0>(tokensList[i]) << " " << get<1>(tokensList[i]) << " " <<get<2>(tokensList[i]) << endl;
    //    }
    //    cout << "Postings count: " << tokensList.size()<< endl;
    //    cout<<endl;
    //end test
    
    
    
    
    
    //    // make the dictionary -> turn tokens list into a dictionary by grouping terms
    //    //dictionary < term, doc freq>
    //    vector<tuple<string, size_t>> dict;
    //
    //    for ( size_t i{0}; i< tokensList.size(); i++) {
    //        //check ith element in the tokenslist vs the last element in the dictionary
    //        //if the word exists, increase the counter
    //        if ( dict.size() > 0 && get<0>(tokensList[i]) == get<0>(dict.back()) ) { //dict.size() > 0 is necessary to prevent bad memory access
    //            get<1>(dict.back())++;
    //        }
    //        else { //otherwise add it to the dictionary
    //            dict.push_back(make_tuple( get<0>(tokensList[i]), 1));
    //        }
    //    }
    //
    //    //test the dictionary
    //    for ( size_t i{0}; i < dict.size(); i++) {
    //        cout<< get<0>(dict[i]) << " " << get<1>(dict[i]) << endl;
    //    }
    //    // make postings lists
    //    //vector o
    
    
    
    return 0;
}

