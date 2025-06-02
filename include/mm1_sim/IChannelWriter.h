//
// Created by Admin on 2025-05-29.
//

#ifndef ICHANNELWRITER_H
#define ICHANNELWRITER_H

#include <memory>

namespace mm1_sim {

    template<typename T>
    class IChannelWriter {
    public:
        virtual void push(std::shared_ptr<T> item) = 0;
        virtual ~IChannelWriter() = default;
    };

} // namespace mm2_sim

#endif //ICHANNELWRITER_H
