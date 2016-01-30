#ifndef _RSPIN_IMAGE_H__
#define _RSPIN_IMAGE_H__

#include "PclIO.h"
#include <Rcpp.h>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/features/spin_image.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/impl/kdtree_flann.hpp>

#include <Eigen/Core>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/time.h>
#include <pcl/console/print.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/features/fpfh_omp.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>
#include <pcl/registration/icp.h>
#include <pcl/registration/sample_consensus_prerejective.h>
// Types
typedef pcl::PointXYZ PointNT;
typedef pcl::PointCloud<PointNT> PointCloudT;
typedef pcl::FPFHSignature33 FeatureT;
typedef pcl::FPFHEstimationOMP<PointNT,PointNT,FeatureT> FeatureEstimationT;
typedef pcl::PointCloud<FeatureT> FeatureCloudT;
typedef pcl::Histogram<153> SpinImage;
// typedef pcl::visualization::PointCloudColorHandlerCustom<PointNT> ColorHandlerT;

pcl::PointCloud<SpinImage>::Ptr RspinImage(PclIO &cloud, float &radiusSearch, unsigned int &imageWidth);
RcppExport SEXP Rpclregister(SEXP vbRef_, SEXP vbTar_, SEXP normalsRef_ , SEXP normalsTar_ , SEXP radiusSearch, SEXP imageWidth, SEXP maxIt, SEXP numberOfSample, SEXP correspondenceRandomness, SEXP similarityThreshold, SEXP inlierFraction);
#endif