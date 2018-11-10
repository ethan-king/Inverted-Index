//
//  main.cpp
//  Inverted Index
//
//  Created by Ethan on 10/26/18.
//  Copyright © 2018 Ethan. All rights reserved.
//

#include <iostream>
#include <fstream>
//#include <string>
//#include <cstdlib>
#include <vector>
#include <array>
#include <algorithm>
#include <cctype>
#include <map>
#include <utility>
#include <iomanip>

using namespace std;

//function prototypes
unsigned getMenuInput();
void showIndex();
void cleanString( string&);

//global variables
unsigned menuSelection{0};
//file list
vector<string> fileList = {"t1.txt", "t2.txt", "t3.txt", "t4.txt", "t5.txt", "t6.txt", "t7.txt", "t8.txt", "t9.txt", "t10.txt", "t11.txt", "t12.txt"};
//data structure v2
// term -                doc# - position
map< string, vector< map<size_t, vector<size_t>>>> dictionary;


int main() {
    // temp variables for pulling tokens
    string x;
    
    //Create the index
    //open files for input
    for ( size_t i{0}; i< fileList.size(); i++) {
        
        // open ith file in fileList and check if the file was opened
        ifstream inFile{fileList[i], ios::in};
        if (!inFile) {
            cerr << "File "<< fileList[i]<< " could not be opened" << endl;
            exit(EXIT_FAILURE);
        }
        
        cout<< "Reading from "<<fileList[i]<< " into the index." << endl;
        size_t wordCt{0}; //counter for word position in each text file
        while(inFile >> x) {
            cleanString(x); //normalize term
            
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
                        dictionary.at(x)[j].at(i).push_back(wordCt++);
                        
                        break;
                    }
                    //if word exists, but doc# doesn't exist in dict, add both the doc# key and word position vector
                    else if ( j == dictionary.at(x).size()-1 ){
                        vector<size_t> temp{wordCt};
                        map<size_t, vector<size_t>> temp2;
                        temp2.insert( make_pair(i, temp));
                        dictionary.at(x).push_back(temp2);
                        wordCt++;
                        break;
                    }
                }
                
            }
            //else - word doesn't exist in dict, build a new word entry to insert into dictionary
            else {
                vector<size_t> temp{wordCt};
                map<size_t, vector<size_t>> temp2;
                temp2[i] = temp;
                vector<map <size_t, vector<size_t>>> temp3 = {temp2};
                dictionary.insert( make_pair( x, temp3));
                wordCt++;
            }
        }
    }
    
    //display entire index
    showIndex();
    
    //data structure reference for coding the mess below
    //map< string, vector< map<size_t, vector<size_t>>>> dictionary;
    //map<term, vector<map<doc#, vector<wordCt>>>> dictionary;

    //Display Menu
    while( menuSelection <2) {
        switch(getMenuInput()) {
            case 1: {
                string query;
                cout << "Enter search term"<<endl;
                cout << ">> ";
                cin >> query;
                cleanString(query);
                cout << endl;
                
                // check search term against dictionary. If exists, return results. Else, return error msg
                for ( map< string, vector< map<size_t, vector<size_t>>>>::iterator it = dictionary.begin(); it != dictionary.end(); it++ ) {
                    if ( it->first == query) {
                        cout << it->first <<endl;
                        //iterate through vector of maps ( doc# & pos)
                        for ( size_t j{0}; j< it->second.size(); j++) {
                            
                            //iterate through second map (doc# & pos)
                            for ( map<size_t, vector<size_t>>::iterator it2 = it->second[j].begin(); it2 != it->second[j].end(); it2++){
                                
                                cout<< fileList[ it2->first] << " "; //print doc #
                                
                                for ( size_t k{0}; k< it2->second.size(); k++) {
                                    cout << it2->second[k] << " ";
                                }
                                cout << endl;
                                
                            }
                        }
                        break;
                    }
                    else if ( next(it) == dictionary.end()) {
                        cout << "There is no \""<< query<< "\" in our system." <<endl;
                    }
                }
                break;
            }
            case 2: {
                cout << "System exit" << endl;
                break;
            }
        }
        
        
    }
    
    
    
    
    
    
    return 0;
}

unsigned getMenuInput() {
    unsigned selection{0}, menuOptions{2};
    cout<< endl;
    cout<< "Inverted index query" << endl;
    cout<< "  (1) Search" << endl;
    cout<< "  (2) Quit" << endl;
    cout<< ">> ";
    
    cin >> selection;
    cout<< endl;
    
    while (!cin || selection > menuOptions) {
        cout << endl << "Invalid menu selection.\n>> ";
        cin.clear();
        cin.ignore(256,'\n');
        cin >> selection;
    }
    menuSelection= selection;
    return selection;
}

void showIndex() { //display entire index
    cout<< "Displaying index contents."<<endl;
    //map iterator
    for ( map< string, vector< map<size_t, vector<size_t>>>>::iterator it = dictionary.begin(); it != dictionary.end(); it++ ) {
        
        cout << it->first <<endl;
        
        //iterate through vector of maps ( doc# & pos)
        for ( size_t j{0}; j< it->second.size(); j++) {
            
            //iterate through second map (doc# & pos)
            for ( map<size_t, vector<size_t>>::iterator it2 = it->second[j].begin(); it2 != it->second[j].end(); it2++){
                
                cout<< fileList[ it2->first] << " "; //print doc #
                
                for ( size_t k{0}; k< it2->second.size(); k++) {
                    cout << it2->second[k] << " ";
                }
                cout << endl;
                
            }
        }
        
        cout << endl;
    }
}

void cleanString(string& term) { //tolower and remove non-alphanum
    //tolower
    transform(term.begin(), term.end(), term.begin(), ::tolower); // lower case; this is a pain in the ass
    //check alphanumeric, delete non-alphanum
    for ( size_t j{0}; j< term.size(); j++) { //check chars in string
        if( ! isalpha(term[j]) ) { //if not alphanumeric, then erase the char
            term.erase(j);
        }
    }
}
