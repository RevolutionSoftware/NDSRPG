#ifndef MACROS_H
#define MACROS_H
/* File: aux_macros.c
 * Authors: Chickendude, add
 * Description:
 * Helper macros for knowing what the static does at that point. */

/* HINT:
 * Depending on where you declare a static, it will change
 * what it does!
 *
 * 1. internal:
 * When declaring a function static, it will be internal to that file,
 * e.g you can't access from anywhere else than in that file.
 *
 * 2. local_persist:
 * When declaring a variable static inside a function makes it retain
 * it's value even in between calls.
 *
 * 3. global_variable:
 * When declaring a variable static outside any function makes it a
 * global variable. */

#define internal static /* see 1. internal */
#define local_persist static /* see 2. local_persist */
#define global_variable static /* see 3. global_variable */
#define EOF -1

#endif /* MACROS_H */
