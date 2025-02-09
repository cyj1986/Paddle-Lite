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

#include "lite/operators/scale_op.h"
#include <gtest/gtest.h>
#include <random>
#include "lite/backends/npu/bridge/registry.h"
#include "lite/backends/npu/bridge/test_helper.h"
#include "lite/core/op_registry.h"

namespace paddle {
namespace lite {
namespace npu {
namespace bridge {

void scale_ref(const std::shared_ptr<operators::ScaleOp> op) {
  Scope* scope = op->scope();
  const OpInfo* op_info = op->op_info();
  auto x = scope->FindVar(op_info->Input("X").front())->GetMutable<Tensor>();
  auto out =
      scope->FindVar(op_info->Output("Out").front())->GetMutable<Tensor>();
  float scale = op_info->GetAttr<float>("scale");
  float bias = op_info->GetAttr<float>("bias");
  bool bias_after_scale = op_info->GetAttr<bool>("bias_after_scale");
  if (!bias_after_scale) {
    bias *= scale;
  }
  auto x_data = x->data<float>();
  auto out_data = out->mutable_data<float>();
  DDim x_dims = x->dims();
  DDim out_dims = out->dims();
  CHECK_EQ(x_dims.production(), out_dims.production());
  for (int i = 0; i < out_dims.production(); i++) {
    out_data[i] = x_data[i] * scale + bias;
  }
}

void test_scale(int bs,
                int ic,
                int ih,
                int iw,
                bool bias_after_scale,
                float scale,
                float bias) {
  // prepare input&output variables
  Scope scope;
  std::string x_var_name("x");
  std::string out_var_name("out");
  std::string out_ref_var_name("out_ref");
  auto* x = scope.Var(x_var_name)->GetMutable<Tensor>();
  auto* out = scope.Var(out_var_name)->GetMutable<Tensor>();
  auto* out_ref = scope.Var(out_ref_var_name)->GetMutable<Tensor>();
  x->Resize({bs, ic, ih, iw});

  // initialize input&output data
  FillTensor<float, int>(x);

  // initialize op desc
  cpp::OpDesc opdesc;
  opdesc.SetType("scale");
  opdesc.SetInput("X", {x_var_name});
  opdesc.SetOutput("Out", {out_var_name});
  opdesc.SetAttr("bias_after_scale", bias_after_scale);
  opdesc.SetAttr("scale", scale);
  opdesc.SetAttr("bias", bias);

  // create and convert op to NPU model, then run it on NPU
  auto op = CreateOp<operators::ScaleOp>(opdesc, &scope);
  LauchOp(op, {x_var_name}, {out_var_name});
  out_ref->CopyDataFrom(*out);

  // execute reference implementation and save to output tensor('out')
  scale_ref(op);

  // compare results
  auto* out_data = out->mutable_data<float>();
  auto* out_ref_data = out_ref->mutable_data<float>();
  for (int i = 0; i < out->dims().production(); i++) {
    VLOG(5) << i;
    EXPECT_NEAR(out_data[i], out_ref_data[i], 1e-5);
  }
}

TEST(NPUBridges, scale) {
  for (auto bs : {1, 3}) {
    for (auto ic : {1, 3}) {
      for (auto ih : {3, 4}) {
        for (auto iw : {4, 3}) {
          for (auto bias_after_scale : {true, false}) {
            for (auto scale : {-1.0f, 5.0f}) {
              for (auto bias : {-2.0f, 30.0f}) {
                VLOG(3) << "bs: " << bs << " ic: " << ic << " ih: " << ih
                        << " iw: " << iw
                        << " bias_after_scale: " << bias_after_scale
                        << " scale: " << scale << " bias: " << bias;
                test_scale(bs, ic, ih, iw, bias_after_scale, scale, bias);
              }
            }
          }
        }
      }
    }
  }
}

}  // namespace bridge
}  // namespace npu
}  // namespace lite
}  // namespace paddle

USE_LITE_OP(scale);
USE_NPU_BRIDGE(scale);
