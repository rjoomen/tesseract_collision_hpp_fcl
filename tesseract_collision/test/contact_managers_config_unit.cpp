/**
 * @file contact_managers_config_unit.cpp
 * @brief Tesseract collision contact managers config test
 *
 * @author Levi Armstrong
 * @date December 26, 2022
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2022, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <gtest/gtest.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_collision/test_suite/contact_manager_config_unit.hpp>
#include <tesseract_collision/bullet/bullet_discrete_simple_manager.h>
#include <tesseract_collision/bullet/bullet_discrete_bvh_manager.h>
#include <tesseract_collision/bullet/bullet_cast_simple_manager.h>
#include <tesseract_collision/bullet/bullet_cast_bvh_manager.h>
#include <tesseract_collision/fcl/fcl_discrete_managers.h>
#include <tesseract_collision/hpp_fcl/hpp_fcl_discrete_managers.h>
#include <tesseract_collision/core/utils.h>

using namespace tesseract_collision;

class AlwaysTrueContactAllowedValidator : public tesseract_common::ContactAllowedValidator
{
public:
  bool operator()(const std::string&, const std::string&) const override { return true; }
};

class AlwaysFalseContactAllowedValidator : public tesseract_common::ContactAllowedValidator
{
public:
  bool operator()(const std::string&, const std::string&) const override { return false; }
};

class TestOrigContactAllowedValidator : public tesseract_common::ContactAllowedValidator
{
public:
  bool operator()(const std::string& s1, const std::string& s2) const override
  {
    if (s1 == "link_1" && s2 == "link_2")
      return true;

    if (s1 == "link_1" && s2 == "link_3")
      return true;

    return false;
  }
};

class TestOvrdContactAllowedValidator : public tesseract_common::ContactAllowedValidator
{
public:
  bool operator()(const std::string& s1, const std::string& s2) const override
  {
    return (s1 == "link_1" && s2 == "link_2");
  }
};

TEST(TesseractCollisionUnit, BulletDiscreteSimpleContactManagerConfigUnit)  // NOLINT
{
  tesseract_collision_bullet::BulletDiscreteSimpleManager checker;
  test_suite::runTest(checker);
}

TEST(TesseractCollisionUnit, BulletDiscreteBVHContactManagerConfigUnit)  // NOLINT
{
  tesseract_collision_bullet::BulletDiscreteBVHManager checker;
  test_suite::runTest(checker);
}

TEST(TesseractCollisionUnit, BulletCastSimpleContactManagerConfigUnit)  // NOLINT
{
  tesseract_collision_bullet::BulletCastSimpleManager checker;
  test_suite::runTest(checker);
}

TEST(TesseractCollisionUnit, BulletCastBVHContactManagerConfigUnit)  // NOLINT
{
  tesseract_collision_bullet::BulletCastBVHManager checker;
  test_suite::runTest(checker);
}

TEST(TesseractCollisionUnit, FCLDiscreteBVHContactManagerConfigUnit)  // NOLINT
{
  tesseract_collision_fcl::FCLDiscreteBVHManager checker;
  test_suite::runTest(checker);
}

TEST(TesseractCollisionUnit, HPP_FCLDiscreteBVHContactManagerConfigUnit)  // NOLINT
{
  tesseract_collision_hpp_fcl::HPP_FCLDiscreteBVHManager checker;
  test_suite::runTest(checker);
}

TEST(TesseractCollisionUnit, CombineContactAllowedFnUnit)  // NOLINT
{
  {  // tesseract_collision::ACMOverrideType::NONE
    auto orig = std::make_shared<AlwaysTrueContactAllowedValidator>();
    auto ovrd = std::make_shared<AlwaysFalseContactAllowedValidator>();

    auto comb = combineContactAllowedValidators(orig, ovrd, tesseract_collision::ACMOverrideType::NONE);
    EXPECT_TRUE((*comb)("", ""));
  }

  {  // tesseract_collision::ACMOverrideType::ASSIGN
    auto orig = std::make_shared<AlwaysTrueContactAllowedValidator>();
    auto ovrd = std::make_shared<AlwaysFalseContactAllowedValidator>();

    auto comb = combineContactAllowedValidators(orig, ovrd, tesseract_collision::ACMOverrideType::ASSIGN);
    EXPECT_FALSE((*comb)("", ""));
  }

  {  // tesseract_collision::ACMOverrideType::AND
    auto orig = std::make_shared<TestOrigContactAllowedValidator>();
    auto ovrd = std::make_shared<TestOvrdContactAllowedValidator>();

    auto comb = combineContactAllowedValidators(orig, ovrd, tesseract_collision::ACMOverrideType::AND);
    EXPECT_TRUE((*comb)("link_1", "link_2"));
    EXPECT_FALSE((*comb)("link_1", "link_3"));
    EXPECT_FALSE((*comb)("abc", "def"));

    auto comb1 = combineContactAllowedValidators(nullptr, ovrd, tesseract_collision::ACMOverrideType::AND);
    EXPECT_TRUE(comb1 == nullptr);
  }

  {  // tesseract_collision::ACMOverrideType::AND
    auto orig = std::make_shared<TestOrigContactAllowedValidator>();
    auto ovrd = std::make_shared<TestOvrdContactAllowedValidator>();

    auto comb = combineContactAllowedValidators(orig, ovrd, tesseract_collision::ACMOverrideType::OR);
    EXPECT_TRUE((*comb)("link_1", "link_2"));
    EXPECT_TRUE((*comb)("link_1", "link_3"));
    EXPECT_FALSE((*comb)("abc", "def"));

    auto comb1 = combineContactAllowedValidators(nullptr, ovrd, tesseract_collision::ACMOverrideType::OR);
    EXPECT_TRUE((*comb1)("link_1", "link_2"));
    EXPECT_FALSE((*comb1)("link_1", "link_3"));
    EXPECT_FALSE((*comb1)("abc", "def"));
  }
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
