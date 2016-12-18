//Copyright 2015 Alessandro Viganò https://github.com/alvistar
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//        limitations under the License.

#include "MsgPack.h"
#include "MsgUnpack.h"
#include "mpack.h"
#include <sstream>

MsgPack::MsgPack() {
    mpack_writer_init(&writer, data, sizeof(data));
}

void MsgPack::pack_array(uint32_t number) {
    mpack_start_array(&writer, number);
}

void MsgPack::pack(String s) {
    mpack_write_cstr(&writer, s.c_str());
}

void MsgPack::clear() {
    mpack_writer_init(&writer, data, sizeof(data));
}

void MsgPack::pack(int32_t i) {
    mpack_write_i32(&writer, i);
}

void MsgPack::pack(int64_t i) {
    mpack_write_i32(&writer, i);
}

void MsgPack::pack(uint32_t i) {
    mpack_write_u64(&writer, i);
}

void MsgPack::pack(uint64_t i) {
    mpack_write_u64(&writer, i);
}

void MsgPack::pack_map(uint32_t n) {
    mpack_start_map(&writer, n);
}

size_t MsgPack::getUsedBuffer() const {
    return mpack_writer_buffer_used((mpack_writer_t*)&writer);
}

char *MsgPack::getData() const{
    return (char*) data;
}

void MsgPack::pack(MsgPack mp) {
    mpack_write_bytes(&writer, mp.getData(),mp.getUsedBuffer());
}




