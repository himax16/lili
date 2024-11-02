/**
 * @file field_test.cpp
 * @brief Unit tests for the Fields class
 */

#include <gtest/gtest.h>

#include "fields.hpp"
#include "mesh.hpp"

TESET(FieldTest, FieldDefaultInit) {
  lili::mesh::Fields fields;

  EXPECT_EQ(fields.size.dim(), 1);
  EXPECT_EQ(fields.size.nx(), 1);
  EXPECT_EQ(fields.size.ny(), 1);
  EXPECT_EQ(fields.size.nz(), 1);
  EXPECT_EQ(fields.size.ngx(), 0);
  EXPECT_EQ(fields.size.ngy(), 0);
  EXPECT_EQ(fields.size.ngz(), 0);
  EXPECT_EQ(fields.size.lx(), 1.0);
  EXPECT_EQ(fields.size.ly(), 1.0);
  EXPECT_EQ(fields.size.lz(), 1.0);
  EXPECT_EQ(fields.size.x0(), 0.0);
  EXPECT_EQ(fields.size.y0(), 0.0);
  EXPECT_EQ(fields.size.z0(), 0.0);
  EXPECT_EQ(fields.dx(), 1.0);
  EXPECT_EQ(fields.dy(), 1.0);
  EXPECT_EQ(fields.dz(), 1.0);
}

TEST(FieldTest, FieldInit) {
  lili::mesh::Fields fields(10, 10, 10);
  EXPECT_EQ(fields.ex.size(), 10 * 10 * 10);
  EXPECT_EQ(fields.ey.size(), 10 * 10 * 10);
  EXPECT_EQ(fields.ez.size(), 10 * 10 * 10);
  EXPECT_EQ(fields.bx.size(), 10 * 10 * 10);
  EXPECT_EQ(fields.by.size(), 10 * 10 * 10);
  EXPECT_EQ(fields.bz.size(), 10 * 10 * 10);
  EXPECT_EQ(fields.size.dim, 1);
  EXPECT_EQ(fields.size.nx, 10);
  EXPECT_EQ(fields.size.ny, 10);
}

TEST(FieldTest, FieldCopy) {
  lili::mesh::Fields fields(10, 10, 10);
  fields.ex = 5.0;
  fields.ex(5, 5, 0) = 1.2;
  lili::mesh::Fields field2(fields);
  EXPECT_EQ(fields.ex(5, 5, 0), field2.ex(5, 5, 0));
  EXPECT_EQ(fields.ex(1, 5, 0), field2.ex(1, 5, 0));
}
