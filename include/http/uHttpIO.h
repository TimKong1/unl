#include "uIO.h"

namespace unl
{
    class uHttpIO : public uIO
    {
    private:
    public:
        void read_callback();
        void event_callback(short what);
        uHttpIO(uBase *base, int fd);
        ~uHttpIO();
    };
}