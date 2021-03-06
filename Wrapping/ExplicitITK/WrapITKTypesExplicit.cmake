#------------------------------------------------------------------------------

# define some macro to help creation of types vars

macro(WRAP_EXPLICIT_TYPE class prefix)
   # begin the creation of a type vars
   # call to this macro should be followed by one or several call to WRAP_TEMPLATE()
   # and one call to END_WRAP_EXPLICIT_TYPE to really create the vars
   set(WRAPPER_TEMPLATES "")
   set(itk_Wrap_Explicit_Prefix "${prefix}")
   set(itk_Wrap_Explicit_Class "${class}")
endmacro(WRAP_EXPLICIT_TYPE)

macro(END_WRAP_EXPLICIT_TYPE)
   # create the type vars.
   # must be called after END_WRAP_EXPLICIT_TYPE
   # Create the vars used for to designe types in all the cmake
   # files. This method ensure all the type names are constructed
   # with the same method
   foreach(wrap ${WRAPPER_TEMPLATES})
      string(REGEX REPLACE "([0-9A-Za-z]*)[ ]*#[ ]*(.*)" "\\1" wrapTpl "${wrap}")
      string(REGEX REPLACE "([0-9A-Za-z]*)[ ]*#[ ]*(.*)" "\\2" wrapType "${wrap}")
      set(ITKT_${itk_Wrap_Explicit_Prefix}${wrapTpl} "Templates::${itk_Wrap_Explicit_Class}${wrapTpl}")
      set(ITKM_${itk_Wrap_Explicit_Prefix}${wrapTpl} "${itk_Wrap_Explicit_Prefix}${wrapTpl}")
   endforeach(wrap)
endmacro(END_WRAP_EXPLICIT_TYPE)

#------------------------------------------------------------------------------

# now, define types vars
# the result is stored in itk_Wrap_Explicit_XXX where XXX is the name of the class
# to be wrapped in there own file, most of the time in CommonA


