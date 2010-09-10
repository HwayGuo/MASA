program main
  use masa
  use iso_c_binding

  implicit none

  real*8 :: value

   print*,'Inside Fortran main'

!  call masa_init(C_CHAR_"mytest"//C_NULL_CHAR,C_CHAR_"euler_1d"//C_NULL_CHAR)
  call masa_init('mytest','euler_1d')
  call masa_init_param()


  call masa_list_mms()
  call masa_sanity_check()

  call masa_display_param

  print*,'getting original L' 
  value = masa_get_param("L")

  print*,'Original L = ',value

  call masa_set_param("L",3.1415d0)
  call masa_display_param

  print*,'getting updated L' 
  value = masa_get_param("L")

  print*,'Updated L = ',value

  stop
end program main
