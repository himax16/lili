std::cout << "Ex init : " << field.ex.data() << std::endl;
// Change some data
field.ex = 5.0;
field.ex(5, 5, 0) = 1.2;
mesh::Field field2(field);
std::cout << "Ex copy : " << field2.ex.data() << std::endl;
std::cout << "Ex after: " << field.ex.data() << std::endl;
// Check if the data is the same
std::cout << "Ex check: " << field.ex(5, 5, 0) << " and " << field2.ex(5, 5, 0)
          << std::endl;
std::cout << "Ex check: " << field.ex(1, 5, 0) << " and " << field2.ex(1, 5, 0)
          << std::endl;