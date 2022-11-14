option(COVDEL_BUILD_CV "Build ComputerVision module" FALSE)
option(COVDEL_BUILD_MA "Build MultiArray module" TRUE)
option(COVDEL_BUILD_NN "Build NeuralNetwork module" FALSE)

if(COVDEL_BUILD_CV)
  add_subdirectory(cv)
endif()

if(COVDEL_BUILD_MA)
  add_subdirectory(ma)
endif()

if(COVDEL_BUILD_NN)
  add_subdirectory(nn)
endif()
