WRAP_CLASS("itk::VersorRigidTransform" POINTER)
  FILTER_DIMS(d3 3)
  if(d3)
    WRAP_TEMPLATE("${ITKM_D}" "${ITKT_D}")
  endif(d3)
END_WRAP_CLASS()
