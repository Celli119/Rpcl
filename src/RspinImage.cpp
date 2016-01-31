#include "RspinImage.h"

pcl::PointCloud<SpinImage>::Ptr RspinImage(PclIO &cloud, float &radiusSearch, unsigned int &imageWidth, bool recomputeNormals)
{
	try{

		// Object for storing the spin image for each point.
		pcl::PointCloud<SpinImage>::Ptr descriptors(new pcl::PointCloud<SpinImage>);

		// Spin image estimation object.
		pcl::SpinImageEstimation<pcl::PointXYZ, pcl::Normal, SpinImage> si;
		si.setInputCloud(cloud.getCloud());
		// Compute the normals
		if (recomputeNormals)
		{
			pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
			ne.setInputCloud (cloud.getCloud());

			// Create an empty kdtree representation, and pass it to the normal estimation object.
			// Its content will be filled inside the object, based on the given input dataset (as no other search surface is given).
			pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
			ne.setSearchMethod (tree);

			// Output datasets
			pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);

			// Use all neighbors in a sphere of radius 3cm
			ne.setRadiusSearch (0.03);

			// Compute the features
			ne.compute (*cloud_normals);
			si.setInputNormals(cloud_normals);
		}
		else si.setInputNormals(cloud.getNormals());
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

RcppExport SEXP Rpclregister(SEXP vbRef_, SEXP vbTar_, SEXP normalsRef_ , SEXP normalsTar_ , SEXP radiusSearch_, SEXP imageWidth_, SEXP maxIt_, SEXP numberOfSample_, SEXP correspondenceRandomness_, SEXP similarityThreshold_, SEXP inlierFraction_, SEXP recomputeNormals_)
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
		bool recomputeNormals = Rcpp::as<bool>(recomputeNormals_);
		// Import the mesh
		PclIO cloudRef;
		cloudRef.pclRead(vbRef_, normalsRef_);
		PclIO cloudTar;
		cloudTar.pclRead(vbTar_, normalsTar_);
		pcl::PointCloud<SpinImage>::Ptr spinRef = RspinImage(cloudRef, radiusSearch, imageWidth, recomputeNormals);
		pcl::PointCloud<SpinImage>::Ptr spinTar = RspinImage(cloudTar, radiusSearch, imageWidth, recomputeNormals);
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