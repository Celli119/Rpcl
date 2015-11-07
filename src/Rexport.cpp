#include "Rexport.h"


// using namespace Rcpp;

// using Rcpp::List;

RcppExport SEXP Rexport(SEXP vb_, SEXP normals_ = Rcpp::wrap(0))
{
	try{
		PclIO cloud;
		cloud.pclRead(vb_, normals_);
		return PclIO::RpclToR(cloud.getCloud(), cloud.getNormals());
	}
	catch (std::exception& e) {
		::Rf_error( e.what());
	}
	catch (...) {
		::Rf_error("unknown exception");
	}
}