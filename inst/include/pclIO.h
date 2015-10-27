#ifndef _PCL_IO_H__
#define _PCL_IO_H__

// #include <pcl/io/pcd_io.h>

#include <RcppEigen.h>
#include <pcl/point_types.h>

pcl::PointCloud<pcl::PointXYZ>  pclRead(SEXP vb);
Rcpp::List RpclToR(pcl::PointCloud<pcl::PointXYZ> cloud);