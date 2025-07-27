#pragma once

#include "SinglePortModule.h"
#include "MeshTypes.h"

class AutoResponseModule : public SinglePortModule
{
  public:
    AutoResponseModule();

  protected:
    virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

  private:
    unsigned long lastResponseTime = 0;
};
