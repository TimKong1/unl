#include "uIO.h"

namespace unl
{
    class uHttpIO : public uIO
    {
    private:
        bool response_ok = 0;
    public:
        void read_callback();
        void write_callback();
        void event_callback(short what);
        uHttpIO(uBase *base, int fd);
        ~uHttpIO();
    };
}