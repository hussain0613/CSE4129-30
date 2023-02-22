# Write a program to print the header files used in a source program.
import re

with open("s1le1_input.txt") as f:
    inp = f.read()
    x = re.findall("#include( *)[<\"](.+)[>\"]", inp)

for tp in x:
    print(tp[1], end=", ")