/**
 * @file field_test.cpp
 * @brief Unit tests for the Field class
 */

#include "field.hpp"

#include <gtest/gtest.h>

#include "mesh.hpp"

TESET(FieldTest, FieldDefaultInit) {
  lili::mesh::Field field;

  EXPECT_EQ(field.size.dim(), 1);
  EXPECT_EQ(field.size.nx(), 1);
  EXPECT_EQ(field.size.ny(), 1);
  EXPECT_EQ(field.size.nz(), 1);
  EXPECT_EQ(field.size.ngx(), 0);
  EXPECT_EQ(field.size.ngy(), 0);
  EXPECT_EQ(field.size.ngz(), 0);
  EXPECT_EQ(field.size.lx(), 1.0);
  EXPECT_EQ(field.size.ly(), 1.0);
  EXPECT_EQ(field.size.lz(), 1.0);
  EXPECT_EQ(field.size.x0(), 0.0);
  EXPECT_EQ(field.size.y0(), 0.0);
  EXPECT_EQ(field.size.z0(), 0.0);
  EXPECT_EQ(field.dx(), 1.0);
  EXPECT_EQ(field.dy(), 1.0);
  EXPECT_EQ(field.dz(), 1.0);
}

TEST(FieldTest, FieldInit) {
  lili::mesh::Field field(10, 10, 10);
  EXPECT_EQ(field.ex.size(), 10 * 10 * 10);
  EXPECT_EQ(field.ey.size(), 10 * 10 * 10);
  EXPECT_EQ(field.ez.size(), 10 * 10 * 10);
  EXPECT_EQ(field.bx.size(), 10 * 10 * 10);
  EXPECT_EQ(field.by.size(), 10 * 10 * 10);
  EXPECT_EQ(field.bz.size(), 10 * 10 * 10);
  EXPECT_EQ(field.size.dim, 1);
  EXPECT_EQ(field.size.nx, 10);
  EXPECT_EQ(field.size.ny, 10);
}

TEST(FieldTest, FieldCopy) {
  lili::mesh::Field field(10, 10, 10);
  field.ex = 5.0;
  field.ex(5, 5, 0) = 1.2;
  lili::mesh::Field field2(field);
  EXPECT_EQ(field.ex(5, 5, 0), field2.ex(5, 5, 0));
  EXPECT_EQ(field.ex(1, 5, 0), field2.ex(1, 5, 0));
}
