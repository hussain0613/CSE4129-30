#include<bits/stdc++.h>
#include<ctype.h>
#include "utils.cpp"
#include "assignment1_submission.cpp"
#include "assignment2.cpp"
#include "assignment3_submission.cpp"



using namespace std;



int main()
{
    FILE *src_fp;
    
    src_fp = fopen("input.txt", "r");
    
    vector<pair<int, string> > lines = filter(src_fp);

    printf("Lines:\n");
    for(auto itr = lines.begin(); itr != lines.end(); ++itr){
        cout << itr->first << ". " << itr->second << "\n";
    }
    cout << "\n";

    fclose(src_fp);
    
    vector<Triple<string, string, int> >  tokens = tokenize(lines);

    printf("Tokens:\n");
    for(auto itr = tokens.begin(); itr != tokens.end(); ++itr){
        cout << "[ " << itr->first << " " << itr->second << " " << itr->third << " ]";
    }
    cout << "\n";

    printf("Symbol Table:\n");
    SymbolTable symbol_table = construct_table(tokens);

    symbol_table.display();



    return 0;
}


