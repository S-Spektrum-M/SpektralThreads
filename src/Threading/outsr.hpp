#include <mutex>
#include <ostream>

namespace Threading {

    class outsr {
        public:
            void write(const char *data, size_t len);
            void flush();
            static outsr * getInst();

        private:
            static outsr *inst;
            std::ostream &os_;
            std::mutex mutex_;
            outsr(std::ostream &os);
    };
} // namespace Threading
