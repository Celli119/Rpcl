#include "PclIO.h"

using Rcpp::List;


PclIO::PclIO()
{
	m_cloud = pcl::PointCloud<pcl::PointXYZ>::Ptr (new pcl::PointCloud<pcl::PointXYZ>);
	m_normals = pcl::PointCloud<pcl::Normal>::Ptr (new pcl::PointCloud<pcl::Normal>);
}

int PclIO::pclRead(SEXP vb_, SEXP normals_ = Rcpp::wrap(0))
{
	//insert vertices
	if (Rf_isMatrix(vb_)) {
		Rcpp::NumericMatrix vb(vb_);
		uint32_t const width = vb.ncol();
		uint32_t const height = 1;

		// Allocate

		// pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

		// Fill in the cloud data
		m_cloud->width  = width;
		m_cloud->height = height;
		m_cloud->points.resize (m_cloud->width * m_cloud->height);

		for (size_t i = 0; i < m_cloud->points.size (); ++i){
			m_cloud->push_back(pcl::PointXYZ(vb(0,i),vb(1,i),vb(2,i)));
			// cloud->points[i].x = vb(0,i);
			// cloud->points[i].y = vb(1,i);
			// cloud->points[i].z = vb(2,i);
		}


		//insert vertex normals
		if (Rf_isMatrix(normals_)) {
			Rcpp::NumericMatrix normals_mesh(normals_);
			if (normals_mesh.ncol() != width) {
			  Rprintf("number of normals is not equal to number of vertices");
			}
			else {
				// Allocate
				// pcl::PointCloud normals(width, height)
				// Object for storing the normals.


				// pcl::PointCloud<pcl::Normal>::VectorType normals(width, height);
				// pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
				for (int i=0; i < width; i++) {
					m_normals->push_back(pcl::Normal(normals_mesh(0,i),normals_mesh(1,i),normals_mesh(2,i)));
				}
			}
		}
		return 1;
	}
	else{
		return 0;
	}

}

pcl::PointCloud<pcl::PointXYZ>::Ptr PclIO::getCloud() const {
	return m_cloud;
}
pcl::PointCloud<pcl::Normal>::Ptr PclIO::getNormals() const {
	return m_normals;
}

Rcpp::List PclIO::RpclToR(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
	try{
		Rcpp::List out;
		Rcpp::NumericMatrix vb(4);
		for (size_t i = 0; i < cloud->points.size (); ++i){
			vb(0,i) = cloud->points[i].x;
			vb(1,i) = cloud->points[i].y;
			vb(2,i) = cloud->points[i].z;
			}
		out["vb"] = vb;
		out.attr("class") = "mesh3d";
		return out;
	}
	catch(...) {
    ::Rf_error("PointCloud to R-Data conversion failed");
	}
}
