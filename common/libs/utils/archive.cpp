/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common/libs/utils/archive.h"

#include <string>
#include <vector>

#include <android-base/strings.h>
#include <glog/logging.h>

#include "common/libs/utils/subprocess.h"

namespace cvd {

Archive::Archive(const std::string& file) : file(file) {
}

Archive::~Archive() {
}

std::vector<std::string> Archive::Contents() {
  cvd::Command bsdtar_cmd("/usr/bin/bsdtar");
  bsdtar_cmd.AddParameter("-tf");
  bsdtar_cmd.AddParameter(file);
  std::string bsdtar_input, bsdtar_output;
  auto bsdtar_ret = cvd::RunWithManagedStdio(std::move(bsdtar_cmd), &bsdtar_input,
                                             &bsdtar_output, nullptr);
  if (bsdtar_ret != 0) {
    LOG(ERROR) << "`bsdtar -tf \"" << file << "\"` returned " << bsdtar_ret;
  }
  return bsdtar_ret == 0
      ? android::base::Split(bsdtar_output, "\n")
      : std::vector<std::string>();
}

bool Archive::ExtractAll(const std::string& target_directory) {
  return ExtractFiles({}, target_directory);
}

bool Archive::ExtractFiles(const std::vector<std::string>& to_extract,
                           const std::string& target_directory) {
  cvd::Command bsdtar_cmd("/usr/bin/bsdtar");
  bsdtar_cmd.AddParameter("-x");
  bsdtar_cmd.AddParameter("-v");
  bsdtar_cmd.AddParameter("-C");
  bsdtar_cmd.AddParameter(target_directory);
  bsdtar_cmd.AddParameter("-f");
  bsdtar_cmd.AddParameter(file);
  bsdtar_cmd.AddParameter("-S");
  for (const auto& extract : to_extract) {
    bsdtar_cmd.AddParameter(extract);
  }
  bsdtar_cmd.RedirectStdIO(cvd::Subprocess::StdIOChannel::kStdOut,
                           cvd::Subprocess::StdIOChannel::kStdErr);
  auto bsdtar_ret = bsdtar_cmd.Start().Wait();
  if (bsdtar_ret != 0) {
    LOG(ERROR) << "bsdtar extraction on \"" << file << "\" returned " << bsdtar_ret;
  }
  return bsdtar_ret == 0;
}

std::string Archive::ExtractToMemory(const std::string& path) {
  cvd::Command bsdtar_cmd("/usr/bin/bsdtar");
  bsdtar_cmd.AddParameter("-xf");
  bsdtar_cmd.AddParameter(file);
  bsdtar_cmd.AddParameter("-O");
  bsdtar_cmd.AddParameter(path);
  std::string stdout, stderr;
  auto ret = RunWithManagedStdio(std::move(bsdtar_cmd), nullptr, &stdout,
                                 nullptr);
  if (ret != 0) {
    LOG(ERROR) << "Could not extract \"" << path << "\" from \"" << file
               << "\" to memory.";
    return "";
  }
  return stdout;
}

} // namespace cvd