WRAP_EXPLICIT_TYPE("Offset" "O")
  foreach(d ${EXPLICIT_ITK_DIMS})
    WRAP_TEMPLATE("${d}"  "${d}")
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_Offset ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("Vector" "V")
  foreach(d ${EXPLICIT_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_F}${d}"  "${ITKT_F},${d}")
    WRAP_TEMPLATE("${ITKM_D}${d}"  "${ITKT_D},${d}")
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_Vector ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("CovariantVector" "CV")
  foreach(d ${EXPLICIT_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_F}${d}"  "${ITKT_F},${d}")
    WRAP_TEMPLATE("${ITKM_D}${d}"  "${ITKT_D},${d}")
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_CovariantVector ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("ContinuousIndex" "CI")
  foreach(d ${EXPLICIT_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_F}${d}"  "${ITKT_F},${d}")
    WRAP_TEMPLATE("${ITKM_D}${d}"  "${ITKT_D},${d}")
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_ContinuousIndex ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("Array" "A")
  WRAP_TEMPLATE("${ITKM_D}" "${ITKT_D}")
  WRAP_TEMPLATE("${ITKM_F}" "${ITKT_F}")
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_Array ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("Array2D" "A2D")
  WRAP_TEMPLATE("${ITKM_D}" "${ITKT_D}")
  WRAP_TEMPLATE("${ITKM_F}" "${ITKT_F}")
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_Array2D ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("FixedArray" "FA")
  UNIQUE(array_sizes "${EXPLICIT_ITK_DIMS};1;4")
  # make sure that 1-D FixedArrays are wrapped. Also wrap for each selected
  # image dimension. We also wrapped 4D arrays since some of the classes need
  # it.
  # TODO: Do we need fixed arrays for all of these types? For just the selected
  # pixel types plus some few basic cases? Or just for a basic set of types?
  foreach(d ${array_sizes})
    WRAP_TEMPLATE("${ITKM_D}${d}"  "${ITKT_D},${d}")
    WRAP_TEMPLATE("${ITKM_F}${d}"  "${ITKT_F},${d}")
    WRAP_TEMPLATE("${ITKM_UL}${d}" "${ITKT_UL},${d}")
    WRAP_TEMPLATE("${ITKM_US}${d}" "${ITKT_US},${d}")
    WRAP_TEMPLATE("${ITKM_UC}${d}" "${ITKT_UC},${d}")
    WRAP_TEMPLATE("${ITKM_UI}${d}" "${ITKT_UI},${d}")
    WRAP_TEMPLATE("${ITKM_SL}${d}" "${ITKT_SL},${d}")
    WRAP_TEMPLATE("${ITKM_SS}${d}" "${ITKT_SS},${d}")
    WRAP_TEMPLATE("${ITKM_SC}${d}" "${ITKT_SC},${d}")
    WRAP_TEMPLATE("${ITKM_B}${d}"  "${ITKT_B},${d}")
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_FixedArray ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("RGBPixel" "RGB")
  if(EXPLICIT_rgb_unsigned_char)
    WRAP_TEMPLATE("${ITKM_UC}" "${ITKT_UC}")
  endif(EXPLICIT_rgb_unsigned_char)

  if(EXPLICIT_rgb_unsigned_short)
    WRAP_TEMPLATE("${ITKM_US}" "${ITKT_US}")
  endif(EXPLICIT_rgb_unsigned_short)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_RGBPixel ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("Image" "I")
  # Make a list of all of the selected image pixel types and also double (for
  # BSplineDeformableTransform), uchar (for 8-bit image output), and ulong
  # (for the watershed and relabel filters).
  UNIQUE(wrap_image_types "${EXPLICIT_ITK_ALL_TYPES};D;UC;UL")

  foreach(d ${EXPLICIT_ITK_DIMS})
    foreach(type ${wrap_image_types})
      if("${EXPLICIT_ITK_VECTOR}" MATCHES "(^|;)${type}(;|$)")
        # if the type is a vector type with no dimension specified, make the
        # vector dimension match the image dimension.
        set(type "${type}${d}")
      endif("${EXPLICIT_ITK_VECTOR}" MATCHES "(^|;)${type}(;|$)")

      WRAP_TEMPLATE("${ITKM_${type}}${d}"  "${ITKT_${type}},${d}")
    endforeach(type)
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_Image ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("VectorImage" "VI")
  # Make a list of all of the selected image pixel types and also uchar
  # (for 8-bit image output)
  UNIQUE(wrap_image_types "${EXPLICIT_ITK_SCALAR};UC")

  foreach(d ${EXPLICIT_ITK_DIMS})
    foreach(type ${wrap_image_types})
      WRAP_TEMPLATE("${ITKM_${type}}${d}"  "${ITKT_${type}},${d}")
    endforeach(type)
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_VectorImage ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("VariableLengthVector" "VLV")
  # Make a list of all of the selected image pixel types and also uchar
  # (for 8-bit image output)
  UNIQUE(wrap_image_types "${EXPLICIT_ITK_SCALAR};UC")

  foreach(type ${wrap_image_types})
    WRAP_TEMPLATE("${ITKM_${type}}"  "${ITKT_${type}}")
  endforeach(type)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_VariableLengthVector ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("Point" "P")
  foreach(d ${EXPLICIT_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_F}${d}"  "${ITKT_F},${d}")
    WRAP_TEMPLATE("${ITKM_D}${d}"  "${ITKT_D},${d}")
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_Point ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("LevelSetNode" "LSN")
  # Only make level set nodes for the selected image pixel types
  foreach(d ${EXPLICIT_ITK_DIMS})
    foreach(type ${EXPLICIT_ITK_SCALAR})
      WRAP_TEMPLATE("${ITKM_${type}}${d}"  "${ITKT_${type}},${d}")
    endforeach(type)
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_LevelSetNode ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("BinaryBallStructuringElement" "SE")
  # Only make structuring elements for the selected image pixel types
  foreach(d ${EXPLICIT_ITK_DIMS})
    foreach(type ${EXPLICIT_ITK_SCALAR})
      WRAP_TEMPLATE("${ITKM_${type}}${d}"  "${ITKT_${type}},${d}")
    endforeach(type)
    WRAP_TEMPLATE("${ITKM_B}${d}"  "${ITKT_B},${d}")
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_StructuringElement ${WRAPPER_TEMPLATES})

WRAP_EXPLICIT_TYPE("SpatialObject" "SO")
  foreach(d ${EXPLICIT_ITK_DIMS})
    WRAP_TEMPLATE("${d}"  "${d}")
  endforeach(d)
END_WRAP_EXPLICIT_TYPE()
set(itk_Wrap_Explicit_SpatialObject ${WRAPPER_TEMPLATES})

