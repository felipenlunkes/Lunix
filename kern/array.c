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

#include <Lunix/kernel/array.h>
#include <Lunix/kernel/kernel.h>

s8int standard_lessthan_predicate(type_t a, type_t b)
{

    return (a<b)?1:0;

}

ordered_array_t create_ordered_array(u32int max_size, lessthan_predicate_t less_than)
{

    ordered_array_t to_ret;
    to_ret.array = (void*)kmalloc(max_size*sizeof(type_t));

    memset(to_ret.array, 0, max_size*sizeof(type_t));

    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;

    return to_ret;

}

ordered_array_t place_ordered_array(void *addr, u32int max_size, lessthan_predicate_t less_than)
{

    ordered_array_t to_ret;
    to_ret.array = (type_t*)addr;

    memset(to_ret.array, 0, max_size*sizeof(type_t));

    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;

    return to_ret;

}

void destroy_ordered_array(ordered_array_t *array)
{

//    kfree(array->array);

}

void insert_ordered_array(type_t item, ordered_array_t *array)
{

    ASSERT(array->less_than);

    u32int iterator = 0;

    while (iterator < array->size && array->less_than(array->array[iterator], item))
        iterator++;

    if (iterator == array->size) // just add at the end of the array.
      
        array->array[array->size++] = item;

    else
    {

        type_t tmp = array->array[iterator];
        array->array[iterator] = item;

        while (iterator < array->size)
        {

            iterator++;
            type_t tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;

        }

        array->size++;

    }
}

type_t lookup_ordered_array(u32int i, ordered_array_t *array)
{

    ASSERT(i < array->size);

    return array->array[i];

}

void remove_ordered_array(u32int i, ordered_array_t *array)
{

    while (i < array->size)
    {

        array->array[i] = array->array[i+1];
        i++;

    }

    array->size--;
    
}
