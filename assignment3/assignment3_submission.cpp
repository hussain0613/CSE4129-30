#include<bits/stdc++.h>
#include<ctype.h>
using namespace std;

unordered_set<string> valid_token_types = {"kw", "id", "op", "num", "sep", "par", "brc", "brkt", "str", "char", "unkwn"};



/*
Identifies various conditions in a character based on current char and state
Parameters:
    char ch - the character to examine
    int curr_state - current_state
Returns -
    -2: found closing mark (') for the invalid character token
    -1: invalid character token
    0: not part of an character
    1: part of a character [if curr_state was 0 then the opennig mark (') has just been found]
    2: part of a character, the 'actual character' has been found
    3: part of a character and back slash i.e. the escape character found [so the immidiate mark (') (if any) will not be the closing mark,
        also the lenght of the char has increase now
    4: end of a character, i.e. c is the closing mark
*/
int is_in_char(char c, int curr_state);



/*
Identifies various conditions in a string based on current char and state
Parameters:
    char ch - the character to examine
    int curr_state - current_state
Returns -
    0: not part of an string
    1: part of a string [if curr_state was 0 then the opennig mark (") has just been found]
    2: part of a string and back slash i.e. the escape character found [so the immidiate mark (") (if any) will not be the closing mark
    3: end of a string i.e. ch is the closing mark
*/
int is_in_str(char ch, int curr_state);



/*
- Fetches the tokens from assignment2 output file and returns them as vector of string string pairs
- Exists program with errror messages if the file is not in the right format or if finds any unidentified token type
- Is not concerned with token's correctness, only concerned with the structure/format
- The file should not contain any newlines
- The file should end with ']' or space
Parameter:
    string fn - filename of the file to be read
Returns:
    - a vector containing the token types and token in pair of strings.
*/
vector<pair<string, string> > get_tokens(string fn);



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



int main()
{
    vector<pair<string, string> > tokens = get_tokens("input.txt");

    SymbolTable table = construct_table(tokens);


    table.display();

    return 0;
}



int is_in_char(char c, int curr_state)
{
    if(curr_state == 0 && c == '\''){
        curr_state = 1;
    }
    else if(curr_state == 1){
        if(c == '\'') curr_state = -2;
        else if (c == '\\')curr_state = 3;
        else curr_state = 2;
    }
    else if(curr_state == 2){
        if(c == '\'') curr_state = 4; // final state
        else curr_state = -1;
    }
    else if(curr_state == 3){
        curr_state = 2;
    }
    else if(curr_state == -1 && c == '\''){
        curr_state = -2;
    }

    return curr_state;
}



int is_in_str(char ch, int curr_state)
{
    if(curr_state == 0 && ch == '"'){ // curr_state will change from 0 only if the current character is '"'
        curr_state = 1;
    }
    else if(curr_state == 1 && ch == '\\'){
        curr_state = 2;
    }
    else if(curr_state == 2){ // escape
        curr_state = 1;
    }
    else if(curr_state == 1 && ch == '"'){
        curr_state = 3; // final_state
    }
    return curr_state;
}



vector<pair<string, string> > get_tokens(string fn)
{
    vector<pair<string, string> > tokens;

    FILE *fp = fopen(fn.c_str(), "r");

    if(fp == NULL){
        printf("[!] Can't open input file '%s'.\nExiting program...", fn.c_str());
        exit(EXIT_FAILURE);
    }

    int state = 0;
    string token_type = "", token = "";
    int in_char = 0, in_str = 0, cnt = 0;

    char c = fgetc(fp);
    while(c != EOF){
        if(state == 0){
            if(c == '[') state = 1;
            else{
                printf("[!] Error in input file. Expected '[' as char#%d. Found '%c' instead.\nExiting...", cnt, c);
                exit(EXIT_FAILURE);
            }
        }
        else if(state == 1){
            if(c == ' ') state = 2;
            else if(islower(c)) {
                state = 3;
                token_type += c;
            }
            else{
                printf("[!] Error in input file. Expected ' '(a space) or a lower case alphabet as char#%d for token type after '['. Found '%c' instead.\nExiting...", cnt, c);
                exit(EXIT_FAILURE);
            }
        }
        else if(state == 2){

            if(islower(c)) {
                state = 3;
                token_type += c;
            }
            else{
                printf("[!] Error in input file. Expected a lower case alphabet as char#%d for token type. Found '%c' instead.\nExiting...", cnt, c);
                exit(EXIT_FAILURE);
            }
        }
        else if(state == 3){

            if(islower(c)) {
                token_type += c;
            }
            else if(c == ' '){
                state = 4;
            }
            else{
                printf("[!] Error in input file. Expected a lower case alphabet as char#%d for token type or a ' '(space). Found '%c' instead.\nExiting...", cnt, c);
                exit(EXIT_FAILURE);
            }
        }
        else if(state == 4){
            if(valid_token_types.find(token_type) == valid_token_types.end()){
                printf("[!] Error in input file. Invalid token type %s' [Ends with char#%d].\nExiting...", token_type.c_str(), cnt-1);
                exit(EXIT_FAILURE);
            }

            if(!in_str && (in_char = is_in_char(c, in_char)) > 0 && in_char <= 4){
                token += c;
                if(in_char == 4) in_char = 0;
            }
            else if( (in_str = is_in_str(c, in_str)) > 0 && in_str <= 3){
                token += c;
                if(in_str == 3) in_str = 0;
            }
            else if(c == ' '){
                state = 5;
            }
            else if(c == ']'){
                if(token.size() == 0){ // this will restrict the appearance of the ] to one instance only, except for strings and chars
                    token += c;
                    state = 5;
                }
                else{
                    state = 6;
                }
            }
            else{
                token += c;
            }
        }
        else if(state == 5){
            if(c == ']'){
                state = 6;
            }
            else{
                printf("[!] Error in input file. Expected ']' as char#%d. Found '%c' instead.\nExiting...", cnt, c);
                exit(EXIT_FAILURE);
            }
        }
        else if(state == 6){
            tokens.push_back({token_type, token});
            token_type = "";
            token = "";

            if(c == '[') state = 1;
            else if (c == ' ') state = 0;
            else{
                printf("[!] Error in input file. Expected '[' or ' '(space) as char#%d. Found '%c' instead.\nExiting...", cnt, c);
                exit(EXIT_FAILURE);
            }
        }

        c = fgetc(fp);
        ++cnt;
    }

    fclose(fp);

    if(state == 6){
        tokens.push_back({token_type, token});
    }
    else if(state != 0){
        printf("[!] Error in input file. Expected ']' or ' '(space) as the last char. Found '%c' instead.\nExiting...", cnt, c);
        exit(EXIT_FAILURE);
    }

    return tokens;
}



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


SymbolTable construct_table(vector<pair<string, string> > tokens)
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
