#include<bits/stdc++.h>
#include<ctype.h>
#include "utils.cpp"

using namespace std;

unordered_set<string> valid_token_types = {"kw", "id", "op", "num", "sep", "par", "brc", "brkt", "str", "char", "unkwn"};



/*
A custom data structure to represent the rows of the symbol table
*/
struct SymbolTableRow{
    int sl;
    string name, id_type, data_type, scope;
    //void *value;
    string value;
};
typedef struct SymbolTableRow SymbolTableRow;



/*
A custom class to represent the symbol table as defined in Session3
*/
class SymbolTable{
    vector<SymbolTableRow> rows;
    // unordered_map< pair<string, string>, int> index; // {(id_name, scope): sl_no, ...}
    unordered_map< string, unordered_map<string, int> > index; // {scope: {id_name: sl_no, ...}, ...}

    public:
    int insert(string name, string scope, string id_type, string data_type, string value);
    int lookup(string name, string scope);
    int update(string name, string scope, string value);
    void display();
    void free();
};



SymbolTable construct_table(vector<pair<string, string> > tokens);




int SymbolTable::insert(string name, string scope, string id_type, string data_type, string value)
{
    int flag = 2; // 0: already exists, do not insert, 1: scope exists, id does not, 2: scope, id nothing exists
    SymbolTableRow row;
    row.name = name;
    row.sl = rows.size() + 1;
    row.value = value;
    row.scope = scope;
    row.id_type = id_type;
    row.data_type = data_type;
    row.value = value;

    if(index.find(scope) != index.end()){
        if(index.at(scope).find(name) == index.at(scope).end()){
            flag = 1;
        }else{
            flag = 0;
        }
    }


    if(flag){
        rows.push_back(row);
        if(flag == 2){
            index[scope] = {};
        }
        index[scope][name] = row.sl;

    }
    return flag;
}

int SymbolTable::lookup(string name, string scope)
{
    if(index.find(scope) != index.end()){
        if(index.at(scope).find(name) != index.at(scope).end()){
            return index.at(scope).at(name);
        }
    }
    return 0;
}


void SymbolTable::display()
{
    printf("Sl. No. \t Name \t Id Type \t Data Type \t Scope \t Value (%d)\n", rows.size());
    for(auto row = rows.begin(); row != rows.end(); ++row){
        cout << row->sl << " \t " << row->name << " \t " << row->id_type << " \t " << row->data_type << " \t " << row->scope << " \t " << row->value;
        cout << "\n";
    }
}


void SymbolTable::free()
{
    rows.clear();
}



int SymbolTable::update(string name, string scope, string value)
{
    if(int sl = lookup(name, scope)){
        rows.at(sl-1).value = value;
        return 1;
    }
    return 0;
}



/*
    A function to construct a SymbolTable from a vector of token_type-token pairs
    Parameters:
        vector<pair<string, string> > tokens - a vector containing the token types and tokens in pairs of strings
    Returns:
        a completed SymbolTable
    
    NOTE: Some serious issues are to be fixed
        1. Unable to detect pointers
        and more
*/
SymbolTable construct_table(vector<Triple<string, string, int> > tokens)
{
    SymbolTable table;
    string scope = "global";
    int state = 0; // 0: global, 1: found func, 2: found params/args 3: found new params 4: end of params/args, 5: inside func
    string last_func = "";
    for(int i = 0; i < tokens.size(); ++i){
        auto curr = tokens.at(i);
        int flag = 2;
        if(curr.first == "id" && (i > 0 && (tokens.at(i-1).first == "kw" || tokens.at(i-1).first == "id")) ){ // issue: return true; counts return as id_type and true as id_name
            if(tokens.at(i+1).second  == "("){
                flag = table.insert(curr.second, scope, "func", tokens.at(i-1).second, ""); // only er ager ta kw or id hoile it's defination
                last_func = curr.second;
                if(flag) {
                    scope = curr.second;
                }
                state = 1;
            }
            else{
                string value = "";
                if(i+2 < tokens.size() && (tokens.at(i + 2).first == "num" || tokens.at(i + 2).first == "char" || tokens.at(i + 2).first == "str")){
                    value = tokens.at(i + 2).second;
                }
                flag = table.insert(curr.second, scope, "var", tokens.at(i-1).second, value);
            }
        }
        else if(curr.first == "id" && (i+2) < tokens.size() && (tokens.at(i + 2).first == "num" || tokens.at(i + 2).first == "char" || tokens.at(i + 2).first == "str") ){
            table.update(curr.second, scope, tokens.at(i+2).second);
        }
        else if(state == 1 && curr.second == "("){ // parameteres/arguments
            state = 2;
            if(scope != "global"){ // parameters
                state = 3;
            }
        }
        else if((state == 2 || state == 3) && curr.second == ")"){ // end of params or args
            state = 4;
        }
        else if(state == 4 && (curr.second == "{" || curr.second == ";")){ // found start of func def
            if(curr.second == "{"){
                state = 5;
                scope = last_func;
            }
            else{ // end of a function, only declaration, no defination yet
                state = 0;
                scope = "global";
                last_func = "";
            }
        }
        else if(state == 5 && curr.second == "}"){ // end of func
            state = 0;
            scope = "global";
            last_func = "";
        }
    }

    return table;
}
