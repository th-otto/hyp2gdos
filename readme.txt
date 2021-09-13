This is my attempt to reconstruct the sources of hyp2gdos
(version 1.1, from May 31, 1997, filesize 38153 bytes).

It should compile to an (almost) identical binary version, with a few
exceptions:

- in a few functions, variables are assigned to different registers.
  Those functions have been thoroughly check to do the same thing
  as the original. If you find a construct that would really
  compile to the original, please let me know ;)
  
- same hacks were used that should not be there, to produce more
  identical results. The sourcecode is marked with "XXX" at those
  places

- Some variables functions could not be figured out, because
  they are nowhere referenced


Some other notes about the sources:

- the vdi.s and tos.s are only there because i normally use my
  own version of the libraries, which are different. If you are
  using the original pctoslib/pcgemlib, they are not needed
  and can be commented out in the project file

- the vq_scrninfo() function relies on the VDIParBlk structure
  of the original PCGEMLIB. It should be replaced when using
  your own library.

- There are several places where the length byte of names/strings
  is sign-extended. This will restrict nodenames etc. to be no
  longer than 127 bytes or the program will crash or hang in an
  endless loop.

