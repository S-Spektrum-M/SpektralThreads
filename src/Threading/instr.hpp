#include <istream>
#include <mutex>

namespace Threading {
    class instr {
        public:
            size_t read(char* data, size_t len);
            static instr * getInst();
        private:
            static instr * inst;
            instr(std::istream& is);
            std::istream& is_;
            std::mutex mutex_;
    };
} // namespace Threading
