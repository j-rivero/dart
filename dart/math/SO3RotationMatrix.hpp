/*
 * Copyright (c) 2016, Graphics Lab, Georgia Tech Research Corporation
 * Copyright (c) 2016, Humanoid Lab, Georgia Tech Research Corporation
 * Copyright (c) 2016, Personal Robotics Lab, Carnegie Mellon University
 * All rights reserved.
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DART_MATH_SO3ROTATIONMATRIX_HPP_
#define DART_MATH_SO3ROTATIONMATRIX_HPP_

#include <Eigen/Eigen>

#include "dart/math/MathTypes.hpp"
#include "dart/math/Geometry.hpp"
#include "dart/math/SO3Base.hpp"

namespace dart {
namespace math {

struct RotationMatrixRep : SO3Representation {};

template <typename S_>
class SO3<S_, RotationMatrixRep> : public SO3Base<SO3<S_, RotationMatrixRep>>
{
public:

  enum ConstructFromRotationMatrixTag { ConstructFromRotationMatrix };

  using This = SO3<S_, RotationMatrixRep>;
  using Base = SO3Base<This>;
  using S = typename Base::S;
  using Rep = typename Base::Rep;
  using RotationMatrixType = typename Base::RotationMatrixType;
  using RepDataType = typename Base::RepDataType;
  using Tangent = typename Base::Tangent;
  using so3 = typename Base::so3;

  using Base::operator =;
  using Base::operator *;
  using Base::operator *=;

  using Base::coordinates;
  using Base::matrix;

  SO3() : mRepData(RepDataType())
  {
    // Do nothing
  }

  SO3(const SO3& other) : Base(), mRepData(other.mRepData)
  {
    // Do nothing
  }

  SO3(SO3&& other) : mRepData(std::move(other.mRepData))
  {
    // Do nothing
  }

  template <typename Derived>
  SO3(const SO3Base<Derived>& other)
    : mRepData(detail::SO3::convert_impl<S, typename Derived::Rep, Rep>::run(
              other.derived().matrix()))
  {
    // Do nothing
  }

  template <typename Derived>
  SO3(SO3Base<Derived>&& other)
    : mRepData(detail::SO3::convert_impl<S, typename Derived::Rep, Rep>::run(
              std::move(other.derived().matrix())))
  {
    // Do nothing
  }

  template <typename Derived>
  SO3(ConstructFromRotationMatrixTag,
      const Eigen::MatrixBase<Derived>& matrix) : mRepData(matrix)
  {
    using namespace Eigen;
    EIGEN_STATIC_ASSERT_SAME_MATRIX_SIZE(Derived, RepDataType)
  }

  template <typename Derived>
  SO3(ConstructFromRotationMatrixTag,
      Eigen::MatrixBase<Derived>&& matrix) : mRepData(std::move(matrix))
  {
    using namespace Eigen;
    EIGEN_STATIC_ASSERT_SAME_MATRIX_SIZE(Derived, RepDataType)
  }

  explicit SO3(const so3& tangent) : mRepData(expMapRot(tangent))
  {
    // Do nothing
  }

  explicit SO3(so3&& tangent) : mRepData(expMapRot(std::move(tangent)))
  {
    // Do nothing
  }

  SO3& operator=(const SO3& other)
  {
    Base::operator =(other);
    return *this;
  }

  SO3& operator=(SO3&& other)
  {
    Base::operator=(std::move(other));
    return *this;
  }

  bool operator ==(const SO3& other)
  {
    return mRepData == other.mRepData;
  }

  void setIdentity()
  {
    mRepData.setIdentity();
  }

  void setRandom()
  {
    *this = exp(Tangent::Random());
    // TODO(JS): improve
  }

  void inverseInPlace()
  {
    mRepData.transposeInPlace();
  }

  const SO3 inverse() const
  {
    return SO3(ConstructFromRotationMatrix, mRepData.transpose());
  }

  static SO3 exp(const so3& tangent)
  {
    return SO3(ConstructFromRotationMatrix, expMapRot(tangent));
    // TODO(JS): improve
  }

  static so3 log(const This& point)
  {
    return ::dart::math::logMap(point.mRepData);
    // TODO(JS): improve
  }

  /// \returns A pointer to the data array of internal data type
  S* data()
  {
    return mRepData.data();
  }

protected:

  template <typename>
  friend class SO3Base;

  RepDataType mRepData;
};

} // namespace math
} // namespace dart

#endif // DART_MATH_SO3ROTATIONMATRIX_HPP_
