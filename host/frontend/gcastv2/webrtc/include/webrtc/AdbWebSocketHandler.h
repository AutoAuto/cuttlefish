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

#pragma once

#include <https/WebSocketHandler.h>
#include <https/RunLoop.h>

#include <memory>

struct AdbWebSocketHandler
    : public WebSocketHandler,
      public std::enable_shared_from_this<AdbWebSocketHandler> {

    explicit AdbWebSocketHandler(
            std::shared_ptr<RunLoop> runLoop,
            const std::string &adb_host_and_port);

    ~AdbWebSocketHandler() override;

    void run();

    int handleMessage(
            uint8_t headerByte, const uint8_t *msg, size_t len) override;

private:
    struct AdbConnection;

    std::shared_ptr<RunLoop> mRunLoop;
    std::shared_ptr<AdbConnection> mAdbConnection;

    int mSocket;

    int setupSocket(const std::string &adb_host_and_port);
};

