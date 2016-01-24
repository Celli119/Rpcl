#include "RspinImage.h"

pcl::PointCloud<SpinImage>::Ptr RspinImage(PclIO cloud)
{
	try{

		// Object for storing the spin image for each point.
		pcl::PointCloud<SpinImage>::Ptr descriptors(new pcl::PointCloud<SpinImage>);

		// Spin image estimation object.
		pcl::SpinImageEstimation<pcl::PointXYZ, pcl::Normal, SpinImage> si;
		si.setInputCloud(cloud.getCloud());
		si.setInputNormals(cloud.getNormals());
		// Radius of the support cylinder.
		si.setRadiusSearch(20);
		// Set the resolution of the spin image (the number of bins along one dimension).
		// Note: you must change the output histogram size to reflect this.
		si.setImageWidth(8);

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

RcppExport SEXP Rpclregister(SEXP vbRef_,  SEXP vbTar_, SEXP normalsRef_ = Rcpp::wrap(0), SEXP normalsTar_ = Rcpp::wrap(0))
{
	// Import the mesh
	PclIO cloudRef;
	cloudRef.pclRead(vbRef_, normalsRef_);
	PclIO cloudTar;
	cloudTar.pclRead(vbTar_, normalsRef_);
	pcl::PointCloud<SpinImage>::Ptr spinRef = RspinImage(cloudRef);
	pcl::PointCloud<SpinImage>::Ptr spinTar = RspinImage(cloudTar);
	pcl::SampleConsensusPrerejective<PointNT,PointNT,SpinImage> align;
	align.setInputSource (cloudRef.getCloud());
	align.setSourceFeatures (spinRef);
	align.setInputTarget (cloudTar.getCloud());
	align.setTargetFeatures (spinTar);
	align.setMaximumIterations (50000); // Number of RANSAC iterations
	align.setNumberOfSamples (5); // Number of points to sample for generating/prerejecting a pose
	align.setCorrespondenceRandomness (20); // Number of nearest features to use
	align.setSimilarityThreshold (0.9f); // Polygonal edge length similarity threshold
	// align.setMaxCorrespondenceDistance (2.5f * leaf); // Inlier threshold
	align.setInlierFraction (0.25f); // Required inlier fraction for accepting a pose hypothesis
	PointCloudT::Ptr cloudAligned (new PointCloudT);
	align.align (*cloudAligned);
	return PclIO::RpclToR(cloudAligned, cloudTar.getNormals());
}
// pcl::KdTreeFLANN<pcl::Histogram<153> > kdTree;
// 	kdTree.setInputCloud(modelFeaturesRef.siFeatures.makeShared());

// 	//compare scene descriptor to all model descriptors and find the nearest one
// 	for (int i = 0; i < sceneRef.siFeatures.size(); i++)
// 	{
// 		std::vector<int> neighborIndices(1);
// 		std::vector<float> neighborSqrDistance(1);

// 		int foundNeighbor = kdTree.nearestKSearch(sceneRef.siFeatures.at(i), 1, neighborIndices, neighborSqrDistance);

// 		//check if neighbor found and if it is it below the distance threshold
// 		if (1 == foundNeighbor && distanceThreshold > neighborSqrDistance[0])
// 		{
// 			pcl::Correspondence correspondence(modelFeaturesRef.modelIndices[neighborIndices[0]], i, neighborSqrDistance[0]);
// 			std::map<std::string, Model>::iterator it = modelsRef.find(modelFeaturesRef.modelNames[neighborIndices[0]]);
// 			it->second.correspondences->push_back(correspondence);
// 		}
// 	}
