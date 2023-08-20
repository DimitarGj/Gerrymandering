//  Dimitar Gjorgievski, UIN 650730211 
//  CS 251
//  Project 1 - Gerrymandering
//  
//

#include "iostream"
#include "string"
#include "ourvector.h"
#include "fstream"
#include <cmath>
using namespace std;

typedef struct StateInfo State;
//Structure that stores every available info of a given state
struct StateInfo
{
    string name;
    ourvector<int> dem_votes;
    ourvector<int> rep_votes;
    int el_voters;
};

//Printing plot of a district of a given state
void PrintPlot(int D, int R, int n){
    
    cout<<"District: "<<n+1<<endl;
    
    for(int i = 0; i<D; i++){
        cout<<"D";
    }
    
    for(int i = 0; i<R; i++){
        cout<<"R";
    }

    cout<<endl;

}

//This functions plots the data for a given state
void PlotData(ourvector<State>& States, int& s){

    for(int i=0; i< States[s].dem_votes.size(); i++){
        float D;
        int R;
        if(States[s].dem_votes[i]!=States[s].rep_votes[i]){
            D = (float)(States[s].dem_votes[i]*100/(States[s].dem_votes[i]+States[s].rep_votes[i]));
            R = 100 - (int)D;
        }else{
           D = 0.0;
           R = 0;
        }
        PrintPlot(D, R, i);
        
    }

}

//This function prints the stats for a given state
void PrintStats(bool gerrymandering, int wasted_dem_votes, int wasted_rep_votes, int eligible_voters, double eff_gap, string winner){
    
    if(gerrymandering == true){
        cout<<"Gerrymandered: Yes"<<endl;
        if(winner=="Democrats"){
            cout<<"Gerrymandered against: Republicans"<<endl;
        }else{
            cout<<"Gerrymandered against: Democrats"<<endl;
        }
        cout<<"Efficiency Factor: "<<eff_gap<<"%"<<endl;
        cout<<"Wasted Democratic votes: "<<wasted_dem_votes<<endl;
        cout<<"Wasted Republican votes: "<<wasted_rep_votes<<endl;
        cout<<"Eligible voters: "<<eligible_voters<<endl;
    
    }else{
        cout<<"Gerrymandered: No"<<endl;
        cout<<"Wasted Democratic votes: "<<wasted_dem_votes<<endl;
        cout<<"Wasted Republican votes: "<<wasted_rep_votes<<endl;
        cout<<"Eligible voters: "<<eligible_voters<<endl;

    }
}

//This function determines the wasted votes for a given party in a given state
void WastedVotes(int& wasted_dem_votes, int& wasted_rep_votes, ourvector<State>& States, int s){
    
    for(int i=0; i<States[s].dem_votes.size(); i++){
        int over_half = 0;
        int total_votes = 0;
        
        if(States[s].dem_votes[i]>States[s].rep_votes[i]){
            total_votes = States[s].dem_votes[i] + States[s].rep_votes[i];
            over_half = (total_votes/2) + 1; 
            wasted_dem_votes += (States[s].dem_votes[i] - over_half);
            wasted_rep_votes += (total_votes - States[s].dem_votes[i]);
        
        }else{
            total_votes = States[s].dem_votes[i] + States[s].rep_votes[i];
            over_half = (total_votes/2) + 1; 
            wasted_rep_votes = wasted_rep_votes + (States[s].rep_votes[i] - over_half);
            wasted_dem_votes = wasted_dem_votes + (total_votes - States[s].rep_votes[i]);
        }
    }    
}

//This function counts the total votes and determines the winner
void TotalVotesWinner(int& total_dem_votes, int& total_rep_votes, string& winner, int& total, ourvector<State>& States, int s){
    
    for(int i=0; i<States[s].dem_votes.size(); i++){
        total_dem_votes+=States[s].dem_votes[i];
        total_rep_votes+=States[s].rep_votes[i];
    }

    total = total_dem_votes + total_rep_votes;

    if(total_dem_votes > total_rep_votes){
        winner = "Democrats";
    }else{
        winner = "Republicans";
    } 
}

