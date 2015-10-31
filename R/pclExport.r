#' Test read & export
#'
#' calculate common allometric component
#'
#' @param mesh mesh to read
#'
#' @return
#' \item{vb}{original vertices of the mesh }
#' @export

pclExport <- function(mesh)
	{
		vb <- mesh$vb[1:3,,drop=FALSE]
		tmp <- .Call("Rexport",vb, PACKAGE="Rpcl")
		return(tmp)
	}