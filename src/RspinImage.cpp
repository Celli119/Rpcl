#include "RspinImage.h"

pcl::PointCloud<SpinImage>::Ptr RspinImage(PclIO &cloud, float &radiusSearch, unsigned int &imageWidth)
{
	try{

		// Object for storing the spin image for each point.
		pcl::PointCloud<SpinImage>::Ptr descriptors(new pcl::PointCloud<SpinImage>);

		// Spin image estimation object.
		pcl::SpinImageEstimation<pcl::PointXYZ, pcl::Normal, SpinImage> si;
		si.setInputCloud(cloud.getCloud());
		si.setInputNormals(cloud.getNormals());
		// Radius of the support cylinder.
		si.setRadiusSearch(radiusSearch);
		// Set the resolution of the spin image (the number of bins along one dimension).
		// Note: you must change the output histogram size to reflect this.
		si.setImageWidth(imageWidth);

		si.compute(*descriptors);
		return descriptors;
	}
	catch (std::exception& e) {
		::Rf_error( e.what());
	}
	catch (...) {
		::Rf_error("unknown exception");
	}
}

RcppExport SEXP Rpclregister(SEXP vbRef_, SEXP vbTar_, SEXP normalsRef_ , SEXP normalsTar_ , SEXP radiusSearch_, SEXP imageWidth_, SEXP maxIt_, SEXP numberOfSample_, SEXP correspondenceRandomness_, SEXP similarityThreshold_, SEXP inlierFraction_)
{
	try{
		// Declare variables
		float radiusSearch = Rcpp::as<float>(radiusSearch_);
		unsigned int imageWidth = Rcpp::as<unsigned int>(imageWidth_);
		int maxIt = Rcpp::as<int>(maxIt_);
		unsigned int numberOfSample = Rcpp::as<unsigned int>(numberOfSample_);
		int correspondenceRandomness = Rcpp::as<int>(correspondenceRandomness_);
		float similarityThreshold = Rcpp::as<float>(similarityThreshold_);
		float inlierFraction = Rcpp::as<float>(inlierFraction_);
		// Import the mesh
		PclIO cloudRef;
		cloudRef.pclRead(vbRef_, normalsRef_);
		PclIO cloudTar;
		cloudTar.pclRead(vbTar_, normalsTar_);
		pcl::PointCloud<SpinImage>::Ptr spinRef = RspinImage(cloudRef, radiusSearch, imageWidth);
		pcl::PointCloud<SpinImage>::Ptr spinTar = RspinImage(cloudTar, radiusSearch, imageWidth);
		pcl::SampleConsensusPrerejective<PointNT,PointNT,SpinImage> align;
		align.setInputSource (cloudRef.getCloud());
		align.setSourceFeatures (spinRef);
		align.setInputTarget (cloudTar.getCloud());
		align.setTargetFeatures (spinTar);
		align.setMaximumIterations (maxIt); // Number of RANSAC iterations
		align.setNumberOfSamples (numberOfSample); // Number of points to sample for generating/prerejecting a pose
		align.setCorrespondenceRandomness (correspondenceRandomness); // Number of nearest features to use
		align.setSimilarityThreshold (similarityThreshold); // Polygonal edge length similarity threshold
		// align.setMaxCorrespondenceDistance (2.5f * leaf); // Inlier threshold
		align.setInlierFraction (inlierFraction); // Required inlier fraction for accepting a pose hypothesis
		PointCloudT::Ptr cloudAligned (new PointCloudT);
		align.align (*cloudAligned);

		Eigen::Matrix4f transfo = align.getFinalTransformation();
		Rcpp::Rcout << transfo << std::endl;
		return PclIO::RpclToR(cloudAligned, cloudTar.getNormals());
	}
		catch (std::exception& e) {
		::Rf_error( e.what());
	}
		catch (...) {
		::Rf_error("unknown exception");
	}
}