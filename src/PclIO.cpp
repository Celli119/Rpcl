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
		size_t width = vb.ncol();
		size_t height = 1;
		// Allocate

		// pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

		// Fill in the cloud data
		m_cloud->width  = width;
		m_cloud->height = height;
		m_cloud->points.resize (m_cloud->width * m_cloud->height);
		for (size_t i = 0; i < m_cloud->points.size (); ++i){
			// m_cloud->push_back(pcl::PointXYZ(vb(0,i),vb(1,i),vb(2,i)));
			m_cloud->points[i].x = vb(0,i);
			m_cloud->points[i].y = vb(1,i);
			m_cloud->points[i].z = vb(2,i);
		}

		//insert vertex normals
		if (Rf_isMatrix(normals_)) {
			Rcpp::NumericMatrix normals_mesh(normals_);
			if (normals_mesh.ncol() != width) {
			  Rprintf("number of normals is not equal to number of vertices");
			}
			else {
				// Allocate
				m_normals->width  = width;
				m_normals->height = height;
				m_normals->points.resize (m_normals->width * m_normals->height);

				for (int i=0; i < width; i++) {
					// m_normals->push_back(pcl::Normal(normals_mesh(0,i),normals_mesh(1,i),normals_mesh(2,i)));
					m_normals->points[i].normal_x = normals_mesh(0,i);
					m_normals->points[i].normal_y = normals_mesh(1,i);
					m_normals->points[i].normal_z = normals_mesh(2,i);
				}
			}
		}
		else {
			Rprintf("Normals not defined");
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

Rcpp::List PclIO::RpclToR(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::PointCloud<pcl::Normal>::Ptr normals) {
	try{
		Rcpp::List out;
		Rcpp::NumericMatrix vb(4,cloud->points.size ());
		Rcpp::NumericMatrix nm(4,cloud->points.size ());

		for (size_t i = 0; i < cloud->points.size (); ++i){
			vb(0,i) = cloud->points[i].x;
			vb(1,i) = cloud->points[i].y;
			vb(2,i) = cloud->points[i].z;
			vb(3,i) = 1;
			}
		for (size_t i = 0; i < cloud->points.size (); ++i){
			nm(0,i) = normals->points[i].normal_x;
			nm(1,i) = normals->points[i].normal_y;
			nm(2,i) = normals->points[i].normal_z;
			nm(3,i) = 1;
			}
		out["vb"] = vb;
		out["normals"] = nm;
		out.attr("class") = "mesh3d";
		return out;
	}
	catch(...) {
    ::Rf_error("PointCloud to R-Data conversion failed");
	}
}
