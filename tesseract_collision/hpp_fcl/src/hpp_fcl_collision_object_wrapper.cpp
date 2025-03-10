/**
 * @file hpp_fcl_collision_object_wrapper.cpp
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

#include <tesseract_collision/hpp_fcl/hpp_fcl_collision_object_wrapper.h>

namespace tesseract_collision::tesseract_collision_hpp_fcl
{
void HPP_FCLCollisionObjectWrapper::setContactDistanceThreshold(double contact_distance)
{
  contact_distance_ = contact_distance;
  updateAABB();
}

double HPP_FCLCollisionObjectWrapper::getContactDistanceThreshold() const { return contact_distance_; }

void HPP_FCLCollisionObjectWrapper::updateAABB()
{
  computeAABB();
  aabb.expand(contact_distance_);
}

void HPP_FCLCollisionObjectWrapper::setShapeIndex(int index) { shape_index_ = index; }

int HPP_FCLCollisionObjectWrapper::getShapeIndex() const
{
  assert(shape_index_ >= 0);
  return shape_index_;
}

}  // namespace tesseract_collision::tesseract_collision_hpp_fcl
