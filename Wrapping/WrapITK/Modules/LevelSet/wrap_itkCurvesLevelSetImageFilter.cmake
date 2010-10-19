WRAP_CLASS("itk::CurvesLevelSetImageFilter" POINTER)
  FILTER_DIMS(dims 2+)
  foreach(d ${dims})
    foreach(t ${WRAP_ITK_REAL})
      WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_I${t}${d}}${ITKM_${t}}" "${ITKT_I${t}${d}},${ITKT_I${t}${d}},${ITKT_${t}}")
    endforeach(t)
  endforeach(d)
END_WRAP_CLASS()
