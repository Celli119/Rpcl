#' Test read & export
#'
#'
#' @param mesh mesh to read
#'
#' @return
#' \item{vb}{original vertices of the mesh }
#' @export

pclRegister <- function(meshRef, meshTar)
	{
		vbRef <- meshRef$vb[1:3,,drop=FALSE]
		normalsRef <- meshRef$normals[1:3,,drop=FALSE]
		vbTar <- meshTar$vb[1:3,,drop=FALSE]
		normalsTar <- meshTar$normals[1:3,,drop=FALSE]
		tmp <- .Call("Rpclregister", vbRef, normalsRef, vbTar, normalsTar)
		tmp$it <- meshTar$it
		return(tmp)
	}