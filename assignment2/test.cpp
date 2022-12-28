#include<bits/stdc++.h>

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
        curr_state = 0;
    }else{
        printf("inside: %c, %d\n", c, curr_state);
        curr_state = -1; // error
    }

    return curr_state;
}


int main()
{
    int var = 10;
    float flt = 10.;
    // var = var << 1;
    var = ~var;
    printf("%d\n", var);

    printf("%d\n", is_in_char('\'', 0));

    printf("%d\n", isalnum(' '));

    printf("%d\n", isdigit('9'));
    
    return 0;
}