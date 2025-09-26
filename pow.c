/*  Estimate large exponentiation operations and print it in scientic notation
    Copyright (C) 2025 Anonymous1212144

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const unsigned int SPLIT = LDBL_MANT_DIG / 2;

int add(long double *const a, const long double b, int decimal)
{
    // Add 2 numbers
    // If decimal flag is set, only write the fractional part and return the integral part
    if (decimal)
    {
        long double c = (b - 1) + *a;
        if (c >= 0)
        {
            *a = c;
            return 1;
        }
    }
    *a += b;
    return 0;
}

long double raw_mult(const long double ai, const long double ad, const long double bi, const long double bd, long double *const i)
{
    // Perfect precise multiplication, returns the lower long doubles and writes the upper one to pointer "i"
    // Only precise when the arguments use half the mantissa or less
    long double o1 = ai * bi;
    long double o2 = ad * bd;

    long double c = ai * bd;
    long double ci, cd;
    cd = modfl(c, &ci);
    o1 += ci;
    o1 += add(&o2, cd, 1);

    c = ad * bi;
    cd = modfl(c, &ci);
    o1 += ci;
    o1 += add(&o2, cd, 1);
    *i = o1;
    return o2;
}

long double merge_sum(const long double a1, const long double a2, const long double b1, const long double b2, long double *const i)
{
    // Sum from smallest to biggest for 4 numbers where a1<a2 and b1<b2
    // If decimal flag is set, only write the fractional part and return the integral part
    // Only precise whan all numbers are positive
    long double sum;
    int overflow = 0;
    int decimal = i ? 1 : 0;
    if (a1 < b1)
    {
        sum = a1;
        if (a2 < b1)
        {
            overflow += add(&sum, a2, decimal);
            overflow += add(&sum, b1, decimal);
            overflow += add(&sum, b2, decimal);
        }
        else
        {
            overflow += add(&sum, b1, decimal);
            if (a2 < b2)
            {
                overflow += add(&sum, a2, decimal);
                overflow += add(&sum, b2, decimal);
            }
            else
            {
                overflow += add(&sum, b2, decimal);
                overflow += add(&sum, a2, decimal);
            }
        }
    }
    else
    {
        sum = b1;
        if (a1 < b2)
        {
            overflow += add(&sum, a1, decimal);
            if (a2 < b2)
            {
                overflow += add(&sum, a2, decimal);
                overflow += add(&sum, b2, decimal);
            }
            else
            {
                overflow += add(&sum, b2, decimal);
                overflow += add(&sum, a2, decimal);
            }
        }
        else
        {
            overflow += add(&sum, b2, decimal);
            overflow += add(&sum, a1, decimal);
            overflow += add(&sum, a2, decimal);
        }
    }
    if (i)
        *i += overflow;
    return sum;
}

long double mult(const long double a, const long double b, const long double c, long double *const i)
{
    // Calcualtes a(b+c)
    // The result should be as close as reasonably feasible
    // Returns the fractional part and writes the integral part to pointer "i"
    int ae, be, ce;
    long double ai, ad, bi, bd, ci, cd;
    ad = modfl(ldexpl(frexpl(a, &ae), SPLIT), &ai);
    bd = modfl(ldexpl(frexpl(b, &be), SPLIT), &bi);
    cd = modfl(ldexpl(frexpl(c, &ce), SPLIT), &ci);
    int e1 = ae + be - SPLIT * 2;
    int e2 = ae + ce - SPLIT * 2;

    long double o1, o2, o3, o4;
    o2 = raw_mult(ai, ad, bi, bd, &o1);
    o4 = raw_mult(ai, ad, ci, cd, &o3);

    long double p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    if (e1)
    {
        if (e1 > 0)
        {
            o1 = ldexpl(o1, e1);
            o2 = modfl(ldexpl(o2, e1), &p2);
        }
        else
        {
            p1 = modfl(ldexpl(o1, e1), &o1);
            o2 = ldexpl(o2, e1);
        }
    }
    if (e2)
    {
        if (e2 > 0)
        {
            o3 = ldexpl(o3, e2);
            o4 = modfl(ldexpl(o4, e2), &p4);
        }
        else
        {
            p3 = modfl(ldexpl(o3, e2), &o3);
            o4 = ldexpl(o4, e2);
        }
    }

    *i = merge_sum(p2, o1, p4, o3, NULL);
    long double out = merge_sum(o2, p1, o4, p3, i);
    return out;
}

int main(const int argc, const char *const argv[])
{
    int precision;
    if (argc > 1)
        precision = atoi(argv[1]);
    else
        precision = 12;
    printf("Precision is set to %d decimal places\n", precision);

    char input[LDBL_MAX_EXP];
    printf("Enter base: ");
    fgets(input, LDBL_MAX_EXP, stdin);
    long double a = strtold(input, NULL);
    printf("Base read as:\n%.*Lf\n", precision, a);
    if (a <= 0)
    {
        printf("Error: invalid base\n");
        exit(EXIT_FAILURE);
    }

    printf("\nEnter exponent: ");
    fgets(input, LDBL_MAX_EXP, stdin);
    long double b = strtold(input, NULL);
    printf("Exponent read as:\n%.*Lf\n", precision, b);
    int invert = b < 0;
    if (invert)
        b *= -1;

    long double width = floorl(log10l(a));
    long double lad = log10l(a / powl(10, width));
    long double i, d;
    d = mult(b, width, lad, &i);
    if (invert)
    {
        i = -i - 1;
        d = 1 - d;
    }
    d = powl(10, d);
    printf("\nResult:\n%.*Lf E %.0Lf\n", precision, d, i);
}