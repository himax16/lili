/**
 * @file lili.cpp
 * @brief Main LILI program
 *
 */

// #include <numbers>

#include "hdf5.h"
#include "input.hpp"
#include "mesh.hpp"

/**
 * @brief Main `LILI` program
 *
 * @param[in] argc
 *  Number of command line arguments
 * @param[in] argv
 *  Command line arguments
 */
int main(int argc, char *argv[]) {
  // Parse inputs
  lili::input::ParseArguments(argc, argv);

  // Test print
  std::cout << lili::input::gInputFile << std::endl;
  std::cout << lili::input::gProblemName << std::endl;

  // Test mesh
  lili::mesh::Mesh<double> mesh(9, 10, 4);
  std::cout << mesh.n0() << std::endl;
  std::cout << mesh.n1() << std::endl;
  std::cout << mesh.n2() << std::endl;
  std::cout << mesh.data() << std::endl;

  // Buildin instruction
  __builtin_cpu_init();
  std::cout << "Test builtin instruction" << std::endl;
  std::cout << "sse " << __builtin_cpu_supports("sse") << std::endl;
  std::cout << "sse2 " << __builtin_cpu_supports("sse2") << std::endl;
  std::cout << "ssse3 " << __builtin_cpu_supports("ssse3") << std::endl;
  std::cout << "avx " << __builtin_cpu_supports("avx") << std::endl;
  std::cout << "avx2 " << __builtin_cpu_supports("avx2") << std::endl;
  std::cout << "avx512f " << __builtin_cpu_supports("avx512f") << std::endl;
  std::cout << "avx512vpopcntdq " << __builtin_cpu_supports("avx512vpopcntdq")
            << std::endl;
  /****************************************************************************/
  // // Test HDF5
  // hid_t file_id, dataset_id, dataspace_id; /* identifiers */
  // herr_t status;
  // int i, j, dset_data[4][6], read_data[4][6];
  // hsize_t dims[2];

  // /* Initialize the dataset. */
  // for (i = 0; i < 4; i++)
  //   for (j = 0; j < 6; j++) dset_data[i][j] = i * 6 + j + 1;

  // /* Create a new file using default properties. */
  // file_id = H5Fcreate("dset.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  // /* Create the data space for the dataset. */
  // dims[0] = 4;
  // dims[1] = 6;
  // dataspace_id = H5Screate_simple(2, dims, NULL);

  // /* Create the dataset. */
  // dataset_id = H5Dcreate2(file_id, "/dset", H5T_STD_I32BE, dataspace_id,
  //                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  // /* Write the dataset. */
  // status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
  // H5P_DEFAULT,
  //                   dset_data);

  // /* End access to the dataset and release resources used by it. */
  // status = H5Dclose(dataset_id);

  // //------------------------------------------------------

  // /* Open an existing dataset. */
  // dataset_id = H5Dopen2(file_id, "/dset", H5P_DEFAULT);

  // status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
  //                  read_data);

  // for (i = 0; i < 4; i++)
  //   for (j = 0; j < 6; j++) printf("%d ", read_data[i][j]);  // 1-24
  // printf("\n");

  // /* Close the dataset. */
  // status = H5Dclose(dataset_id);

  // /* Close the file. */
  // status = H5Fclose(file_id);
  /****************************************************************************/
  // Variable declaration
  // int n_simsys = 0;
  // bool has_mhd = false;
  // bool use_emf = true;

  // int i_loop = 0;
  // int n_loop = 10;

  // // Initialize simulation system based on the input file
  // if (n_simsys <= 0) {
  //   std::cerr << "Number of simulated system must be positive" << std::endl;
  //   exit(10);
  // }
  // for (int i_simsys = 0; i_simsys < n_simsys; ++i_simsys) {
  //   switch (simsys[i_simsys].type) {
  //     case 'm':
  //       if (has_mhd) {
  //         std::cerr << "There are multiple MHD simulation system" <<
  //         std::endl; exit(12);
  //       }
  //       initialize_mhdsys(simsys[i_simsys].property);
  //       has_mhd = true;
  //     case 'p':
  //       initialize_parsys(simsys[i_simsys].property);
  //       break;
  //     case 'f':
  //       initialize_flusys(simsys[i_simsys].property);
  //       break;
  //     case 'n':
  //       initialize_neusys(simsys[i_simsys].property);
  //       break;
  //     default:
  //       std::cerr << "Unrecognized simulation system" << std::endl;
  //       exit(11);
  //   }
  // }

  // // Initialize field variable if needed
  // if (use_emf) {
  //   initialize_emfsys();
  // }

  // // boost::json::object obj;
  // // obj["pi"] = boost::math::constants::pi<double>();

  // // std::cout << obj << std::endl;

  // // LILI main loop
  // for (i_loop = 0; i_loop < n_loop; ++i_loop) {
  //   std::cout << i_loop << std::endl;

  // }

  return 0;
}
