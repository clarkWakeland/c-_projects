# Turing Machine

A Turing machine as a deterministic finited automota (DFA) implementation in C++ (in case you though C++ wasn't turing complete). 

# Use

Modify a *.dat file to define the following fields: 

## Alphabet
Defines the input alphabet. If the input contains characters not in this alphabet, it is rejected.

## Tape Alphabet
Defines the set of characters that can be written on the tape as the machine is running. Together with the input alphabet it makes all the symbols that can appear on the tape.

## States
Defines the total states the machine has.

## Initial
Initial state the machine starts in

## Accept
The state the machine will stop and accept the input

## Reject
The state in which the machine will stop and reject the input

## Transitions
The rules that the machine follows. Each rule is in the format `current state -- read symbol, write symbol, move direction --> next state`. Forms logic of machine


# Informal Description
The goal of the machine is to square the number `m` of `a`’s present on the tape.
This can be accomplished with a nested loop in traditional programming,
which we must implement in the form of a turing machine. We begin by
reading all `a`’s up until the end of the tape and writing a special character `*` to
mark it (state 1). We then move all the way back to the beginning of the tape
(state 2) and then mark the first `a` we see with another special character `c` (state
3). Next, we move up until we reach our mark `*` (state 4) and write an `a` on
the first blank space after it (state 5). After this, we move back through the
tape until we reach a `c` character (`a` that was previously marked) (state 6),
and find the next `a` that has not been marked (state 3). We now have a loop.


We loop through this procedure until all `a` characters before the `*` are marked with a
`c`. When this happens, we move back through our list of `c`’s (which is the same
length as our initial string of a’s) and write `a` over them (state 8) until we reach the end
marker. After reaching the end marker, we write over the first `a` we encounter with a
blank space `w` (state 9), indicating we have iterated through it and written the input
string to tape. We now have a string of `a`’s of length `m` after our special character `*`. We
check and see if all our initial input `a`’s have been reached by seeing if the next character
is `*`(state 10). If so, the algorithm is complete, we write a `w` over the `*`, and we move
to the accept state (state 11). If not, we repeat the entire loop again (state 1) while also
treating any `w`’s we encounter before the `*` character as `a`’s. To remember the difference
between a `w` visited and an `a` visited, we mark the w’s visited with a `b` character.
The loop then proceeds in the same way, writing `m` number of `a`’s in the rightmost blank
space and marking how many times that loop has gone through with `w`’s
