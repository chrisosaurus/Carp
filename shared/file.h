#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

/* test for something POSIX-ey */
#if defined(__APPLE__) || defined(__linux__)

#include <glob.h>

#endif

Array * carp_file_glob(char *pattern) {
  glob_t globbuf;
  /*
   * flags:
   * GLOB_MARK - append '/' to each path which is a dir
   *
   * errfunc of NULL
   *
   */
  int ret = glob(pattern, GLOB_MARK, 0, &globbuf);

  switch(ret){
    case 0:
      /* success */
      break;

    case GLOB_NOMATCH:
      /* no matches found */
      /* treat as success */
      break;

    case GLOB_NOSPACE:
      /* ran out of space */
      /* FIXME TODO report error */
      return 0;
      break;

    case GLOB_ABORTED:
      /* aborted, read error */
      /* FIXME TODO report error */
      return 0;
      break;

    default:
      /* impossible */
      /* FIXME TODO report error */
      return 0;
      break;
  }

  Array *array = 0;
  /* FIXME check success */
  /* FIXME check all malloc calls throughout */
  array = malloc(sizeof(Array));
  array->count = globbuf.gl_pathc;
  char **char_array = malloc( array->count * sizeof(char*) );
  array->data  = char_array;

  /* go through results in globbuf */
  size_t i = 0;
  for( i=0; i<globbuf.gl_pathc; ++i ){
    /* collect results */
    char *result = globbuf.gl_pathv[i];
    /* duplicate string */
    char *dup = strdup(result);
    /* store string in array */
    char_array[i] = dup;
  }

  globfree(&globbuf);
  return array;
}

