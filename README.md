# Large-Exponents
Estimates very large exponentiation operations

# Motivation
Suppose after you died, you get to play a fun game in the afterlife where every person that has ever lived on Earth are all brought together and each of them think of a random atom in the universe, and you have to guess which atom it is. What is the chance of you getting them all correct? What if the number of people is an integer multiple of π and the number of atoms is an integer multiple of φ?

You might have the preposterous thought that this is not a common occurance, but someday you will need to calculate 6207145307999.2998046875<sup>-2852895446.646540164947509765625</sup> and you'll be glad that you have to tool to figure out that it is approximately 1.267398993665 × 10<sup>-36496783086</sup>

Also, the chance is around 1 in 10<sup>10<sup>83</sup></sup>:
- https://ourworldindata.org/the-future-is-vast
- https://www.livescience.com/how-many-atoms-in-universe.html

# Notes
MSVC compiler does not support `long double`, so the mantissa is limited to 52 bits. If you find another compiler for this program you can get better precision without altering it (like some supports 63-bit mantissa). I estimate the output precision is around 13 digits, so by default it only prints 12 decimal places, but you can get the program to output more by doing `pow <num of decimal places>`. E.g. `pow 20` will output 20 decimal places.
