#include "uFtpFactory.h"
#include "uFtpServerCMD.h"
#include "cmd/uFtpUSER.h"
#include "cmd/uFtpList.h"
#include "cmd/uFtpPort.h"
#include "cmd/uFtpRETR.h"
#include "cmd/uFtpSTOR.h"

using namespace unl;

uTask *uFtpFactory::create_task()
{
    uFtpServerCMD *cmd = new uFtpServerCMD();
    cmd->reg("USER", new uFtpUSER());
    uFtpTask *lis_ttask = new uFtpList();
    cmd->reg("PWD", lis_ttask);
    cmd->reg("LIST", lis_ttask);
    cmd->reg("CWD", lis_ttask);
    cmd->reg("CDUP", lis_ttask);
    cmd->reg("PORT", new uFtpPort());
    cmd->reg("RETR", new uFtpRETR());
    cmd->reg("STOR", new uFtpSTOR());
    return cmd;
}

uFtpFactory::uFtpFactory() {}
uFtpFactory::~uFtpFactory() {}