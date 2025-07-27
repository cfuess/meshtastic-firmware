#pragma once

#include "modules/Module.h"

class AutoResponseModule : public Module
{
  public:
    AutoResponseModule();

  protected:
    virtual bool handleReceived(const meshtastic_MeshPacket &mp) override;

  private:
    unsigned long lastResponseTime = 0;
};
