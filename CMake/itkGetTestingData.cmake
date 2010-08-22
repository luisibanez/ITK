find_program( MIDAScli_EXECUTABLE MIDAScli )                                  #find the MIDAscli exectutable

macro( get_testing_data TEST_NAME DESTINATION_DIR PATH_TO_ITEM_OR_BITSTREAM ) #The macro needs 3 parameters: name of test,the directory where to pull the data and the path of the data in the server

  if( DEFINED MCN )                                                           # MCN = Macro Call Number: to distinguish each call of the macro
    math(EXPR MCN "${MCN}+1" )
  else()
    set( MCN 1 )
  endif()


  set( DIN 1 )                                                                # DIN = Downloaded Item Number: to distinguish the items that are dowloaded

  foreach( test_download_data ${PATH_TO_ITEM_OR_BITSTREAM} )                  #The variable takes the value of the different items

    add_test( ${TEST_NAME}_${MCN}_${DIN}
      ${MIDAScli_EXECUTABLE} --database ${MIDASdatabase} pull -d ${DESTINATION_DIR}  #pulling the data into the destination directory
       -r -i ${test_download_data} ${MIDASurl} )

    math( EXPR DIN "${DIN}+1" )                                               #Each time the macro is called, MCN is increased by 1

  endforeach( test_download_data )

endmacro()
