; file ps.as
.entry LIST
.extern W
ARR: .data 5,3
MAIN: add r3, LIST
TEST: lea MAIN,   r7
LOOP: prn #48
STR: .string "abcd"
 lea STR, r6
    LIST: .data 6, -9