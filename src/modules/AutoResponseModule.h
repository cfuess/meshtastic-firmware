#pragma once

#include "Module.h"
#include "MeshTypes.h"

class AutoResponseModule : public Module
{
  public:
    AutoResponseModule(const char *name);

  protected:
    virtual bool handleReceived(const meshtastic_MeshPacket &mp) override;

  private:
    unsigned long lastResponseTime = 0;
};
