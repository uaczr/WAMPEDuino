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

#include "Arduino.h"
#include <string>
#include <sys/types.h>
#include <utility>
#include "mpack.h"

#ifndef CPPWAMP_MSGPACK_H
#define CPPWAMP_MSGPACK_H

#define MPARR(X) MComp {mpArr, X}
#define MPMAP(X) MComp {mpMap, X}


class MsgPack {
private:
    mpack_writer_t writer;
    char data[1024];

    void packAll() {

    }

    template <typename A>
    void packAll (A&& p1){
        pack(p1);
    }

    template <typename A, typename ...B>
    void packAll (A&& p1, B&&... params) {
        packAll (std::forward<A> (p1));
        packAll (std::forward<B>(params)...);
    }

public:
    MsgPack();
    MsgPack(int i):MsgPack() {
        pack(i);
    }

    MsgPack(String s):MsgPack() {
        pack(s);
    }

    bool isEmpty() {
        return mpack_writer_buffer_used(&writer) == 0;
    }

    void pack_array(uint32_t number);
    void pack_map(uint32_t n);
    void pack(String s);
    void pack(int32_t i);
    void pack(int64_t i);
    void pack(uint32_t i);
    void pack(uint64_t i);
    void pack(MsgPack mp);
    void clear();
    size_t getUsedBuffer() const;
    char* getData() const;

    template <typename ...ITEMS>
    void packArray(ITEMS&&... items) {
        uint elements = uint(sizeof ...(items));
        pack_array(elements);
        packAll(std::forward<ITEMS>(items)...);
    }

    template <typename T>
    MsgPack& operator << (const T& data) {
        pack(data);
        return *this;
    }



};

class MsgPackArr: public MsgPack {
private:
    void packAll() {

    }

    template <typename A>
    void packAll (A&& p1){
        pack(p1);
    }

    template <typename A, typename ...B>
    void packAll (A&& p1, B&&... params) {
        packAll (std::forward<A> (p1));
        packAll (std::forward<B>(params)...);
    }

public:

    template <typename ...A>
    MsgPackArr(A&&... params) {
        uint elements = uint(sizeof ...(A));
        pack_array(elements);
        packAll(std::forward<A>(params)...);
    }


};

class MsgPackMap: public MsgPack {
private:
    void packAll() {
    }

    template <typename A>
    void packAll (A&& p1){
        pack(p1);
    }

    template <typename A, typename ...B>
    void packAll (A&& p1, B&&... params)
    {
        packAll (std::forward<A> (p1));
        packAll (std::forward<B>(params)...);
    }
public:

    template <typename ...A>
    MsgPackMap(A&&... params) {
        uint elements = (uint(sizeof ...(A)))/2;
        pack_map(elements);
        packAll(std::forward<A>(params)...);
    }
};


#endif //CPPWAMP_MSGPACK_H
