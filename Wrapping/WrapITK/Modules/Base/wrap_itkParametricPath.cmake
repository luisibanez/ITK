WRAP_CLASS("itk::ParametricPath" POINTER)
  foreach(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${d}" "${d}")
  endforeach(d)
END_WRAP_CLASS()
