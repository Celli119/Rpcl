#' Pose estimation through spine images
#'
#'
#' @param meshRef triangular mesh of class "mesh3d": a reference mesh.
#' @param meshTar triangular mesh of class "mesh3d": a target mesh.
#' @param radiusSearch numeric: radius of the support cylinder.
#' @param imageWidth unsigned int: resolution of the spin image (the number of bins along one dimension), cannot be changed yet.
#' @param maxIt int: number of RANSAC iterations.
#' @param numberOfSample int: number of points to sample for generating/prerejecting a pose.
#' @param correspondenceRandomness int: number of nearest features to use.
#' @param similarityThreshold numeric: polygonal edge length similarity threshold.
#' @param inlierFraction numeric: required inlier fraction for accepting a pose hypothesis.
#' @param recomputeNormals boolean: if TRUE pcl recompute normals through a kdtree search.
#' @return the registered reference mesh
#' @author François Girinon
#' @export pclRegister

pclRegister <- function(meshRef, meshTar, radiusSearch=100, imageWidth=8, maxIt=500, numberOfSample=3, correspondenceRandomness=5, similarityThreshold=0.9, inlierFraction=0.25, recomputeNormals=FALSE)
	{
		vbRef <- meshRef$vb[1:3,,drop=FALSE]
		normalsRef <- meshRef$normals[1:3,,drop=FALSE]
		vbTar <- meshTar$vb[1:3,,drop=FALSE]
		normalsTar <- meshTar$normals[1:3,,drop=FALSE]

		tmp <- .Call("Rpclregister", vbRef, vbTar, normalsRef, normalsTar, radiusSearch, imageWidth, maxIt, numberOfSample, correspondenceRandomness, similarityThreshold, inlierFraction, recomputeNormals)
		tmp$it <- meshRef$it
		tmp$it <- meshRef$it
		return(tmp)
	}