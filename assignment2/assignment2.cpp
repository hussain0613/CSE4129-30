#include<bits/stdc++.h>
using namespace std;

unordered_set<char> seperators = {',', ';'};
unordered_set<char> operators = { '+' , '-', '*', '/', '%' , '<', '>', '=', '!', '|', '&', '^', '.', '?', ':'};
unordered_set<string> multi_char_ops = { // multiple character operators
    "++", "--", "+=", "-=", "*=", "/=", "%=", "<=", ">=", "==",
    "!=", "||", "&&", "|=", "&=", "^=", "<<", ">>", "<<=", ">>=", "->"
    };

unordered_set<char> braces = {'(', ')', '{', '}', '[', ']'};
unordered_set<string> keywords = {
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if",
    "int","long","register","return","short","signed","sizeof","static",
    "struct","switch","typedef","union","unsigned","void","volatile","while"
    };



/*
Identifies various conditions in a string based on current char and state

char ch - the character to examine
int curr_state - current_state

returns - 
    0: not part of an string
    1: part of a string [if curr_state was 0 then the opennig mark (") has just been found]
    2: part of a string and back slash i.e. the escape character found [so the immidiate mark (") (if any) will not be the closing mark
    3: end of a string i.e. ch is the closing mark
*/
int is_in_str(char ch, int curr_state);



/*
Identifies various conditions in a character based on current char and state

char ch - the character to examine
int curr_state - current_state

returns -
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
Gives tokens types for given token
*/
string get_token_type(string token);



/*
A function to 'tokenize' a 'filtered' C source code.
Here filtered means all comments and all extra white spaces are removed
Also assumes, there's no preprocessor directives in the src file 

FILE *src - the source file to tokenize
returns - a list of pairs of strings
    where the first element is the token type and the second element is the token
*/
list<pair<string, string> > tokenize(FILE *src);



