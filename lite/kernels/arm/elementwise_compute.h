// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <algorithm>
#include "lite/core/kernel.h"
#include "lite/core/op_registry.h"

namespace paddle {
namespace lite {
namespace kernels {
namespace arm {

class ElementwiseAddCompute
    : public KernelLite<TARGET(kARM), PRECISION(kFloat)> {
 public:
  void Run() override;

  virtual ~ElementwiseAddCompute() = default;
};

class ElementwiseAddActivationCompute
    : public KernelLite<TARGET(kARM), PRECISION(kFloat)> {
 public:
  void Run() override;

  virtual ~ElementwiseAddActivationCompute() = default;
};

class ElementwiseMulCompute
    : public KernelLite<TARGET(kARM), PRECISION(kFloat)> {
 public:
  void Run() override;

  virtual ~ElementwiseMulCompute() = default;
};

class ElementwiseMulActivationCompute
    : public KernelLite<TARGET(kARM), PRECISION(kFloat)> {
 public:
  void Run() override;

  virtual ~ElementwiseMulActivationCompute() = default;
};

class ElementwiseMaxCompute
    : public KernelLite<TARGET(kARM), PRECISION(kFloat)> {
 public:
  void Run() override;

  virtual ~ElementwiseMaxCompute() = default;
};

class ElementwiseMaxActivationCompute
    : public KernelLite<TARGET(kARM), PRECISION(kFloat)> {
 public:
  void Run() override;

  virtual ~ElementwiseMaxActivationCompute() = default;
};

class ElementwiseDivCompute
    : public KernelLite<TARGET(kARM), PRECISION(kFloat)> {
 public:
  void Run() override;

  virtual ~ElementwiseDivCompute() = default;
};

class ElementwiseDivActivationCompute
    : public KernelLite<TARGET(kARM), PRECISION(kFloat)> {
 public:
  void Run() override;

  virtual ~ElementwiseDivActivationCompute() = default;
};

}  // namespace arm
}  // namespace kernels
}  // namespace lite
}  // namespace paddle
