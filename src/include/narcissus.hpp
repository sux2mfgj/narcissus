#pragma once

namespace narcissus {
    namespace h8_3069f {
        enum class interrupts : std::uint8_t;
    } // namespace h8_3069f

    class narcissus 
    {
        public:
            virtual auto run(void) -> void = 0;
            virtual auto interrupt(h8_3069f::interrupts int_num) -> bool = 0;

    };
} // namespace narcissus
