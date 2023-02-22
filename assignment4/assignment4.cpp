#include<bits/stdc++.h>
#include<ctype.h>
#include "utils.cpp"
#include "assignment1_submission.cpp"
#include "assignment2.cpp"
#include "assignment3_submission.cpp"



using namespace std;



int main()
{
    FILE *src_fp, *dest_fp;
    
    src_fp = fopen("input.txt", "r");
    dest_fp = fopen("output.txt", "w");
    
    vector<pair<int, string> > lines = filter(src_fp, dest_fp);

    for(auto itr = lines.begin(); itr != lines.end(); ++itr){
        cout << itr->first << ". " << itr->second << "\n";
    }
    cout << "\n";

    fclose(src_fp);
    fclose(dest_fp);

    src_fp = fopen("output.txt", "r");
    vector<Triple<string, string> >  tokens = tokenize(src_fp);
    fclose(src_fp);

    // for(auto itr = tokens.begin(); itr != tokens.end(); ++itr){
    //     cout << "[ " << itr->first << " " << itr->second << " ]";
    // }
    // cout << "\n";

    SymbolTable symbol_table = construct_table(tokens);

    symbol_table.display();



    return 0;
}


