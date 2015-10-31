#ifndef _REXPORT_H__
#define _REXPORT_H__

#include "PclIO.h"
#include <Rcpp.h>

Rcpp::List  Rexport(SEXP vb_, SEXP normals_);

#endif