pclExport <- function(mesh)
    {
        vb <- mesh$vb[1:3,,drop=FALSE]
        tmp <- .Call("Rexport",vb)
        return(tmp)
    }