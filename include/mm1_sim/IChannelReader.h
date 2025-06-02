//
// Created by Admin on 2025-05-29.
//

#ifndef ICHANNELREADER_H
#define ICHANNELREADER_H

#include <memory>

namespace mm1_sim {

    template<typename T>
    class IChannelReader {
    public:
        virtual std::shared_ptr<T> pop() = 0;
        virtual bool try_pop(std::shared_ptr<T>& out) = 0;
        virtual ~IChannelReader() = default;
    };

} // namespace mm2_sim

#endif //ICHANNELREADER_H
