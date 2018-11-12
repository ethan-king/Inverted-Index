//
//  main.cpp
//  Inverted Index
//
//  Copyright © 2018 Ethan King, Joseph Hall, Shishir Kumar. All rights reserved.
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
#include <tuple>

using namespace std;

//function prototypes
void createIndex(string); //takes the fileList name as arg
unsigned getMenuInput();
void showIndex();
void cleanString(string&);
bool checkSkip(string);
bool sortBySec(const pair<string, size_t>&, const pair<string, size_t>&);
void searchIndex(string);
vector<string> intersect(vector<vector<pair<string, size_t>>>);

//global variables
unsigned menuSelection{0};
vector<string> fileListVector{};
vector<string> skipWords{"and", "or", "the", "a"}; // exclude list

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
    //    for (auto t: fileList) { txtOut << t << endl;}
    //    txtOut.close();
    
    createIndex("fileList.txt");
    
    //display entire index - optional
    showIndex();
    
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

    
    ifstream fileListStream{fileList, ios::in};
    if (!fileListStream) {
        cerr << "File "<< fileList<< " could not be opened" << endl;
        exit(EXIT_FAILURE);
    }
    
    while( fileListStream >> x) {
        fileListVector.push_back(x);
    }
    
    //Create the index
    for ( size_t i{0}; i< fileListVector.size(); i++) {
            size_t wordCt{0}; //counter for word position in each text file
        // open ith file in fileList and check if the file was opened
        ifstream inFile{fileListVector[i], ios::in};
        cout<< "Reading from "<<fileListVector[i]<< " into the index." << endl;
        while(inFile >> x) {
            cleanString(x);
            cleanString(x);//normalize term
            if ( checkSkip(x)) {
                wordCt++;
                continue;
            }
            //data structure reference for coding the mess below
            //map< string, vector< map<size_t, vector<size_t>>>> dictionary;
            //map<term, vector<map<doc#, vector<wordCt>>>> dictionary;
            else if( dictionary.count(x) > 0) { //if - word exists in dict
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
            term.erase(term.begin() +j);
        }
    }
}

bool checkSkip(string s) {// checks if a word should be excluded from the index
    for( auto skip : skipWords) {
        if ( s == skip) {
            return true;
        }
    }
    return false;
}

bool sortBySec(const pair<string, size_t>& a, const pair<string, size_t>& b){ //third argument for sort()
    return ( a.second > b.second); // compare two pairs, sort in descending order by pair.second
}

