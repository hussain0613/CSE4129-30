#include<bits/stdc++.h>
using namespace std;

int test_int = 4;
float test_flt = 5.9;
float test_flt2 = .05;
float test_flt3 = 60.;
string test_str = "alsdkj ' \" \' asd";

unordered_set<char> seperators = {',', ';'};
unordered_set<char> operators = { '+' , '-', '*', '/', '%' , '<', '>', '=', '!', '|', '&', '^', '.', '?', ':'};
unordered_set<string> multi_char_ops = {
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



bool is_in_str(char ch, bool curr_state, FILE *src);



/*
Parses a src file and filter out the extra whitespaces and comments and write it to a dest file
*/
void filter(FILE *src, FILE *dest);



/*
Detects and filters out comments in src file by moving the pointer to the end of the comment if found
i.e. after the terminating character of the comment

char c - last read char from the file, to start detection with
FILE *src - src file pointer
returns - true if comment detected, false otherwise 
*/
bool skip_comments(char c, FILE *src);

int is_in_char(char c, int curr_state);

string get_token_type(string token);

list<pair<string, string> > tokenize(FILE *src, FILE *dest);


int main()
{
    test_int += 50;
    FILE *src_fp, *output_fp;

    string fns, fnc;

    char c;

    // cout << "Path to source file: ";
    // cin >> fns;
    fns = "assignment2.cpp";
    fnc = "output.txt";

    src_fp = fopen(fns.c_str(), "r");
    output_fp = fopen(fnc.c_str(), "w");

    printf("Starting Filtering...\n");

    filter(src_fp, output_fp);

    printf("Filtered succesfully!\n");

    fclose(src_fp);
    fclose(output_fp);


    src_fp = fopen(fns.c_str(), "r");
    output_fp = fopen(fnc.c_str(), "r");

    printf("Tokenizing...\n");
    tokenize(output_fp, src_fp);
    printf("Tokenized succesfully\n");

    // cout << "\nInput File (" << fns << "):\n\n";
    // while( (c = fgetc(src_fp)) != EOF) cout << c;

    // cout << endl << endl << "Output File (" << fnc << "):\n\n";
    // while( (c = fgetc(output_fp)) != EOF) cout << c;
    // cout << endl << endl;

    return 0;
}


bool is_in_str(char ch, bool curr_state, FILE *src)
{
    if(ch == '"'){ // curr_state only will change if the current character is '"'
        if(!fseek(src, -2L, SEEK_CUR)){
            ch = fgetc(src); // reading the character before the double quotaion (") character
            // if already in_str and the previous character is a backslash (\) then it's still in the str i.e. in_str = in_str
            // or if not in_str and the previous char is a single quotation mark (') then it's still outside any str
            if( !((curr_state && ch == '\\') || (!curr_state && ch == '\'')) ) curr_state ^= 1;
            fgetc(src); // returning the file pointer to where it is supposed be
        }
    }
    return curr_state;
}

int is_in_str2(char ch, int curr_state, FILE *src)
{
    if(curr_state == 0 && ch == '"'){ // curr_state only will change if the current character is '"'
        curr_state = 1;
    }
    else if(curr_state == 1 && ch == '\\'){
        curr_state = 2;
    }
    else if(curr_state == 2){ //escape
        curr_state = 1;
    }
    else if(curr_state == 1 && ch == '"'){
        curr_state = 3; // final_state
    }
    return curr_state;
}


void filter(FILE *src, FILE *dest)
{
    char c;
    bool in_str = false;
    c = fgetc(src);
    while(c != EOF){
        if(c == '"'){
            fputc(c, dest);
            in_str = is_in_str(c, in_str, src);
        }
        else if(!in_str && skip_comments(c, src));
        else if(!in_str && (c == ' ' || c == '\n' || c == '\r' || c == '\t')){
            fputc(' ', dest);
            while( (c = fgetc(src)) != EOF && (c == ' ' || c == '\n' || c == '\r' || c == '\t' || skip_comments(c, src)));
            continue; // at the end of parsing these whitespaces we got a character that has not been checked
                        // so we need to check this one before fetching a new character from the src
        }
        else{
            fputc(c, dest);
        }

        c = fgetc(src);
    }
}



bool skip_comments(char c, FILE *src)
{
    if(c == '/'){ // now there's a chance that it might be the start of a comment
        
        c = fgetc(src);
        
        if(c != EOF){
            if(c == '/'){ // surely a single-line comment
                while( (c = fgetc(src)) != EOF && c != '\n');
                return true;
            }
            else if(c == '*'){ // surely start of a multiline comment
                c = fgetc(src);
                while(c != EOF){
                    if(c == '*'){
                        c = fgetc(src);
                        if(c == '/') { // end of the multiline comment
                            return true;
                        }
                    }else{
                        c = fgetc(src);
                    }
                }
                return true; // multiline comment without ending
            }
            else{ // not a comment
                fseek(src, -1, SEEK_CUR); // undoing the last read operation with the src file pointer postion
                // it's not necessary when we find a comment as we want them to be skipped
                // and it's not ncessary when there's no chance of a comment as we do not do any read operation with the pointer.
            }
        }
    }
    return false;
}

int is_in_char(char c, int curr_state)
{
    if(curr_state == 0 && c == '\''){
        curr_state = 1;
    }
    else if(curr_state == 1 && c == '\\'){
        curr_state = 3;
    }
    else if(curr_state == 1 || curr_state == 3){
        curr_state = 2;
    }
    else if(curr_state == 2 && c == '\''){
        curr_state = 4; // final state
    }

    return curr_state;
}

string get_token_type(string token)
{
    if(token.front() == '"' && token.back() == '"') return "str";
    else if(token.size() > 2 && token.size() < 6 && token.front() == '\'' && token.back() == '\'') return "char";
    else if(token.size() == 1 && seperators.find(token.front()) != seperators.end()) return "sep";
    else if(token.size() == 1 && operators.find(token.front()) != operators.end()) return "op";
    else if(multi_char_ops.find(token) != multi_char_ops.end()) return "op";
    else if(token.size() == 1 && braces.find(token.front()) != seperators.end()) return "par";
    else if(keywords.find(token) != keywords.end()) return "kw";
    else {
        if(isalpha(token.front()) || token.front() == '_'){
            bool flag = true;  
            for(int i = 1; i < token.size(); ++i){
                if(! (isalnum(token.at(i)) || token.at(i) == '_')){
                    flag = false;
                    break;
                }
            }
            if(flag) return "id";
        }
        else if(isdigit(token.front()) || token.front() == '.'){
            bool flag = true;  
            
            int dot_count = (token.front() == '.'? 1: 0);

            for(int i = 1; i < token.size() && dot_count < 2 && flag; ++i){
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


list<pair<string, string> > tokenize(FILE *src, FILE *dest)
{
    // assumption 1: no comment in the input file
    // assumption 2: no whitespace other than a single space in the input file [exception: strings]
    // assumption 3: no preprocessor directives in the input file
    char c;
    int in_str = 0;
    int in_char = 0;
    string token = "", token_type = "";
    int state = 0; // 0: empty, 1: char, 2: str, 3: opt_grp, 4: op or num 5: num, 6: others
    list<pair<string, string> > tokens;

    // +=, -=, *=, /=, %=, <=, >=, !=, |=, &=, ^=, ||, &&, ->, <<, >>, <<=, >>=, ==
    

    c = fgetc(src);
    while( c != EOF ){
        int flag = 0; // 0: not complete 'word' yet, 1: complete word, type still undetermined, 2: complete word, type already determined
        bool flag2 = true; // true: read new char, false: don't

        if(!in_str  && ((in_char = is_in_char(c, in_char)) > 0)){
            if(state && state != 1){
                flag = 1;
                flag2 = false;
                in_char = 0;
            }else{
                token += c;
                state = 1;
            }

            if(in_char == 4){
                in_char = 0;
                flag = 2;
                token_type = "char";
            }
        }
        else if((in_str = is_in_str2(c, in_str, src))){
            if(state && state != 2){
                flag = 1;
                flag2 = false;
                in_str = 0;
            }else{
                token += c;
                state = 2;
            }
        
            if(in_str == 3){
                in_str = false;
                flag = 2;
                token_type = "str";
            }
        }
        else if(operators.find(c) != operators.end()){
            if(!state || state == 3 || state == 4){
                token += c;
                if(c == '.' && !state) state = 4; // this dot is the first and/or only char here
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
                flag = 1;
                if(!isspace(c)) flag2 = false;
            }
            else if (!iswspace(c)){
                token += c;
                flag = 1;
            }
        }
        else{
            if(state && state != 6){
                flag = 1;
                flag2 = false;
            }else{
                token += c;
                state = 6;
            }
        }

        if(flag == 1){
            token_type = get_token_type(token);
        }
        if(flag){
            tokens.push_back({token_type, token});
            token = "";
            state = 0;
        }

        if(flag2){
            c = fgetc(src);
        }

    }

    for(auto itr = tokens.begin(); itr != tokens.end(); ++itr){
        cout << "[ " << itr->first << " " << itr->second << " ]";
    }
    cout << "\n";

    return tokens;
}

