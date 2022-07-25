/*
Copyright (c) 2022, Felipe Miguel Nery Lunkes
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <math.h>
#include <errno.h>
#include <float.h>

static const double pi   = 3.1415926535897932384626433832795;
static const double ln10 = 2.3025850929940456840179914546844;
static const double ln2 = 0.69314718055994530941723212145818 ;


 double ceil(double x)
{
    int y;

    y = (int)x;

    if ((double)y < x)
    {

        y++;
    
    }
    
    return ((double)y);

}

 double fabs(double x)
{
    
    if (x < 0.0)
    {

        x = -x;

    }

    return (x);

}

 double floor(double x)
{

    int y;

    if (x < 0.0)
    {

        y = (int)x;

        if ((double)y != x)
        {

            y--;

        }

    }

    else
    {
    
        y = (int)x;
    
    }
    
    return ((double)y);

}

 double fmod(double x, double y)
{
    
    int imod;

    if(y == 0.0) return (0.0);
  
    imod = x / y;
  
    return ((double)x-((double)imod*y));

}

/*

  For cos just use (sin(x)**2 + cos(x)**2)=1
  Note:- asin(x) decides which taylor series
  to use to ensure quickest convergence.
*/

 double acos(double x)
{

/*
*/

    if ( fabs(x) > 1.0 ) /* is argument out of range */
    {

        errno=EDOM;

        return (HUGE_VAL);

    }

    if ( x < 0.0 ) return ( pi - acos(-x) ) ;

    return ( asin ( sqrt(1.0 - x*x) ) );

}

/*

   This routines Calculate arcsin(x) & arccos(x).

   Note if "x" is close to "1" the series converges slowly.
   To avoid this we use (sin(x)**2 + cos(x)**2)=1
   and fact cos(x)=sin(x+pi/2)

*/

 double asin (double y)
{

    int i;
    double term,answer,work,x,powx,coef;

    x = y;

/*
  if arg is -ve then we want "-asin(-x)"
*/

    if (x <0.0 ) return ( -asin(-x) );

/*
    If arg is > 1.0 we can't calculate
    (note also < -1.0 but previous statement removes this case)
*/
    if ( x > 1.0 )
    {

        errno=EDOM;

        return(HUGE_VAL);

    }

/*
 now check for large(ish) x > 0.6
*/

    if( x > 0.75 )
    {

        x = ( sqrt(1.0 - (x*x) ) );

        return((pi/2.0)-asin(x));

    }

/*
     arcsin(x) = x + 1/2 (x^3/3) + (1/2)(3/4)(x^5/5) +
        (1/2)(3/4)(5/6)(x^7/7) + ...
*/
    i=1;
    answer=x;
    term = 1;
    coef = 1;
    powx = x;

    while (1)
    {

        work = i;
        coef = (coef * work)/(work+1);
        powx = powx * x * x;
        term =  coef * powx / (work + 2.0);

        if ( answer == (answer + term) )break;

        answer = answer + (term);

        i+=2;

    }

    return(answer);

}

/*

     Because atan(x) is valid for large values of "x" &
     the taylor series converges more slowly for large "X"
     we use the following

     1. Reduce to the first octant by using :-

        atan(-x)=-atan(x),
        atan(1/x)=PI/2-atan(x)

     2. Reduce further so that |x| less than tan(PI/12)

        atan(x)=pi/6+atan((X*sqrt(3)-1)/(x+sqrt(3)))

     3. Then use the taylor series

        atan(x) = x - x**3 + x**5 - x**7
                      ----   ----   ----
                        3      5      7

*/

 double atan (double x)
{

    int i;

    double term,answer,work,powx;

/*
  if arg is -ve then we want "-atan(-x)"
*/

    if ( x<0.0 ) return ( -atan(-x) );

/*
 If arg is large we can't calculate
 use atan(1/x)=PI/2-atan(x)
*/

    if ( x > 1.0 ) return ((pi/2) - atan(1.0/x));

/*
 now check for large(ish) x > tan(15) (0.26794919243112)
 if so use atan(x)=pi/6+atan((X*SQRT3-1)/(X+SQRT3))
*/

    if( x > (2.0 - sqrt(3.0)))
    return( (pi/6.0) + atan( ( x * sqrt(3.0)-1.0 ) / (x + sqrt(3.0) ) ) );

/*
*       atan(x) = x - x**3 + x**5 - x**7
*                     ----   ----   ----
*                       3      5      7
*/

    i=1;
    answer=x;
    term = x;
    powx = x;

    while (1)
    {

        work = i;
        powx = 0.0 - powx * x * x;
        term = powx / (work + 2.0);

        if ( answer == (answer + term) )break;

        answer = answer + (term);

        i+=2;
    }

    return(answer);

}