void searchIndex(string q) {
    cout << endl << "Searching for " << q <<endl;
    //turn seach query into a vector of search terms
    istringstream iss(q);
    string part{};
    vector<string> searchTerms{}, searchTermsDirty{};
    while (getline(iss, part, ' ')){
        cleanString(part);
        searchTerms.push_back(part);
    }
    
    
    vector< vector< pair<string, size_t>>> results{}; // doc, freq pairs
    vector< vector< tuple<string, size_t, vector<size_t>>>> resultsLoc{}; //doc, freq, vector<location>
    
    //data structure reference for coding the mess below
    //map< string, vector< map<size_t, vector<size_t>>>> dictionary;
    //map<term, vector<map<doc#, vector<wordCt>>>> dictionary;
    
    // for each item in searchTerm, create a vector< pair<doc#, frequency>> of index matches.
    for( size_t i{0}; i<searchTerms.size(); i++){
        vector< pair<string, size_t>> temp{};
        vector< tuple<string, size_t, vector<size_t>>> docLoc{};
        for ( map< string, vector< map<size_t, vector<size_t>>>>::iterator it = dictionary.begin(); it != dictionary.end(); it++ ) {
            
            if (it->first == searchTerms[i]) { //if there is a match in the dictionary
                //iterate through vector of maps ( doc# & pos)
                for (size_t j{0}; j< it->second.size(); j++) {
                    //itr through second map ( doc# & pos)
                    for ( map<size_t, vector<size_t>>::iterator it2 = it->second[j].begin(); it2 != it->second[j].end(); it2++) {
                        temp.push_back(make_pair(fileListVector[it2->first], it2->second.size()));
                        docLoc.push_back(make_tuple(fileListVector[it2->first], static_cast<size_t>( it2->second.size() ), it2->second));
                    }
                }
                //sort(temp.begin(), temp.end(), sortBySec); //sort the vector by freq
                results.push_back(temp);
                resultsLoc.push_back(docLoc);
            }
        }
    }
    //if there are results,
    if (results.size()>0) {
        
        //single term search
        if ( searchTerms.size() == 1){
            // sort by freq
            sort(results[0].begin(), results[0].end(), sortBySec);
            
            cout << results[0].size()<< " results:"<< endl;
            for ( size_t n{0}; n< results.size(); n++){
                for ( size_t m{0}; m<results[n].size(); m++) {
                    cout << results[n][m].first << " "<< results[n][m].second << endl;
                    
                }
            }
        }
        
        //boolean search - if searchTerms[1] == "and"
        if( searchTerms[1] == "and" || searchTerms[1] == "AND") {
            if (searchTerms.size() == 2) {
                cout << "No search term after \"and\" "<<endl;
            }
            else if( (searchTerms.size() == 3) && (results.size()==1)){
                for (size_t i{0} ; i<results[0].size(); i++) {
                    cout<< results[0][i].first<<endl;
                }
            }
            
            else {
                string chk{searchTerms[1]};
                cleanString(chk);
                if(chk == "and") {
//                    cout << "outputting intermediate results:"<<endl;
//                    for ( size_t i{0}; i < results.size(); i++) {
//                        for ( size_t j{0}; j< results[i].size(); j++) {
//                            cout<< results[i][j].first << " " << results[i][j].second <<endl;
//                        }
//
//                    }
                    vector<string> ans{intersect(results)};
                    //                cout<< "Outputting intersection:"<<endl;
                    cout<< ans.size() << " results: "<< endl;
                    for (size_t i{0}; i < ans.size(); i++) {
                        cout<< ans[i]<< endl;
                    }
                }
            }
        }
        //two term search - if searchTerms.size() == 2
        if ( results.size() == 2 && searchTerms[1] != "and") {
            
            vector<pair<string, size_t>> ans{};
            
            //for each term in results, check in the second vector for same doc # and position +=1
            for ( vector<tuple<string, size_t, vector<size_t>>>::iterator it1= resultsLoc[0].begin(); it1 != resultsLoc[0].end(); it1++) {
                for (vector<size_t>::iterator it1a = get<2>(*it1).begin(); it1a!= get<2>(*it1).end(); it1a++) {
                    
                    for ( vector<tuple<string, size_t, vector<size_t>>>::iterator it2 = resultsLoc[1].begin(); it2 != resultsLoc[1].end(); it2++) {
                        string d1{get<0>(*it1)}, d2{get<0>(*it2)};
                        
                        for(vector<size_t>::iterator it2a = get<2>(*it2).begin(); it2a!= get<2>(*it2).end(); it2a++) {
                            size_t p1{*it1a}, p2{*it2a};
                            if ( (d1 == d2) && ( p1 == p2 - 1)) {
                                ans.push_back(make_pair(d1, p1));
                            }
                        }
                    }
                }
            }
            if ( ans.size() == 0) {
                cout << "There is no \"" << q << "\" in our system." <<endl;
            }
            else {
                for (auto p : ans) {
                    cout<< p.first << " " << p.second<< endl;
                }
            }
            
        }
        
        
        //wildcard search - if searchTerms[i] contains '*'
        
        
        
    }

    
    else { //if there are no results
        cout << "There is no \"" << q << "\" in our system." <<endl;
        
    }
    
    // check search term against dictionary. If exists, return results. Else, return error msg
//    for ( map< string, vector< map<size_t, vector<size_t>>>>::iterator it = dictionary.begin(); it != dictionary.end(); it++ ) {
//        if ( it->first == q) {
//            cout << it->first <<endl;
//            //iterate through vector of maps ( doc# & pos)
//            for ( size_t j{0}; j< it->second.size(); j++) {
//
//                //iterate through second map (doc# & pos)
//                for ( map<size_t, vector<size_t>>::iterator it2 = it->second[j].begin(); it2 != it->second[j].end(); it2++){
//
//                    cout<< fileListVector[ it2->first] << " " << it2->second.size(); //print doc #
//
//                    //                                //print all term locations
//                    //                                for ( size_t k{0}; k< it2->second.size(); k++) {
//                    //                                    cout << it2->second[k] << " ";
//                    //                                }
//                    cout << endl;
//
//                }
//            }
//            break;
//        }
//        else if ( next(it) == dictionary.end()) {
//            cout << "There is no \""<< q << "\" in our system." <<endl;
//        }
//    }
}



vector<string> intersect(vector<vector<pair<string, size_t>>> v) {
    
    vector<vector<string>> p{};
    const int numLists{2}; //the number of lists we can intersect
    
    for ( size_t i{0}; i< numLists; i++) {
        vector<string> result{};
        for( size_t j{0}; j<v[i][j].first.size(); j++) {
            result.push_back(v[i][j].first);
        }
        p.push_back(result);
    }
    
    vector<string> p1{p[0]}, p2{p[1]}, ans;
    
    //iterators for p1 and p2
    vector<string>::iterator it1{p1.begin()}, it2{p2.begin()};
    while ( it1<p1.end() && it2< p2.end()) {
        if ( *it1 == *it2) {
            ans.push_back(*it1++);
            it2++;
        }
        else {
            if ( *it1 < * it2) {
                it1++;
            }
            else {
                it2++;
            }
        }
    }
    return ans;
}
