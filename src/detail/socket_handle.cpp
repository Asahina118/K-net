#include <unistd.h>         // close()
#include <utility>

#include "src/detail/socket_handle.h"

namespace knet::detail
{
    SocketHandle::SocketHandle(int fileDescriptor) noexcept
        : fileDescriptor_(fileDescriptor)
    {
    }

    SocketHandle::~SocketHandle()
    {
        if (fileDescriptor_ != -1) {
            close(fileDescriptor_);
        }
    }

    int SocketHandle::get_fd() const noexcept
    {
        return fileDescriptor_;
    }

    SocketHandle::SocketHandle(SocketHandle&& other) noexcept
        : fileDescriptor_(std::exchange(other.fileDescriptor_, -1))
    {
    }

    SocketHandle& SocketHandle::operator=(SocketHandle&& other) noexcept
    {
        if (this == &other) return *this;

        if (fileDescriptor_ != -1) {
            close(fileDescriptor_);
        }
        fileDescriptor_ = std::exchange(other.fileDescriptor_, -1);
        return *this;
    }
}
