#include "pclIO.h"
#include <Rcpp.h>


using namespace Rcpp;

using Rcpp::List;

Rcpp::List  Rexport(SEXP vb_, SEXP normals_ = Rcpp::wrap(0))
{
	cloud = pclRead(SEXP vb_, SEXP normals_);
	RpclToR(cloud)
}