int main()
{
    FILE *fp;

    string fn; // file name

    char c;

    cout << "Path to filtered source file: ";
    cin >> fn;
    
   
    fp = fopen(fn.c_str(), "r");

    if(fp == NULL){
        printf("Source file can't be opened!\n");
        exit(0);
    }
    
    printf("Tokenizing...\n");
    auto tokens = tokenize(fp);
    printf("Tokenized succesfully.\n");

    fclose(fp);

    fp = fopen("output.txt", "w");
    if(fp == NULL){
        printf("Output file can't be opened!\n");
        exit(0);
    }

    for(auto itr = tokens.begin(); itr != tokens.end(); ++itr){
        cout << "[ " << itr->first << " " << itr->second << " ]";
        fprintf(fp, "[ %s %s]", itr->first.c_str(), itr->second.c_str());
    }
    cout << "\n";

    return 0;
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



string get_token_type(string token)
{
    if(token.front() == '"' && token.back() == '"') return "str";
    else if(token.front() == '\'' && token.back() == '\'' && (token.size() == 3 || (token.size() == 4 && token.at(1) == '\''))) return "char";
    else if(token.size() == 1 && seperators.find(token.front()) != seperators.end()) return "sep";
    else if(token.size() == 1 && operators.find(token.front()) != operators.end()) return "op";
    else if(multi_char_ops.find(token) != multi_char_ops.end()) return "op";
    else if(token.size() == 1 && braces.find(token.front()) != seperators.end()) return "par";
    else if(keywords.find(token) != keywords.end()) return "kw";
    else {
        if(isalpha(token.front()) || token.front() == '_'){ // start of a possible alphanumeric token
            bool flag = true; // true: alphanumeric token, false: unknown, probably something incorrect
            for(int i = 1; i < token.size(); ++i){
                if(! (isalnum(token.at(i)) || token.at(i) == '_')){
                    flag = false;
                    break;
                }
            }
            if(flag) return "id";
        }
        else if(isdigit(token.front()) || token.front() == '.'){
            bool flag = true; // true: number, false: not number
            
            int dot_count = 0;

            for(int i = 0; i < token.size() && dot_count < 2 && flag; ++i){ // starting from 0 in case token.front() was a dot (.)
                if(token.at(i) == '.'){
                    ++dot_count;
                }
                else if(! (isdigit(token.at(i))) ) {
                    flag = false;
                }
            }
            if(flag && dot_count < 2) return "num";
        }
        
        return "unkwn";
    }
}



list<pair<string, string> > tokenize(FILE *src)
{
    char c;
    int in_str = 0;
    int in_char = 0;
    string token = "", token_type = "";
    int state = 0; // 0: empty, 1: char, 2: str, 3: opt_grp, 4: op or num 5: num, 6: others
    list<pair<string, string> > tokens;
    

    c = fgetc(src);
    while( c != EOF ){
        int flag = 0; // 0: not complete 'word' yet, 1: complete word, type still undetermined, 2: complete word, type already determined
        bool flag2 = true; // true: read new char, false: don't read

        if(!in_str  && ((in_char = is_in_char(c, in_char)) != 0)){ // !in_str- to 'escape' any single quote (') inside a string as
                                                                    // it does not start any 'character'
            if(!state || state == 1){ // start of a brand new token and a char or part of an 'on going' char token
                token += c;
                state = 1;
            }
            else{ // a new char token has been found, but their is an existing unprocessed token, need to process that one first
                flag = 1; // complete token found, yes, but type unknown still
                flag2 = false; // direction to not read a  new char as the current char is yet to be 'proccessed'
                in_char = 0; // not inside a char token yet technacally, 
                             // the char token will start in the next iteration
            }

            if(in_char == 4){ // end of the character token has been found
                in_char = 0; // not part of a char token anymore
                flag = 2; // so complete token found and type is unknown because of possibility of invalid character
                token_type = "char";
            }
            else if(in_char == -2){
                in_char = 0;
                flag = 2;
                token_type = "unkwn";
            }
        }
        else if((in_str = is_in_str(c, in_str))){
            if(!state || state == 2){
                token += c;
                state = 2;
            }
            else{
                flag = 1;
                flag2 = false;
                in_str = 0;
            }
        
            if(in_str == 3){ // end of string found
                in_str = false;
                flag = 2;
                token_type = "str";
            }
        }
        else if(operators.find(c) != operators.end()){
            if(!state || state == 3 || state == 4){ // either start of a new op_grp token or part of an existing op_grp
                                                    // or an operator right after a dot (.) which should be a syntax error
                token += c;
                if(c == '.' && !state) state = 4; // this dot is the first and/or only char in the token
                else state = 3;
            }
            else if(c == '.' && state == 5){ // the dot has come right after a number, so probably a decimal point
                token += c;
            }
            else{
                flag = 1; // complete word in token, but type unknown
                flag2 = false;
            }
        }
        else if(isdigit(c)){
            if(!state || state == 4 || state == 5){
                token += c;
                state = 5;
            }
            else if(state == 6){ // right after something that's not a char, str or any of the other things, so probably part of an identifier
                token += c;
            } 
            else{
                flag = 1; // complete word in token, but type unknown
                flag2 = false;
            }
        }
        else if(seperators.find(c) != seperators.end() || braces.find(c) != braces.end() || iswspace(c)){
            if(!token.empty()) {
                flag = 1; // complete token but type unknown still
                if(!isspace(c)) flag2 = false; // if current char not space than it needs to be checked and processed,
                                                // so directions to not read the next char, if space, will discard when reading the next char
            }
            else if (!iswspace(c)){ // start of a new token
                token += c;
                flag = 1; // as separators and braces are all single character token so we have found the complete token, but type not sure yet
            }
        }
        else{
            if(!state || state == 6){ // start or part of a (new) unknown type of token probably keywords, identifiers or something invalid
                token += c;
                state = 6;
            }
            else{ // previous token not yet processed
                flag = 1;
                flag2 = false;
            }
        }

        if(flag == 1){
            token_type = get_token_type(token);
        }
        if(flag){
            tokens.push_back({token_type, token});
            token = ""; // resetting token
            state = 0; // resetting state
        }

        if(flag2){
            c = fgetc(src);
        }

    }
    return tokens;
}

