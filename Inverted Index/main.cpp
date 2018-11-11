//
//  main.cpp
//  Inverted Index
//
//  Created by Ethan on 10/26/18.
//  Copyright © 2018 Ethan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
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
void createIndex(string); //takes the fileList name as arg
unsigned getMenuInput();
void showIndex();
void cleanString(string&);
bool checkSkip(string);
void searchIndex(string);

//global variables
unsigned menuSelection{0};
vector<string> fileListVector{};

//data structure
//   term                doc#           position
map< string, vector< map<size_t, vector<size_t>>>> dictionary;



int main() {
    
//    //Create fileList.txt - uncomment to create the fileList in the working directory
//    //file list
//    vector<string> fileList = {"t1.txt", "t2.txt", "t3.txt", "t4.txt", "t5.txt", "t6.txt", "t7.txt", "t8.txt", "t9.txt", "t10.txt", "t11.txt", "t12.txt"};
//    //create txt file
//    ofstream txtOut; //create out file object
//    txtOut.open("fileList.txt"); //open the file
//    for (auto t: fileList) {
//        txtOut << t << endl;
//    }
//    txtOut.close();
    
    createIndex("fileList.txt");

    //display entire index
    showIndex();
    
    //data structure reference for coding the mess below
    //map< string, vector< map<size_t, vector<size_t>>>> dictionary;
    //map<term, vector<map<doc#, vector<wordCt>>>> dictionary;

    //Display Menu
    while( menuSelection <2) { //global variable
        switch(getMenuInput()) { //display menu and receive menuSelection
            case 1: {
                //get search query
                string query; //dummy var
                cout << "Enter query:"<<endl;
                cout << ">> ";
                cin.clear();
                cin.ignore(256,'\n');
                getline(cin, query);
                //cleanString(query); //normalize search term
                searchIndex(query);
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

void createIndex(string fileList) {
    //variables
    string x; // temp variable for pulling tokens
    size_t wordCt{0}; //counter for word position in each text file


    ifstream fileListStream{fileList, ios::in};
    if (!fileListStream) {
        cerr << "File "<< fileList<< " could not be opened" << endl;
        exit(EXIT_FAILURE);
    }
    
    while( fileListStream >> x) {
        fileListVector.push_back(x);
    }
    
    //Create the index
    //open files for input
    for ( size_t i{0}; i< fileListVector.size(); i++) {
        
        // open ith file in fileList and check if the file was opened
        ifstream inFile{fileListVector[i], ios::in};
//        if (!inFile) {
//            cerr << "File "<< fileList[i]<< " could not be opened" << endl;
//            exit(EXIT_FAILURE);
//        }
        
        cout<< "Reading from "<<fileListVector[i]<< " into the index." << endl;
        

        
        while(inFile >> x) {
            cleanString(x); //normalize term
            if ( checkSkip(x)) {
                continue;
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
                
                cout<< fileListVector[ it2->first] << " "; //print doc #
                
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
        if( !isalpha(term[j]) ) { //if not alphanumeric, then erase the char
            term.erase(j, 1);
        }
    }
}

bool checkSkip(string s) {
    vector<string> skipWords{"and", "or", "the", "a"}; // words for the index to skip
    for( auto skip : skipWords) {
        if ( s == skip) {
            return true;
        }
    }
    return false;
}

void searchIndex(string q) {
    cout << endl << "Searching for " << q <<endl;
    //turn seach query into a vector of search terms
    istringstream iss(q);
    string part;
    vector<string> searchTerms;
    while ( getline(iss, part, ' ')){
        searchTerms.push_back(part);
    }

    // check search term against dictionary. If exists, return results. Else, return error msg
    for ( map< string, vector< map<size_t, vector<size_t>>>>::iterator it = dictionary.begin(); it != dictionary.end(); it++ ) {
        if ( it->first == q) {
            cout << it->first <<endl;
            //iterate through vector of maps ( doc# & pos)
            for ( size_t j{0}; j< it->second.size(); j++) {
                
                //iterate through second map (doc# & pos)
                for ( map<size_t, vector<size_t>>::iterator it2 = it->second[j].begin(); it2 != it->second[j].end(); it2++){
                    
                    cout<< fileListVector[ it2->first] << " " << it2->second.size(); //print doc #
                    
                    //                                //print all term locations
                    //                                for ( size_t k{0}; k< it2->second.size(); k++) {
                    //                                    cout << it2->second[k] << " ";
                    //                                }
                    cout << endl;
                    
                }
            }
            break;
        }
        else if ( next(it) == dictionary.end()) {
            cout << "There is no \""<< q << "\" in our system." <<endl;
        }
    }
    
    
    
    
}
