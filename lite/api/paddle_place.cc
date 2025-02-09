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

#include "lite/api/paddle_place.h"
#include "lite/utils/cp_logging.h"
#include "lite/utils/hash.h"
#include "lite/utils/replace_stl/stream.h"
#include "lite/utils/string.h"

namespace paddle {
namespace lite_api {

size_t Place::hash() const {
  std::hash<int> h;
  size_t hash = h(static_cast<int>(target));
  hash = lite::hash_combine(hash, static_cast<int>(precision));
  hash = lite::hash_combine(hash, static_cast<int>(layout));
  hash = lite::hash_combine(hash, static_cast<int>(device));
  return hash;
}

bool operator<(const Place& a, const Place& b) {
  if (a.target != b.target) return a.target < b.target;
  if (a.precision != b.precision) return a.precision < b.precision;
  if (a.layout != b.layout) return a.layout < b.layout;
  if (a.device != b.device) return a.device < b.device;
  return false;
}

std::string Place::DebugString() const {
  STL::stringstream os;
  os << TargetToStr(target) << "/" << PrecisionToStr(precision) << "/"
     << DataLayoutToStr(layout);
  return os.str();
}

const std::string& TargetToStr(TargetType target) {
  static const std::string target2string[] = {
      "unk", "host", "x86", "cuda", "arm", "opencl", "any", "fpga", "npu"};
  auto x = static_cast<int>(target);
  CHECK_LT(x, static_cast<int>(TARGET(NUM)));
  return target2string[x];
}

const std::string& PrecisionToStr(PrecisionType precision) {
  static const std::string precision2string[] = {"unk",
                                                 "float",
                                                 "int8_t",
                                                 "int32_t",
                                                 "any",
                                                 "float16",
                                                 "bool",
                                                 "int64_t",
                                                 "int16_t"};
  auto x = static_cast<int>(precision);
  CHECK_LT(x, static_cast<int>(PRECISION(NUM)));
  return precision2string[x];
}

const std::string& DataLayoutToStr(DataLayoutType layout) {
  static const std::string datalayout2string[] = {"unk", "NCHW", "any", "NHWC"};
  auto x = static_cast<int>(layout);
  CHECK_LT(x, static_cast<int>(DATALAYOUT(NUM)));
  return datalayout2string[x];
}

const std::string& TargetRepr(TargetType target) {
  static const std::string target2string[] = {"kUnk",
                                              "kHost",
                                              "kX86",
                                              "kCUDA",
                                              "kARM",
                                              "kOpenCL",
                                              "kAny",
                                              "kFPGA",
                                              "kNPU"};
  auto x = static_cast<int>(target);
  CHECK_LT(x, static_cast<int>(TARGET(NUM)));
  return target2string[x];
}

const std::string& PrecisionRepr(PrecisionType precision) {
  static const std::string precision2string[] = {"kUnk",
                                                 "kFloat",
                                                 "kInt8",
                                                 "kInt32",
                                                 "kAny",
                                                 "kFP16",
                                                 "kBool",
                                                 "kInt64",
                                                 "kInt16"};
  auto x = static_cast<int>(precision);
  CHECK_LT(x, static_cast<int>(PRECISION(NUM)));
  return precision2string[x];
}

const std::string& DataLayoutRepr(DataLayoutType layout) {
  static const std::string datalayout2string[] = {
      "kUnk", "kNCHW", "kAny", "kNHWC"};
  auto x = static_cast<int>(layout);
  CHECK_LT(x, static_cast<int>(DATALAYOUT(NUM)));
  return datalayout2string[x];
}

}  // namespace lite_api
}  // namespace paddle
