# LILI
F**L**uid-K**I**netic P**L**asma S**I**mulator

## Key features
* Multiscale capabilities through implementation of MHD, two fluids, hybrid, and PIC
* Modular and easy to use
* Vectorized
* Modern and optimized input/output
* Non-blocking calculations
* Data layout that can be easily transformed into a GPU code

## Error codes
### Initialization
* `10`: Number of simulation system is not a positive integer
* `11`: Unrecognized simulation system
* `12`: Multiple MHD system

## Idea notes
1. Use array of function pointer for k-stage evolutions for all system variable
     * Make sure at the end of every stage there is MPI wait or something similar.
     * At each, stage the integrator can have multiple substage depending on order of accuracy.
     * Easy to change with function pointers. Need to probably define a variable.
     * Have composite function for example in the case of expanding box transition.
     * This way field can also just be mesh variable.
     * Issue might arise on how to keep the variable cache in place.
       * Try to rearrange process call, i.e. `(v1_1, v2_1, v3_1, v3_2, v2_2[null], v1_2[null], v3_3, v1_3, v2_3, v2_4, ...)`
       * Default option might just be cyclic with `(v1_1, ..., vn_1, vn_2, ..., v1_2, v1_3, ..., vn_3, vn_4, ...)`
2. 

## TODO
### Docs
1. Add `exhale` for automatically moving from `Doxygen` to `breathe`