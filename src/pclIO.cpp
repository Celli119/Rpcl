#include "pclIO.h"

using Rcpp::List;


pcl::PointCloud<pcl::PointXYZ>  pclRead(SEXP vb_, SEXP normals_ = Rcpp::wrap(0))
{
	//insert vertices
	if (Rf_isMatrix(vb_)) {
		Rcpp::NumericMatrix vb(vb_);
		uint32_t width = vb.ncol();
		uint32_t height = 1;

		// Allocate
		pcl::PointCloud<pcl::PointXYZ> cloud;

		// Fill in the cloud data
		cloud.width    = width;
		cloud.height   = height;
		cloud.is_dense = false;
		cloud.points.resize (cloud.width * cloud.height);

		for (size_t i = 0; i < cloud.points.size (); ++i){
			cloud.points[i].x = vb(0,i);
			cloud.points[i].y = vb(1,i);
			cloud.points[i].z = vb(2,i);
		}


		//insert vertex normals
		if (Rf_isMatrix(normals_)) {
			Rcpp::NumericMatrix normals(normals_);
			if (normals.ncol() != width) {
			  Rprintf("number of normals is not equal to number of vertices");
			}
			else {
				// Allocate
				// pcl::PointCloud normals(width, height)
				// Object for storing the normals.


				pcl::PointCloud<pcl::Normal>::VectorType normals(width, height);
				for (int i=0; i < width; i++) {
					normals.push_back(pcl::PointXYZ (normals(0,i),normals(1,i),normals(2,i)));
				}
			}
			return 1
		}
		else {
			return 0
		}
	}
	else {
		return 0
	}
}

Rcpp::List RpclToR(pcl::PointCloud<pcl::PointXYZ> cloud) {
	List out;
	Rcpp::NumericMatrix vb(4, m.vn);
	for (size_t i = 0; i < cloud.points.size (); ++i){
		vb(0,i) = cloud.points[i].x;
		vb(1,i) = cloud.points[i].y;
		vb(2,i) = cloud.points[i].z;
		}
	out["vb"] = vb;
	return out;
}