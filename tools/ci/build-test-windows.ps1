# Copyright (c) 2021 by Apex.AI Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

# This script builds iceoryx_hoofs und iceoryx_posh and executes all tests

Write-Host "building sources"
cmake -Bbuild -Hiceoryx_meta -DBUILD_TEST=ON -DINTROSPECTION=OFF -DBINDING_C=ON -DEXAMPLES=ON -DCMAKE_CXX_FLAGS="/MP" && cmake --build build

Write-Host "running tests (excluding timing_tests)"
# until the windows support is fully implemented and we can use the windows cmake targets
# we have to exclude the tests explicitly until everyone is running
build\hoofs\test\Debug\hoofs_moduletests.exe --gtest_filter="-*TimingTest*:UnixDomainSocket*"
build\hoofs\test\Debug\hoofs_integrationtests.exe
build\binding_c\test\Debug\binding_c_moduletests.exe --gtest_filter="-BindingC_Runtime*:*TimingTest*"
build\posh\test\Debug\posh_moduletests.exe --gtest_filter="-PoshRuntime*:IceoryxRoudiApp*:IceoryxRoudiMemoryManager*:ProcessIntrospection*:ParseAllMalformedInput*:*TimingTest*"
build\posh\test\Debug\posh_integrationtests.exe --gtest_filter="-*ChunkBuildingBlocks*:PortUser*:RoudiFindService*:*TimingTest*"