/* atan2 was taken from libnix and modified slightly */

 double atan2(double y,double x)
{

    return (x >= y) ?
               (x >= -y ? atan(y/x) : -pi/2 - atan(x/y))
              :
               (x >= -y ? pi/2 - atan(x/y)
                        : (y >= 0) ? pi + atan(y/x)
                                   : -pi + atan(y/x));

}

 double cos(double x)
{

/*

   Calculate COS using Taylor series.

   sin(x) = 1 - x**2  +  x**4  - x**6 + x**8
                ====     ====    ====   ====    .........
                  2!       4!      6!     8!

   Note whilst this is accurate it can be slow for large
   values of "X" so we scale

*/

    int i;
    double term,answer,work,x1;

/*
    Scale arguments to be in range 1 => pi
*/

    i = x/(2*pi);
    x1 =  x - (i * (2.0 * pi));

    i=1;
    term=answer=1;


    while (1)
    {

        work = i;
        term = -(term * x1 * x1)/(work * (work + 1.0));

        if ( answer == (answer + term) )break;

        answer = answer + term;
        i += 2;

    }

    return(answer);


}

 double sin(double x)
{

/*

   Calculate SIN using Taylor series.

   sin(x) = x - x**3  +  x**5  - x**7 + x**9
                ====     ====    ====   ====
                  3!       5!      7!     9!

   Note whilst this is accurate it can be slow for large values
   of "X" so we scale

*/

    int i;
    double term,answer,work,x1;

/*
  scale so series converges pretty quickly
*/
    i = x/(2.0*pi);
    x1 =  x - (i * (2.0 * pi));

/*
 set up initial term
*/
    i=1;
    term=answer=x1;
/*
 loop until no more changes
*/
    while (1)
    {

        work = i+1;
        term = -(term * x1 * x1)/(work * (work + 1.0));

        if ( answer == (answer + term) )break;

        answer = answer + term;
        i = i+2;
    }

    return(answer);

}

 double tan (double x)
{

/*

  use tan = sin(x)/cos(x)
  if cos(x) is 0 then return HUGE_VAL else return sin/cos

  *** need to set ERROR for overflow ***

*/
    double temp;

    temp=cos(x);
    if (temp == 0.0 )
    {

        /* errno=EDOM; don't seem to return an error here */

        return (HUGE_VAL); /* need to set error here */
    }

    return ( sin(x)/cos(x) );
}

/*

  Hyperbolic functions

  SINH(X) = (E**X-E**(-1))/2
  COSH(X) = (E**X+E**(-1))/2

*/
 double cosh(double x)
{

    double dexpx;

    dexpx = exp(x);

    return( 0.5 * (dexpx + (1.0/dexpx) ) );

}

 double sinh(double x)
{

    double dexpx;

    dexpx = exp(x);

    return( 0.5 * (dexpx - (1.0/dexpx) ) );

}

/*
    tanh returns the hyperbolic area tangent of floating point argument x.
*/

 double tanh(double x)
{

    double dexp2;

    dexp2 = exp( -2.0 * x);

    return ( (1.0  - dexp2) /  (1.0 + dexp2) );

}

/*

exp(x) = 1 + x + x2/2 + x3/6 + x4/24 + x5/120 + ... + xn/n! + ...

*/
 double exp (double x)
{

    int i;
    double term,answer,work;

    i=2;
    term=x;
    answer=x;

    while (1)
    {

        work = i;
        term =  (term * x)/work;

        if ( answer == (answer + term) )break;

        answer = answer + (term);
        i++;

    }

    answer=answer+1.0;

    return(answer);
}

/*

   Calculate LOG using Taylor series.

   log(1+ x) = x - x**2  +  x**3  - x**4 + x**5
                   ====     ====    ====   ====    .........
                    2         3       4     8

   Note this only works for small x so we scale....

*/
 double log (double x)
{

    int i,scale;
    double term,answer,work,xs;

    if (x <= 0 )
    {
        /* need to set signal */
        errno=EDOM;
        return (HUGE_VAL);

    }

    if( x == 1.0)return(0.0);

/*
  Scale arguments to be in range 1 < x <= 10
*/

/*
    scale = 0;
    xs = x;
    while ( xs > 10.0 ) { scale ++; xs=xs/10.0;}
    while ( xs < 1.0 ) { scale --; xs=xs*10.0;}
*/
    xs = frexp(x,&scale);
    xs = (1.0 * xs) - 1.0;
    scale = scale - 0;

    i=2;
    term=answer=xs;

    while (1)
    {

        work = i;
        term = - (term * xs);

        if ( answer == (answer + (term/work)) )break;

        answer = answer + (term/work);
        i++;

    }

    answer = answer + (double)scale * ln2;

    return(answer);
}


 double log10(double x)
{

    return ( log(x) / ln10 );

}

