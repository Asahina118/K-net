namespace knet::detail
{
    class SocketHandle
    {
    private:
        int fileDescriptor_ = -1;
    public:
        explicit SocketHandle(int fileDescriptor) noexcept;
        ~SocketHandle();

        SocketHandle(const SocketHandle&) = delete;
        SocketHandle& operator=(const SocketHandle&) = delete;

        SocketHandle(SocketHandle&&) noexcept;
        SocketHandle& operator=(SocketHandle&&) noexcept;

        int get_fd() const noexcept;
    };
}
