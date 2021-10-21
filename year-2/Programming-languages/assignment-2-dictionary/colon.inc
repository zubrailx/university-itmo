%macro colon 2

%ifid %2
    %ifdef LAST_LABEL
        %2: dq LAST_LABEL
    %else
        %2: dq 0
    %endif

    %define LAST_LABEL %2
%else
    %fatal "Strings and numbers are not supported as labels"
%endif 

%ifstr %1
    %%key_string:   db   %1, 0
%else
    %error "Numbers and identifiers are not supported as keys"
%endif

%endmacro

