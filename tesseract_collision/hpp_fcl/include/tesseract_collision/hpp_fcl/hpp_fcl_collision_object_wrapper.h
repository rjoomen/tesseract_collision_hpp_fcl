/**
 * @file hpp_fcl_collision_object_wrapper.h
 * @brief Collision Object Wrapper to modify AABB with contact distance threshold
 *
 * @author Levi Armstrong
 * @date April 14, 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
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
#ifndef TESSERACT_COLLISION_HPP_FCL_COLLISION_OBJECT_WRAPPER_H
#define TESSERACT_COLLISION_HPP_FCL_COLLISION_OBJECT_WRAPPER_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <hpp/fcl/collision_object.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

namespace tesseract_collision::tesseract_collision_hpp_fcl
{
/**
 * @brief This is a wrapper around HPP-FCL Collision Object Class which allows you to expand the AABB by the contact
 * dist.
 *
 * This significantly improves performance when making distance requests if performing a contact tests type FIRST.
 */
class HPP_FCLCollisionObjectWrapper : public hpp::fcl::CollisionObject
{
public:
  using hpp::fcl::CollisionObject::CollisionObject;

  /**
   * @brief Set the collision objects contact distance threshold.
   *
   * This automatically calls updateAABB() which increases the AABB by the contact distance.
   * @param contact_distance The contact distance threshold.
   */
  void setContactDistanceThreshold(double contact_distance);

  /**
   * @brief Get the collision objects contact distance threshold.
   * @return The contact distance threshold.
   */
  double getContactDistanceThreshold() const;

  /**
   * @brief Update the internal AABB. This must be called instead of the base class computeAABB().
   *
   * After setting the collision objects transform this must be called.
   */
  void updateAABB();

  /**
   * @brief Set the shape index. This is the geometries index in the urdf.
   * @param index The index
   */
  void setShapeIndex(int index);

  /**
   * @brief Get the shape index. This is the geometries index in the urdf.
   * @return The shape index
   */
  int getShapeIndex() const;

protected:
  double contact_distance_{ 0 }; /**< @brief The contact distance threshold. */

  /** @brief The shape index, which is the geometries index in the urdf. */
  int shape_index_{ -1 };
};

}  // namespace tesseract_collision::tesseract_collision_hpp_fcl

#endif  // TESSERACT_COLLISION_HPP_FCL_COLLISION_OBJECT_WRAPPER_H
