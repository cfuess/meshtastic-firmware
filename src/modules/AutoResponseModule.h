#pragma once

#include "ProtobufModule.h"

class AutoResponseModule : public ProtobufModule
{
  public:
    AutoResponseModule();

  protected:
    virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

  private:
    unsigned long lastResponseTime = 0;
};