//This function calculates all stats of a given state
void Stats(ourvector<State>& States, int& s){
  
     int total_dem_votes=0;
     int total_rep_votes=0;
     string winner;
     int total_votes = 0;
     int wasted_dem_votes = 0;
     int wasted_rep_votes = 0;
     bool gerrymandered;
     
    TotalVotesWinner(total_dem_votes, total_rep_votes, winner, total_votes, States, s);
    WastedVotes(wasted_dem_votes, wasted_rep_votes, States, s);
    
    double efficiency_gap = round((abs(wasted_dem_votes-wasted_rep_votes)*100.0 / total_votes) * 10000.0) / 10000.0;
    
    if((int)efficiency_gap>=7){
        gerrymandered = true;    
    }else if((int)efficiency_gap<7){
        gerrymandered = false;
    }

    if(States[s].dem_votes.size()<3){
        gerrymandered = false;
    }

    PrintStats(gerrymandered, wasted_dem_votes, wasted_rep_votes, States[s].el_voters, efficiency_gap, winner);
}

//This function parses each line of the districts file and distributes data to appropriate places
//This function also prints the districts file list
void DistrictParsing(string data_line, ourvector<State>& States, ourvector<int> &dem_votes, ourvector<int> &rep_votes, int i){
    
    int index;
    string name;
    State State_Entry;
    index = data_line.find(','); //Finds index of comma after name of state
    name = data_line.substr(0, index);
    data_line.erase(0, index+1); //Erasing name of state and first comma
    index = data_line.find(','); //Finds index of comma after number of district
    
    while(index!=-1){
        data_line.erase(0, index+1); //Erasing number of district and comma after it
        index = data_line.find(','); //Finds index of comma after Democrat votes in district
        dem_votes.push_back(stoi(data_line.substr(0, index)));
        data_line.erase(0, index+1); //Erasing number of Democrat votes and comma after it
        index = data_line.find(','); //Finds index of comma after Republican votes iin district
        rep_votes.push_back(stoi(data_line.substr(0, index)));
        data_line.erase(0, index+1); //Erasing number of Republican votes and comma after it
        index = data_line.find(','); //Look for next comma after number of district
    }
    
    State_Entry = {name, dem_votes, rep_votes, 0};
    States.push_back(State_Entry);
    dem_votes.clear();
    rep_votes.clear();

    cout<<"..."<<States[i].name<<"..."<<States[i].dem_votes.size()<<" districts total"<<endl;
}

//This function parses each line of the eligible voters file and distributes data to appropriate places
//This function also prints the eligible voters list
void ElVotersParsing(string data_line, ourvector<State>& States){
    
    string state_name;
    int index;
    index = data_line.find(',');
    state_name= data_line.substr(0, index);
    data_line.erase(0, index+1);
    
    for(int i=0; i<States.size(); i++){
        if(state_name==States[i].name){
            States[i].el_voters = stoi(data_line);
            cout<<"..."<<States[i].name<<"..."<<States[i].el_voters<<" eligible voters"<<endl;
            break;
        }
    }
}

//This function opens the files and reads the contents inside them
void ReadingFiles(string file1, string file2, ourvector<State>& States, ourvector<int> &dem_votes, ourvector<int> &rep_votes, bool& read){
    
    ifstream inF;
    string data_line; //string thast stores each line of a given file
    int line_number = 0; //tracks index of state that has been read
    
    inF.open(file1);
    
    if(!inF.is_open()){
        cout<<"Invalid first file, try again."<<endl;
        cout<<endl;
        return ;
    }
    getline(inF, data_line);
    
    cout<<"Reading: "<<file1<<endl;
    
    while (!inF.fail() && !inF.eof())
    {
        DistrictParsing(data_line, States, dem_votes, rep_votes, line_number);
        getline(inF, data_line);
        line_number++;
    }
    
    inF.close();
    
    inF.open(file2);
    
    if(!inF.is_open()){
        cout<<endl;
        cout<<"Invalid second file, try again."<<endl;
        cout<<endl;
        return ;
    }
    
    getline(inF, data_line);
    
    cout<<endl;
    cout<<"Reading: "<<file2<<endl;
    
    while (!inF.fail() && !inF.eof())
    {
        ElVotersParsing(data_line, States);
        getline(inF, data_line);
    }
    
    cout<<endl;
    
    inF.close();
    read = true;
}