/*

   This code uses log and exp to calculate x to the power y.
   If

*/

 double pow(double x,double y)
{

    int j,neg;
    double yy,xx;
    neg=0;
    j=y;
    yy=j;

    if( yy == y) {

        xx = x;

        if ( y < 0 ){neg = 1; j = -j;}

        if ( y == 0) return (1.0);

        --j;

        while(j>0){ xx=xx * x; j--;}

        if(neg)xx=1.0/xx;

        return (xx);

    }
    if (x < 0.0)
    {

         errno=EDOM;
         return(0.0);

    }

    if (y == 0.0) return (1.0);

    return (exp(y*log(x)));

}

/*

   pretty tivial code here.

     1) Scale x such that 1 <= x <= 4.0

     2) Use newton Raphson to calculate root.

     4) multiply back up.

   Because we only scale by "4" this is pretty slow....

*/

 double sqrt(double x)
{

    double xs,yn,ynn;
    double pow1;
    int i;

    if (x < 0.0)
    {

        errno=EDOM;

        return(0.0);

    }

    if (x == 0.0) return (0.0);

/*

  Scale argument 1 <= x <= 4

*/

    xs=x;
    pow1=1;

    while(xs<1.0){xs=xs*4.0;pow1=pow1/2.0;}
    while(xs>=4.0){xs=xs/4.0;pow1=pow1*2.0;}

/*
  calculate using Newton raphson
  use x0 = x/2.0
*/

    i=0;
    yn = xs/2.0;
    ynn = 0;

    while(1)
    {

        ynn = (yn + xs/yn)*0.5;

        if ( fabs(ynn-yn) <= 10.0 * DBL_MIN ) break; else yn=ynn;

        if ( i > 10  ) break; else i++ ;

    }

    return (ynn*pow1);

}


 double frexp(double x, int *exp)
{
/*
  split float into fraction and mantissa
  note this is not so easy for IBM as it uses HEX float
*/
    union dblhex
    {

        double d;
        unsigned short s[4];

    };

    union dblhex split;

    if ( x == 0.0)
    {

        exp=0;

        return (0.0);

    }

    split.d = x;
    *exp = (((split.s[0] >> 8) & 0x007f)-64) * 4;
    split.s[0] = split.s[0] & 0x80ff;
    split.s[0] = split.s[0] | 0x4000;

    /* following code adjust for fact IBM has hex float */

    while ( (fabs(split.d) < 0.5) && (split.d != 0) )
    {

        split.d = split.d * 2;
        *exp =( *exp ) - 1;

    }
    
    return(split.d);

}

 double ldexp(double x, int exp)
{

/*
  note this is not so easy for IBM as it uses HEX float
*/

    int bin_exp,hex_exp,adj_exp;
    union dblhex
    {

        double d;
        unsigned short s[4];

    };

    union dblhex split;
/*
    note "X" mauy already have an exponent => extract it
*/

    split.d = frexp(x,&bin_exp);
    bin_exp = bin_exp + exp;  /* add in from caller */

/* need to test for sensible value here */

    hex_exp =  (bin_exp / 4); /* convert back to HEX */
    adj_exp =  bin_exp - (hex_exp * 4);

    if (adj_exp < 0){ hex_exp=hex_exp -1; adj_exp = 4 + adj_exp;}
   
    split.s[0] = split.s[0] & 0x80ff;
    split.s[0] = split.s[0] | (((hex_exp+64)  << 8) & 0x7f00);
   
    /* following code adjust for fact IBM has hex float */
    /* well it will I have done */
    
    while ( adj_exp > 0 )
    {

        split.d = split.d * 2;
        --adj_exp;

    }
 
    return(split.d);

}

 double modf(double value, double *iptr)
{

    int neg = 0;
    long i;

    if (value < 0)
    {

        neg = 1;
        value = -value;

    }

    i = (long)value;

    value = value - i;
    if (neg)
    {

        value = -value;
        i = -i;

    }

    *iptr = i;

    return (value);
    
}
