#ifndef _PCL_IO_H__
#define _PCL_IO_H__

// #include <pcl/io/pcd_io.h>

#include <RcppEigen.h>
#include <pcl/point_types.h>
#include <pcl/common/projection_matrix.h>

// pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
class PclIO
{
	public:
	PclIO();
	int pclRead(SEXP vb_, SEXP normals_);
	pcl::PointCloud<pcl::PointXYZ>::Ptr getCloud() const;
	pcl::PointCloud<pcl::Normal>::Ptr getNormals() const;

	static Rcpp::List RpclToR(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::PointCloud<pcl::Normal>::Ptr normals);

	private:
	pcl::PointCloud<pcl::PointXYZ>::Ptr m_cloud;
	pcl::PointCloud<pcl::Normal>::Ptr m_normals;

};


// int pclRead(SEXP vb_, SEXP normals_);
// Rcpp::List RpclToR(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_);

#endif