int main() {
    
    ourvector<State> States;
    ourvector<int> dem_votes;
    ourvector<int> rep_votes;
    string command = " ";
    string file1;
    string file2;
    string state_name = "N/A";
    bool read_data = false; //marker to check if data was read successfully
    int state_index; //saving state index when searching for state 

    cout<<"Welcome to the Gerrymandering App!"<<endl;
    cout<<endl;
    cout<<"Data loaded? No"<<endl;
    cout<<"State: "<<state_name<<endl;
    cout<<endl;
    cout<<"Enter command: ";
    
    while (command!="exit")
    {
        cin>>command;
        
        if(command=="load"){
            cin>>file1;
            cin>>file2;
            cout<<endl;
            cout<<"-----------------------------"<<endl;
            cout<<endl;
            
            if(read_data==true){
                cout<<"Already read data in, exit and start over."<<endl;
                cout<<endl;

            }else{
                ReadingFiles(file1, file2, States, dem_votes, rep_votes, read_data);
            }
            
            if(read_data==true){
                cout<<"Data loaded? Yes"<<endl;
                cout<<"State: "<<state_name<<endl;
            
            }else{
                cout<<"Data loaded? No"<<endl;
                cout<<"State: "<<state_name<<endl;
            }
            
            cout<<endl;
            cout<<"Enter command: ";
        
        }else if(command=="search"){
            
            string save_name = state_name; //Saves name of previously entered state in case user enters invalid state
            
            getline(cin, state_name);
            state_name.erase(0,1); //erasing space inputted in getline
            int index = state_name.find(' '); 
           
            state_name[0] = toupper(state_name[0]);
        
            if(index != -1){
                state_name[index+1] = toupper(state_name[index+1]);
            }else{
                for(int i = 1; i<state_name.size(); i++){
                    state_name[i] = tolower(state_name[i]);
                }
            }

            cout<<endl;
            cout<<"-----------------------------"<<endl;
            cout<<endl;
            
            if(read_data==true){
            
            bool state_found = false;

            for(int i=0; i<States.size(); i++){
                if(state_name==States[i].name){
                     state_found = true;
                     state_index = i;
                     break;
                }
            }

           if(state_found==false){
                state_name = save_name; //If user enters invalid state, the value of state_name remains previously entered state
                cout<<"State does not exist, search again."<<endl;
                cout<<endl;
           }

            cout<<"Data loaded? Yes"<<endl;
            cout<<"State: "<<state_name<<endl;
           
            }else{
                state_name = "N/A";
                cout<<"No data loaded, please load data first."<<endl;
                cout<<endl;
                cout<<"Data loaded? No"<<endl;
                cout<<"State: "<<state_name<<endl;
            }
            
            cout<<endl;
            cout<<"Enter command: ";
       
        }else if(command=="stats"){
            cout<<endl;
            cout<<"-----------------------------"<<endl;
            cout<<endl;
            
            if(state_name!="N/A"){
                Stats(States, state_index);
                cout<<endl;
                cout<<"Data loaded? Yes"<<endl;
                cout<<"State: "<<state_name<<endl;
                cout<<endl;
            
            }else{
                cout<<"No state indicated, please search for state first."<<endl;
                cout<<endl;
                cout<<"Data loaded? Yes"<<endl;
                cout<<"State: "<<state_name<<endl;
                cout<<endl;
            }
            cout<<"Enter command: ";
        
        }else if(command=="plot"){
            cout<<endl;
            cout<<"-----------------------------"<<endl;
            cout<<endl;
            PlotData(States, state_index);
            cout<<endl;
            cout<<"Data loaded? Yes"<<endl;
            cout<<"State: "<<state_name<<endl;
            cout<<endl;
            cout<<"Enter command: ";
        }
    }
    
    cout<<endl;
    cout<<"-----------------------------"<<endl;
    
    return 0;
}
