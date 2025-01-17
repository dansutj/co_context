#pragma once

#include <co_context/log/log.hpp>

#include <coroutine>

namespace co_context::detail {

class trival_task {
  public:
    struct promise_type;

    explicit trival_task(std::coroutine_handle<promise_type> self_handle
    ) noexcept {
        this->handle = self_handle;
    }

    struct final_awaiter {
        [[nodiscard]]
        static constexpr bool await_ready() noexcept {
            return false;
        }

        static std::coroutine_handle<>
        await_suspend(std::coroutine_handle<promise_type> current) noexcept {
            auto continuation = current.promise().parent_coro;
            log::v("trival_task %lx destroy\n", current.address());
            current.destroy();
            return continuation;
        }

        // Won't be resumed anyway
        static constexpr void await_resume() noexcept {}
    };

    struct promise_type {
        static constexpr std::suspend_always initial_suspend() noexcept {
            return {};
        }

        static constexpr final_awaiter final_suspend() noexcept { return {}; }

        std::coroutine_handle<> parent_coro;

        trival_task get_return_object() noexcept {
            return trival_task{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        constexpr void unhandled_exception() noexcept {}

        constexpr void return_void() noexcept {}
    };

    static constexpr bool await_ready() noexcept { return false; }

    [[nodiscard]]
    std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting_coro
    ) const noexcept {
        handle.promise().parent_coro = awaiting_coro;
        return handle;
    }

    static constexpr void await_resume() noexcept {}

    std::coroutine_handle<promise_type> handle;
};

} // namespace co_context::detail
