/*
CS 302 Assignment 7

Name: Keith Beauvais, 5005338658, CS 302 Assignment 7
*/
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "hashMap.h"

int main(){
    std::string filename, canidateName, canidateParty, voteName, nameOfWinner;
    std::ifstream infile;
    int testCase, counter = 1, numberOfCanidates, numberOfVotes, maxVotes, maxVotesTemp; 
    bool tieFlag = false;

    hashMap<std::string, std::string> canidate;
    hashMap<std::string, int> votesPerCanidate;

    std::vector<std::string> vectorOfNames;
    
    //open file
    std::cout << "Enter filename: ";
    std::cin >> filename;
    
    infile.open(filename);

    infile>>testCase;


    do{
        infile>>numberOfCanidates;
        infile.ignore(50,'\n');
        /*
        reads in the name of the canidate and the political party
        creates hashMap with the canidate name as key and canidate party as value
        creates another hashMap with the canidate name as key and the value as the 
        amount of votes they recieve (initially set to 0). reads in canidates name into 
        a vector to use later. 
        */
        for(int i = 0; i<numberOfCanidates; i++){  
            getline(infile, canidateName);
            getline(infile, canidateParty);
            canidate[canidateName]=canidateParty;
            votesPerCanidate[canidateName]=0;
            vectorOfNames.push_back(canidateName);
            infile.clear();
        }
        
        infile>> numberOfVotes;
        infile.ignore(50,'\n');
        
        for(int i = 0; i<numberOfVotes; i++){
            getline(infile, voteName);
            votesPerCanidate[voteName]++; 
        }

        // sets the max amount of votes at index 0
        maxVotes = votesPerCanidate[vectorOfNames[0]];
        // sets the winner of person at index 0
        nameOfWinner = vectorOfNames[0];

        // goes through the vector to count the number of votes using the [] operator as a search
        for(int i= 1; i<vectorOfNames.size();i++){
            maxVotesTemp = votesPerCanidate[vectorOfNames[i]];
            if(maxVotesTemp==maxVotes){
                tieFlag = true;
            }
            if(maxVotesTemp>maxVotes){
                maxVotes = maxVotesTemp;
                nameOfWinner = vectorOfNames[i];
                tieFlag = false;
            }
        }
        if(tieFlag == false){
            std::cout<< "Case "<< counter <<" results: "<<canidate[nameOfWinner]<<std::endl;
        }
        else{
            std::cout<< "Case "<< counter <<" results: "<< "Tie"<<std::endl;
        }
        
        counter++;
        vectorOfNames.clear();
    }while(counter!=testCase+1);

    return 0;

}

